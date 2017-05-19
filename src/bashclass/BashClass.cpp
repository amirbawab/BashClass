#include <bashclass/BashClass.h>
#include <bashclass/BTypes.h>
#include <bashclass/BClass.h>
#include <bashclass/BCallableToken.h>
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
                variable->setKnownType(false);
            } else {
                variable->setTypeScope(cls.front());
                variable->setKnownType(true);
            }
        } else {
            variable->setKnownType(true);
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
                std::cerr << "Undefined type " << castFunction->getType() <<
                " for function " << castFunction->getName() << std::endl;
                castFunction->setKnownType(false);
            } else {
                castFunction->setTypeScope(cls.front());
                castFunction->setKnownType(true);
            }
        } else {
            castFunction->setKnownType(true);
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
                        std::cerr << "Circular reference detected from class " << castClass->getName() << std::endl;
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
        std::cerr << "Class '" << token.getValue() << "' at line " << token.getLine()
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
 * Make sure that functions, members of classes, have at least one param
 */
void _requiredParam(std::shared_ptr<BScope> functionScope) {

    // Check if function is a member of a class
    auto castFunction = std::dynamic_pointer_cast<BFunction>(functionScope);
    if(castFunction->isClassMember()) {
        auto castClass = std::dynamic_pointer_cast<BClass>(functionScope->getParentScope());
        auto params = functionScope->findAllParameters();
        if(params.empty() || params[0]->getTypeScope() != castClass) {
            std::cerr << "Function " << castFunction->getName()->getValue() << " in class "
            << castClass->getName()->getValue() <<" must have the first argument of type "
            << castClass->getName()->getValue() << std::endl;
        }
    }
}

/**
 * Find first variable, and add it to the callable chain
 */
void _findFirstChainVariable(std::shared_ptr<BScope> scope, std::vector<std::shared_ptr<IBCallable>> &callableChain,
                    std::shared_ptr<ecc::LexicalToken> token) {
    auto variable = scope->findClosestVariable(token->getValue());
    if(variable) {
        // Check if the variable is a class member
        if(variable->isClassMember()) {
            std::cerr << "Use the first parameter of the function to refer to the variable "
            << variable->getName() << " at line " << token->getLine() << " and column "
            << token->getColumn() << std::endl;

            // Push variable instead of nullptr because the meaning is correct
            callableChain.push_back(variable);
        } else {
            callableChain.push_back(variable);
        }
    } else {
        std::cerr << "Undefined variable " << token->getValue()
        << " at line " << token->getLine() << " and column "
        << token->getColumn() << std::endl;
        callableChain.push_back(nullptr);
    }
}

/**
 * Find first function, and add it to the callable chain
 */
void _findFirstChainFunction(std::shared_ptr<BScope> globalScope, std::vector<std::shared_ptr<IBCallable>> &callableChain,
                             std::shared_ptr<ecc::LexicalToken> token) {
    auto functions = globalScope->findAllFunctions(token->getValue().c_str());
    if(!functions.empty()) {
        auto function = std::dynamic_pointer_cast<BFunction>(functions.front());
        callableChain.push_back(function);
    } else {
        std::cerr << "Undefined function " << token->getValue()
        << " at line " << token->getLine() << " and column "
        << token->getColumn() << std::endl;
        callableChain.push_back(nullptr);
    }
}

/**
 * Find next variable as a member of the type of the previous element in the chain
 */
void _findNextChainVariableInPrevType(std::shared_ptr<BScope> typeScope,
                                              std::vector<std::shared_ptr<IBCallable>> &callableChain,
                                              std::shared_ptr<ecc::LexicalToken> token) {
    if(typeScope) {
        auto variables = typeScope->findAllVariables(token->getValue().c_str());
        if(!variables.empty()) {
            callableChain.push_back(variables.front());
        } else {
            auto castClass = std::dynamic_pointer_cast<BClass>(typeScope);
            std::cerr << "Class " << castClass->getName() << " does not have a variable member "
            << token->getValue() << " at line "
            << token->getLine() << " and column "
            << token->getColumn() << std::endl;
            callableChain.push_back(nullptr);
        }
    } else {
        std::cerr << "Variable member " << token->getValue()
        << " at line " << token->getLine()
        << " and column " << token->getColumn()
        << " does not exist. Previous element type does not have any known members." << std::endl;
        callableChain.push_back(nullptr);
    }
}

/**
 * Find next variable, and add it to the callable chain
 */
void _findNextChainVariable(std::vector<std::shared_ptr<IBCallable>> &callableChain,
                            std::shared_ptr<ecc::LexicalToken> token) {

    if(!callableChain.back()) {
        std::cerr << "Cannot access variable member " << token->getValue()
        << " of undefined at line " << token->getLine() << " and column "
        << token->getColumn() << std::endl;
    } else {
        auto castPrevVariable = std::dynamic_pointer_cast<BVariable>(callableChain.back());
        auto castPrevFunction = std::dynamic_pointer_cast<BFunction>(callableChain.back());
        if(castPrevVariable) {
            _findNextChainVariableInPrevType(castPrevVariable->getTypeScope(), callableChain, token);
        } else if(castPrevFunction) {
            _findNextChainVariableInPrevType(castPrevFunction->getTypeScope(), callableChain, token);
        }
    }
}

/**
 * Find next function as a member of the type of the previous element in the chain
 */
void _findNextChainFunctionInPrevType(std::shared_ptr<BScope> typeScope,
                                      std::vector<std::shared_ptr<IBCallable>> &callableChain,
                                      std::shared_ptr<ecc::LexicalToken> token) {
    if(typeScope) {
        auto functions = typeScope->findAllFunctions(token->getValue().c_str());
        if(!functions.empty()) {
            auto function = std::dynamic_pointer_cast<BFunction>(functions.front());
            callableChain.push_back(function);
        } else {
            auto castClass = std::dynamic_pointer_cast<BClass>(typeScope);
            std::cerr << "Class " << castClass->getName() << " does not have a function member "
            << token->getValue() << " at line "
            << token->getLine() << " and column "
            << token->getColumn() << std::endl;
            callableChain.push_back(nullptr);
        }
    } else {
        std::cerr << "Function member " << token->getValue()
        << " at line " << token->getLine()
        << " and column " << token->getColumn()
        << " does not exist. Previous element type does not have any known members." << std::endl;
        callableChain.push_back(nullptr);
    }
}

/**
 * Find next function, and add it to the callable chain
 */
void _findNextChainFunction(std::vector<std::shared_ptr<IBCallable>> &callableChain,
                            std::shared_ptr<ecc::LexicalToken> token) {

    if(!callableChain.back()) {
        std::cerr << "Cannot access function member " << token->getValue()
        << " of undefined at line " << token->getLine() << " and column "
        << token->getColumn() << std::endl;
    } else {
        auto castPrevVariable = std::dynamic_pointer_cast<BVariable>(callableChain.back());
        auto castPrevFunction = std::dynamic_pointer_cast<BFunction>(callableChain.back());
        if(castPrevVariable) {
            _findNextChainFunctionInPrevType(castPrevVariable->getTypeScope(), callableChain, token);
        } else if(castPrevFunction) {
            _findNextChainFunctionInPrevType(castPrevFunction->getTypeScope(), callableChain, token);
        }
    }
}

/**
 * Check if the number and type of parameters is correct
 */
void _checkNumberAndTypeOfParameters(std::shared_ptr<BFunction> function,
                                     std::vector<std::shared_ptr<BExpression>> &argumentList) {
    auto parameters = function->findAllParameters();

    if(parameters.size() != argumentList.size()) {
        std::cerr << "Function " << function->getName()->getValue()
        << " expects " << parameters.size() << " arguments but given "
        << argumentList.size() << " instead" << std::endl;
    } else {
        for(size_t i = 0; i < parameters.size(); i++) {
            if(!parameters[i]->hasKnownType()) {
                std::cerr << "Cannot pass argument value to an undefined parameter type" << std::endl;
            } else {
                std::string argumentType = argumentList[i]->getDominantType();
                std::string parameterType = parameters[i]->getTypeValue();
                if(!argumentList[i]->isValid() ||
                        (parameterType != BType::TYPE_VALUE_ANY && parameterType != argumentType)) {
                    std::cerr << "Function " << function->getName()->getValue()
                    << " expects argument " << i + 1 << " to be of type " << parameterType
                    << " but given " << argumentType << std::endl;
                }
            }
        }
    }
}

/**
 * If function exists, then compare its signature with the function call
 */
void _compareFunctionCallAndSignature(std::shared_ptr<IBCallable> callable,
                                      std::vector<std::shared_ptr<BExpression>> &arguments) {
    // Get function from last callable chain
    auto function = std::dynamic_pointer_cast<BFunction>(callable);
    if(function) {
        _checkNumberAndTypeOfParameters(function, arguments);
    } else {
        std::cerr << "Cannot set argument to undefined function" << std::endl;
    }
}

/**
 * Check if the variable and the assigned expression have the same type
 */
void _checkAssignType(std::shared_ptr<IBCallable> chainVariablePtr, std::shared_ptr<BExpression> expression) {
    auto chainVariable = std::dynamic_pointer_cast<BVariable>(chainVariablePtr);
    if(chainVariable) {
        if(!chainVariable->hasKnownType()) {
            std::cerr << "Cannot assign an expression to a variable of undefined type" << std::endl;
        } else {
            std::string expressionType = expression->getDominantType();
            std::string variableType = chainVariable->getTypeValue();
            if(!expression->isValid() ||
                    (variableType != BType::TYPE_VALUE_ANY && expressionType != variableType)) {
                std::cerr << "Variable " << chainVariable->getName()->getValue()
                << " expects an expression of type " << variableType
                << " but given " << expressionType << std::endl;
            }
        }
    } else {
        std::cerr << "Cannot assign expression to undefined variable" << std::endl;
    }
}

/**
 * Check if the return type matches the function signature
 */
void _checkReturnValue(std::shared_ptr<BScope> scope, std::shared_ptr<BExpression> expression) {
    auto scopeFunction = std::dynamic_pointer_cast<BFunction>(scope);
    if(scopeFunction->hasKnowType()) {

        // If function is of type void, then a return statement is not expected
        if(scopeFunction->getType()->getName() == BType::TYPE_NAME_VOID) {
            std::cerr << "Function " << scopeFunction->getName()->getValue()
            << " does not expect to return an expression" << std::endl;
        } else {
            std::string functionType = scopeFunction->getTypeValue();
            std::string expressionType = expression->getDominantType();
            if (!expression->isValid() || functionType != expressionType) {
                std::cerr << "Function " << scopeFunction->getName()->getValue()
                << " expects to return " << functionType << " instead of " << expressionType << std::endl;
            }
        }
    } else {
        std::cerr << "Cannot check if return value is correct because function "
        << scopeFunction->getName()->getValue() << " has an undefined type" << std::endl;
    }
}

/**
 * Check if function is expecting a return statement
 */
void _checkNeedReturn(std::shared_ptr<BScope> scope) {
    auto function = std::dynamic_pointer_cast<BFunction>(scope);
    if(function->getType()->getName() != BType::TYPE_NAME_VOID && !function->hasReturn()) {
        std::cerr << "Function " << function->getName()->getValue()
        << " is missing a return statement" << std::endl;
    }
}

void _checkCondition(std::shared_ptr<BExpression> expression, std::shared_ptr<BScope> scope) {
    if(!expression->isValid() || expression->getDominantType() != BType::TYPE_VALUE_INT) {
        std::cerr << "Condition expression must evaluate to an integer at line "
        << scope->getReferenceToken()->getLine() << std::endl;
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
            _requiredParam(m_scopeStack.back());
            _checkNeedReturn(m_scopeStack.back());
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
            whileScope->setCondition(m_expressionStack.back());
            _checkCondition(m_expressionStack.back(), m_scopeStack.back());
            m_expressionStack.pop_back();
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
            ifScope->setCondition(m_expressionStack.back());
            _checkCondition(m_expressionStack.back(), m_scopeStack.back());
            m_expressionStack.pop_back();
        }
    };

    /**************************************
     *          VARIABLE AND FUNCTION CALL
     **************************************/

    m_startOuterCall = [&](int phase, LexicalTokens &lexicalVector, int index, bool stable){
        if(phase == BashClass::PHASE_EVAL) {
            m_callableChainStack.push_back({});
        }
    };

    m_endOuterCall = [&](int phase, LexicalTokens &lexicalVector, int index, bool stable){
        if(phase == BashClass::PHASE_EVAL) {
            m_callableChainStack.pop_back();
        }
    };

    m_startInnerCall = [&](int phase, LexicalTokens &lexicalVector, int index, bool stable){
        if(phase == BashClass::PHASE_EVAL) {
            m_callableChainStack.push_back({});
        }
    };

    m_endInnerCall = [&](int phase, LexicalTokens &lexicalVector, int index, bool stable){
        if(phase == BashClass::PHASE_EVAL) {
            m_expressionStack.back()->addOperand(m_callableChainStack.back());
            m_callableChainStack.pop_back();
        }
    };

    m_varCall = [&](int phase, LexicalTokens &lexicalVector, int index, bool stable){
        if(phase == BashClass::PHASE_EVAL) {
            if(m_callableChainStack.back().empty()) {
                _findFirstChainVariable(m_scopeStack.back(), m_callableChainStack.back(), lexicalVector[index]);
            } else {
                _findNextChainVariable(m_callableChainStack.back(), lexicalVector[index]);
            }
        }
    };

    m_functionCall = [&](int phase, LexicalTokens &lexicalVector, int index, bool stable){
        if(phase == BashClass::PHASE_EVAL) {
            if(m_callableChainStack.back().empty()) {
                _findFirstChainFunction(m_global, m_callableChainStack.back(), lexicalVector[index]);
            } else {
                _findNextChainFunction(m_callableChainStack.back(), lexicalVector[index]);
            }
        }
    };

    m_tokenCall = [&](int phase, LexicalTokens &lexicalVector, int index, bool stable){
        if(phase == BashClass::PHASE_EVAL) {
            // Example token calls: integer
            auto callableToken = std::make_shared<BCallableToken>();
            callableToken->setLexicalToken(lexicalVector[index]);

            // Add token to expression
            m_expressionStack.back()->addOperand({callableToken});
        }
    };

    /**************************************
     *          OPERATOR
     **************************************/

    m_putOp = [&](int phase, LexicalTokens &lexicalVector, int index, bool stable){
        if(phase == BashClass::PHASE_EVAL) {
            m_expressionStack.back()->addOperator(lexicalVector[index]);
        }
    };

    /**************************************
     *          ARGUMENT
     **************************************/

    m_startArgument = [&](int phase, LexicalTokens &lexicalVector, int index, bool stable){
        if(phase == BashClass::PHASE_EVAL) {
            m_argumentListStack.push_back({});
        }
    };

    m_setArgument = [&](int phase, LexicalTokens &lexicalVector, int index, bool stable){
        if(phase == BashClass::PHASE_EVAL) {
            m_argumentListStack.back().push_back(m_expressionStack.back());
            m_expressionStack.pop_back();
        }
    };

    m_endArgument = [&](int phase, LexicalTokens &lexicalVector, int index, bool stable){
        if(phase == BashClass::PHASE_EVAL) {

            // Check if the calls match the function definition: number and type of params
            _compareFunctionCallAndSignature(m_callableChainStack.back().back(), m_argumentListStack.back());

            // Remove argument list from the stack
            m_argumentListStack.pop_back();
        }
    };

    /**************************************
     *          EXPRESSION
     **************************************/

    m_startExpr = [&](int phase, LexicalTokens &lexicalVector, int index, bool stable){
        if(phase == BashClass::PHASE_EVAL) {
            m_expressionStack.push_back(std::make_shared<BExpression>());
        }
    };

    m_endExpr = [&](int phase, LexicalTokens &lexicalVector, int index, bool stable){
        if(phase == BashClass::PHASE_EVAL) {
            if(!m_expressionStack.back()->evaluate()) {
                std::cerr << "Expression at line " << lexicalVector[index]->getLine()
                << " did not evaluate successfully" << std::endl;
            }
        }
    };

    m_varAssign = [&](int phase, LexicalTokens &lexicalVector, int index, bool stable){
        if(phase == BashClass::PHASE_EVAL) {
            _checkAssignType(m_callableChainStack.back().back(), m_expressionStack.back());
            m_expressionStack.pop_back();
        }
    };

    m_returnExpr = [&](int phase, LexicalTokens &lexicalVector, int index, bool stable){
        if(phase == BashClass::PHASE_EVAL) {
            auto scopeFunction = std::dynamic_pointer_cast<BFunction>(m_scopeStack.back());
            _checkReturnValue(m_scopeStack.back(), m_expressionStack.back());
            scopeFunction->setHasReturn(true);
            m_expressionStack.pop_back();
        }
    };
}
