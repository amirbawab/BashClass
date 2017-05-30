#include <bashclass/BashClass.h>
#include <bashclass/BTypes.h>
#include <bashclass/BClass.h>
#include <bashclass/BIf.h>
#include <bashclass/BWhile.h>
#include <iostream>

BashClass::BashClass() {
    m_global = std::make_shared<BGlobal>();
    m_scopeStack.push_back(m_global);
    initHandlers();
}

void BashClass::printStructure() {
    std::cout << m_global->getStructure().str() << std::endl;
}

/**
 * Link variable types in the provided scope
 */
void _linkVariablesTypes(std::shared_ptr<BScope> scope, std::shared_ptr<BScope> globalScope) {
    // Evaluate all variables in that scope
    for(auto variable : scope->findAllVariables()) {
        if(!BType::isBuiltInType(variable->getType()->getName())) {
            // Find class scope of that type
            auto cls = globalScope->findAllClasses(variable->getType()->getValue().c_str());
            if(cls.empty()) {
                std::cerr << "Undefined type " << variable->getType()->getValue() <<
                " for variable " << variable->getName()->getValue() << std::endl;
            } else {
                variable->setTypeScope(cls.front());
            }
        }
    }
}

/**
 * Link functions types in the provided scope
 */
void _linkFunctionsTypes(std::shared_ptr<BScope> scope, std::shared_ptr<BScope> globalScope) {
    // Evaluate all functions in that scope
    for(auto function : scope->findAllFunctions()) {
        auto castFunction = std::dynamic_pointer_cast<BFunction>(function);
        if(!BType::isBuiltInType(castFunction->getType()->getName())) {
            // Find class scope of that type
            auto cls = globalScope->findAllClasses(castFunction->getType()->getValue().c_str());
            if(cls.empty()) {
                std::cerr << "Undefined type " << castFunction->getType()->getValue() <<
                " for function " << castFunction->getName()->getValue() << std::endl;
            } else {
                castFunction->setTypeScope(cls.front());
            }
        }
    }
}

/**
 * Run a depth-first traversal and link all the variables
 * to their types
 */
void _linkTypes(std::shared_ptr<BScope> globalScope) {

    std::stack<std::shared_ptr<BScope>> scopeStack;
    scopeStack.push(globalScope);
    while(!scopeStack.empty()) {

        auto top = scopeStack.top();
        scopeStack.pop();

        _linkVariablesTypes(top,globalScope);
        _linkFunctionsTypes(top,globalScope);

        for(auto scope : top->getAllScopes()) {
            scopeStack.push(scope);
        }
    }
}

/**
 * Run a depth-first traversal on variable types and detect
 * all circular references
 */
void _detectCircularReference(std::shared_ptr<BScope> scope) {
    for(auto cls : scope->findAllClasses()) {
        std::stack<std::shared_ptr<BScope>> scopeStack;
        scopeStack.push(cls);
        std::set<std::shared_ptr<BScope>> visited;
        while(!scopeStack.empty()) {

            // Get top class scope
            auto top = scopeStack.top();
            scopeStack.pop();

            // Mark class as visited
            visited.insert(top);

            // Evaluate all variables in the class scope
            for(auto variable : top->findAllVariables()) {
                if(variable->getTypeScope()) {
                    if(visited.find(variable->getTypeScope()) == visited.end()) {
                        scopeStack.push(variable->getTypeScope());
                    } else {
                        auto castClass = std::dynamic_pointer_cast<BClass>(cls);
                        std::cerr << "Circular reference detected from class " << castClass->getName()->getValue() << std::endl;
                    }
                }
            }
        }

    }
}

/**
 * Check if class was already defined
 */
void _checkDuplicateClass(ecc::LexicalToken &token, std::vector<std::shared_ptr<BScope>> &scopeStack) {
    auto getClass = scopeStack.back()->getParentScope()->findAllClasses(token.getValue().c_str());
    if(!getClass.empty()) {
        std::cerr << "Class " << token.getValue() << " at line " << token.getLine()
        << " was defined previously" << std::endl;
    }
}

/**
 * Check if function was already defined in the scope
 */
void _checkDuplicateFunction(ecc::LexicalToken &token, std::vector<std::shared_ptr<BScope>> &scopeStack) {
    auto getFunc = scopeStack.back()->getParentScope()->findAllFunctions(token.getValue().c_str());
    if(!getFunc.empty()) {
        std::cerr << "Function " << token.getValue() << " at line " << token.getLine()
        <<" was defined previously in the same scope" << std::endl;
    }
}

/**
 * Check if variable was already defined in the scope
 */
void _checkDuplicateVariable(ecc::LexicalToken &token, std::vector<std::shared_ptr<BScope>> &scopeStack) {
    auto getVar = scopeStack.back()->findAllVariables(token.getValue().c_str());
    if(!getVar.empty()) {
        std::cerr << "Variable " << token.getValue() << " at line " << token.getLine()
        << " was defined previously in the same scope" << std::endl;
    }
}

/**
 * Check if parameter was already defined in the scope
 */
void _checkDuplicateParameter(ecc::LexicalToken &token, std::vector<std::shared_ptr<BScope>> &scopeStack) {
    auto getVar = scopeStack.back()->findAllParameters(token.getValue().c_str());
    if(!getVar.empty()) {
        std::cerr << "Parameter " << token.getValue() << " at line " << token.getLine()
        << " was defined previously in the same scope" << std::endl;
    }
}

/**
 * Initialize semantic action handlers
 */
void BashClass::initHandlers() {

    /**************************************
     *          PROGRAM
     **************************************/
    m_start = [&](int phase, LexicalTokens &lexicalVector, int index, bool stable){
        if(phase == BashClass::PHASE_EVAL) {
            _linkTypes(m_global);
            _detectCircularReference(m_global);
        }
    };

    m_end = [&](int phase, LexicalTokens &lexicalVector, int index, bool stable){};

    /**************************************
     *          CLASSES
     **************************************/
    m_startClass = [&](int phase, LexicalTokens &lexicalVector, int index, bool stable){
        if(phase == BashClass::PHASE_CREATE) {
            auto newClass = m_scopeStack.back()->createClass(lexicalVector[index]);
            m_scopeStack.push_back(newClass);
        } else if(phase == BashClass::PHASE_EVAL) {
            auto newClass = m_scopeStack.back()->getScopeByToken(lexicalVector[index]);
            m_scopeStack.push_back(newClass);
        }
    };

    m_className = [&](int phase, LexicalTokens &lexicalVector, int index, bool stable){
        if(phase == BashClass::PHASE_CREATE) {

            // Check if class already exists
            _checkDuplicateClass(*lexicalVector[index], m_scopeStack);

            // Set class name
            auto createdClass = std::dynamic_pointer_cast<BClass>(m_scopeStack.back());
            createdClass->setName(lexicalVector[index]);
        }
    };

    m_endClass = [&](int phase, LexicalTokens &lexicalVector, int index, bool stable){
        if(phase == BashClass::PHASE_CREATE || phase == BashClass::PHASE_EVAL) {
            m_scopeStack.pop_back();
        }
    };

    /**************************************
     *          FUNCTIONS DECLARATION
     **************************************/
    m_startFunction = [&](int phase, LexicalTokens &lexicalVector, int index, bool stable){
        if(phase == BashClass::PHASE_CREATE) {
            auto newFunction = m_scopeStack.back()->createFunction(lexicalVector[index]);
            m_scopeStack.push_back(newFunction);
        } else if(phase == BashClass::PHASE_EVAL) {
            auto newFunction = m_scopeStack.back()->getScopeByToken(lexicalVector[index]);
            m_scopeStack.push_back(newFunction);
        }
    };

    m_functionType = [&](int phase, LexicalTokens &lexicalVector, int index, bool stable){
        if(phase == BashClass::PHASE_CREATE) {
            auto createdFunction = std::dynamic_pointer_cast<BFunction>(m_scopeStack.back());
            createdFunction->setType(lexicalVector[index]);
        }
    };

    m_functionName = [&](int phase, LexicalTokens &lexicalVector, int index, bool stable){
        if(phase == BashClass::PHASE_CREATE) {

            // Check if function already exists in the current scope
            _checkDuplicateFunction(*lexicalVector[index],m_scopeStack);

            auto createdFunction = std::dynamic_pointer_cast<BFunction>(m_scopeStack.back());
            createdFunction->setName(lexicalVector[index]);
        }
    };

    m_endFunction = [&](int phase, LexicalTokens &lexicalVector, int index, bool stable){
        if(phase == BashClass::PHASE_CREATE) {
            m_scopeStack.pop_back();
        } else if(phase == BashClass::PHASE_EVAL) {
            auto functionScope = std::dynamic_pointer_cast<BFunction>(m_scopeStack.back());
            functionScope->verifyParameters();
            m_scopeStack.pop_back();
        }
    };

    /**************************************
     *          VARIABLES DECLARATION
     **************************************/
    m_startVar = [&](int phase, LexicalTokens &lexicalVector, int index, bool stable){
        if(phase == BashClass::PHASE_CREATE) {
            auto variable = m_scopeStack.back()->createVariable(lexicalVector[index]);
            m_focusVariable = variable;
        }
    };

    m_varType = [&](int phase, LexicalTokens &lexicalVector, int index, bool stable){
        if(phase == BashClass::PHASE_CREATE) {
            m_focusVariable->setType(lexicalVector[index]);
        }
    };

    m_varName = [&](int phase, LexicalTokens &lexicalVector, int index, bool stable){
        if(phase == BashClass::PHASE_CREATE) {

            // Check if variable already exists in the current scope
            _checkDuplicateVariable(*lexicalVector[index], m_scopeStack);

            m_focusVariable->setName(lexicalVector[index]);
        }
    };

    m_endVar = [&](int phase, LexicalTokens &lexicalVector, int index, bool stable){
        if(phase == BashClass::PHASE_CREATE) {
            m_focusVariable = nullptr;
        }
    };

    /**************************************
     *          PARAMETERS
     **************************************/
    m_startParam = [&](int phase, LexicalTokens &lexicalVector, int index, bool stable){
        if(phase == BashClass::PHASE_CREATE) {
            auto createdVar = m_scopeStack.back()->createVariable(lexicalVector[index]);
            createdVar->setIsParam(true);
            m_focusVariable = createdVar;
        }
    };

    m_paramType = [&](int phase, LexicalTokens &lexicalVector, int index, bool stable){
        if(phase == BashClass::PHASE_CREATE) {
            m_focusVariable->setType(lexicalVector[index]);
        }
    };

    m_paramName = [&](int phase, LexicalTokens &lexicalVector, int index, bool stable){
        if(phase == BashClass::PHASE_CREATE) {

            // Check if parameter already exists in the current scope
            _checkDuplicateParameter(*lexicalVector[index], m_scopeStack);

            m_focusVariable->setName(lexicalVector[index]);
        }
    };

    m_endParam = [&](int phase, LexicalTokens &lexicalVector, int index, bool stable){
        if(phase == BashClass::PHASE_CREATE) {
            m_focusVariable = nullptr;
        }
    };

    /**************************************
     *          WHILE
     **************************************/

    m_startWhile = [&](int phase, LexicalTokens &lexicalVector, int index, bool stable){
        if(phase == BashClass::PHASE_CREATE) {
            auto newWhile = m_scopeStack.back()->createWhile(lexicalVector[index]);
            m_scopeStack.push_back(newWhile);
        } else if(phase == BashClass::PHASE_EVAL) {
            auto newWhile = m_scopeStack.back()->getScopeByToken(lexicalVector[index]);
            m_scopeStack.push_back(newWhile);
        }
    };

    m_endWhile = [&](int phase, LexicalTokens &lexicalVector, int index, bool stable){
        if(phase == BashClass::PHASE_CREATE || phase == BashClass::PHASE_EVAL) {
            m_scopeStack.pop_back();
        }
    };

    m_whileCond = [&](int phase, LexicalTokens &lexicalVector, int index, bool stable){
        if(phase == BashClass::PHASE_EVAL) {
            auto whileScope = std::dynamic_pointer_cast<BWhile>(m_scopeStack.back());
            whileScope->setCondition(m_expressionOperandStack.back());
            m_expressionOperandStack.pop_back();
        }
    };

    /**************************************
     *          IF
     **************************************/

    m_startIf = [&](int phase, LexicalTokens &lexicalVector, int index, bool stable){
        if(phase == BashClass::PHASE_CREATE) {
            auto newIf = m_scopeStack.back()->createIf(lexicalVector[index]);
            m_scopeStack.push_back(newIf);
        } else if(phase == BashClass::PHASE_EVAL) {
            auto newIf = m_scopeStack.back()->getScopeByToken(lexicalVector[index]);
            m_scopeStack.push_back(newIf);
        }
    };

    m_endIf = [&](int phase, LexicalTokens &lexicalVector, int index, bool stable){
        if(phase == BashClass::PHASE_CREATE || phase == BashClass::PHASE_EVAL) {
            m_scopeStack.pop_back();
        }
    };

    m_ifCond = [&](int phase, LexicalTokens &lexicalVector, int index, bool stable){
        if(phase == BashClass::PHASE_EVAL) {
            auto ifScope = std::dynamic_pointer_cast<BIf>(m_scopeStack.back());
            ifScope->setCondition(m_expressionOperandStack.back());
            m_expressionOperandStack.pop_back();
        }
    };

    /**************************************
     *          VARIABLE AND FUNCTION CALL
     **************************************/

    m_startOuterCall = [&](int phase, LexicalTokens &lexicalVector, int index, bool stable){
        if(phase == BashClass::PHASE_EVAL) {
            m_chainBuilderStack.push_back(std::make_shared<BChainCall>());
        }
    };

    m_endOuterCall = [&](int phase, LexicalTokens &lexicalVector, int index, bool stable){
        if(phase == BashClass::PHASE_EVAL) {
            // Do nothing ...
        }
    };

    m_startInnerCall = [&](int phase, LexicalTokens &lexicalVector, int index, bool stable){
        if(phase == BashClass::PHASE_EVAL) {
            m_chainBuilderStack.push_back(std::make_shared<BChainCall>());
        }
    };

    m_endInnerCall = [&](int phase, LexicalTokens &lexicalVector, int index, bool stable){
        if(phase == BashClass::PHASE_EVAL) {
            m_expressionOperandStack.push_back(m_chainBuilderStack.back());
            m_chainBuilderStack.pop_back();
        }
    };

    m_varCall = [&](int phase, LexicalTokens &lexicalVector, int index, bool stable){
        if(phase == BashClass::PHASE_EVAL) {
            m_chainBuilderStack.back()->addVariable(m_scopeStack.back(), lexicalVector[index]);
        }
    };

    m_functionCall = [&](int phase, LexicalTokens &lexicalVector, int index, bool stable){
        if(phase == BashClass::PHASE_EVAL) {
            m_chainBuilderStack.back()->addFunction(m_global, lexicalVector[index]);
        }
    };

    m_tokenCall = [&](int phase, LexicalTokens &lexicalVector, int index, bool stable){
        if(phase == BashClass::PHASE_EVAL) {
            auto token = std::make_shared<BTokenCall>();
            token->setLexicalToken(lexicalVector[index]);
            m_chainBuilderStack.back()->addToken(token);
        }
    };

    /**************************************
     *          OPERATOR
     **************************************/

    m_putOp = [&](int phase, LexicalTokens &lexicalVector, int index, bool stable){
        if(phase == BashClass::PHASE_EVAL) {
            m_expressionOperatorStack.push_back(lexicalVector[index]);
        }
    };

    /**************************************
     *          ARGUMENT
     **************************************/

    m_startArgument = [&](int phase, LexicalTokens &lexicalVector, int index, bool stable){
        if(phase == BashClass::PHASE_EVAL) {
            // TODO
        }
    };

    m_setArgument = [&](int phase, LexicalTokens &lexicalVector, int index, bool stable){
        if(phase == BashClass::PHASE_EVAL) {

            // Get function that is currently being built
            auto functionCall = std::dynamic_pointer_cast<BFunctionCall>(m_chainBuilderStack.back()->last());

            // Add the argument to it
            functionCall->addArgument(m_expressionOperandStack.back());

            // Remove consumed expression
            m_expressionOperandStack.pop_back();
        }
    };

    m_endArgument = [&](int phase, LexicalTokens &lexicalVector, int index, bool stable){
        if(phase == BashClass::PHASE_EVAL) {
            // TODO
        }
    };

    /**************************************
     *          EXPRESSION
     **************************************/

    m_startExpr = [&](int phase, LexicalTokens &lexicalVector, int index, bool stable){
        if(phase == BashClass::PHASE_EVAL) {
            // TODO
        }
    };

    m_createExpr = [&](int phase, LexicalTokens &lexicalVector, int index, bool stable){
        if(phase == BashClass::PHASE_EVAL) {

            // Get all expression elements
            auto rightOperand = m_expressionOperandStack.back();
            m_expressionOperandStack.pop_back();
            auto leftOperand = m_expressionOperandStack.back();
            m_expressionOperandStack.pop_back();
            auto operatorToken = m_expressionOperatorStack.back();
            m_expressionOperatorStack.pop_back();

            // Create expression
            auto expression = std::make_shared<BExpressionCall>();
            expression->setRightOperand(rightOperand);
            expression->setLeftOperand(leftOperand);
            expression->setOperator(operatorToken);

            // Push expression again as an operand
            m_expressionOperandStack.push_back(expression);
        }
    };

    m_endExpr = [&](int phase, LexicalTokens &lexicalVector, int index, bool stable){
        if(phase == BashClass::PHASE_EVAL) {
            // TODO
        }
    };

    m_varAssign = [&](int phase, LexicalTokens &lexicalVector, int index, bool stable){
        if(phase == BashClass::PHASE_EVAL) {
            auto variableCall = std::dynamic_pointer_cast<BVariableCall>(m_chainBuilderStack.back()->last());
            m_chainBuilderStack.pop_back();
            variableCall->setExpression(m_expressionOperandStack.back());
            m_expressionOperandStack.pop_back();
        }
    };

    m_returnExpr = [&](int phase, LexicalTokens &lexicalVector, int index, bool stable){
        if(phase == BashClass::PHASE_EVAL) {
            auto functionScope = std::dynamic_pointer_cast<BFunction>(m_scopeStack.back());
            functionScope->setReturnExpression(m_expressionOperandStack.back());
            m_expressionOperandStack.pop_back();
        }
    };
}
