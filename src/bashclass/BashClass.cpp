#include <bashclass/BashClass.h>
#include <bashclass/BTypes.h>
#include <bashclass/BClass.h>
#include <bashclass/BIf.h>
#include <bashclass/BWhile.h>
#include <iostream>

BashClass::BashClass() {
    m_global = std::make_shared<BGlobal>();
    initHandlers();
}

void BashClass::printStructure() {
    std::cout << m_global->getStructure().str() << std::endl;
}

void BashClass::onPhaseStartCheck() {
    if(!m_chainBuilderStack.empty()) {
        throw std::runtime_error("Chain builder stack is not empty. Please report this error.");
    }

    if(!m_expressionOperandStack.empty()) {
        throw std::runtime_error("Expression operand stack is not empty. Please report this error.");
    }

    if(!m_expressionOperatorStack.empty()) {
        throw std::runtime_error("Expression operator stack is not empty. Please report this error.");
    }

    if(!m_scopeStack.empty()) {
        throw std::runtime_error("Scope stack size is not empty. Please report this error.");
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
        if(phase == BashClass::PHASE_CREATE) {

            // Push global scope
            m_scopeStack.push_back(m_global);
        } else if(phase == BashClass::PHASE_EVAL) {

            // Run checks
            onPhaseStartCheck();

            // Push global scope
            m_scopeStack.push_back(m_global);

            // Link types of functions and variables
            m_global->linkTypes();

            // Detect circular references
            m_global->detectCircularReference();
        } else if(phase == BashClass::PHASE_GENERATE) {

            // Run checks
            onPhaseStartCheck();

            // Push global scope
            m_scopeStack.push_back(m_global);
        }
    };

    m_end = [&](int phase, LexicalTokens &lexicalVector, int index, bool stable){

        // Pop the global scope in all phases
        m_scopeStack.pop_back();
    };

    /**************************************
     *          CLASSES
     **************************************/
    m_startClass = [&](int phase, LexicalTokens &lexicalVector, int index, bool stable){
        if(phase == BashClass::PHASE_CREATE) {
            m_focusClass = std::make_shared<BClass>();
        }
    };

    m_className = [&](int phase, LexicalTokens &lexicalVector, int index, bool stable){
        if(phase == BashClass::PHASE_CREATE) {

            // Set class name
            m_focusClass->setName(lexicalVector[index]);

            // Register class
            m_scopeStack.back()->registerClass(lexicalVector[index], m_focusClass);

            // Push class scope
            m_scopeStack.push_back(m_focusClass);

            // Clear focus
            m_focusClass = nullptr;
        } else if(phase == BashClass::PHASE_EVAL) {

            // Push class scope
            auto classScope = m_scopeStack.back()->getScopeByToken(lexicalVector[index]);
            m_scopeStack.push_back(classScope);
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
            m_focusFunction = std::make_shared<BFunction>();
        }
    };

    m_functionType = [&](int phase, LexicalTokens &lexicalVector, int index, bool stable){
        if(phase == BashClass::PHASE_CREATE) {
            m_focusFunction->setType(lexicalVector[index]);
        }
    };

    m_functionName = [&](int phase, LexicalTokens &lexicalVector, int index, bool stable){
        if(phase == BashClass::PHASE_CREATE) {

            // Set function name
            m_focusFunction->setName(lexicalVector[index]);

            // Register function
            m_scopeStack.back()->registerFunction(lexicalVector[index], m_focusFunction);

            // Push function scope
            m_scopeStack.push_back(m_focusFunction);

            // Clear focus
            m_focusFunction = nullptr;
        } else if(phase == BashClass::PHASE_EVAL) {

            // Push function scope
            auto createdFunction = m_scopeStack.back()->getScopeByToken(lexicalVector[index]);
            m_scopeStack.push_back(createdFunction);
        }
    };

    m_endFunction = [&](int phase, LexicalTokens &lexicalVector, int index, bool stable){
        if(phase == BashClass::PHASE_CREATE) {
            m_scopeStack.pop_back();
        } else if(phase == BashClass::PHASE_EVAL) {
            auto functionScope = std::dynamic_pointer_cast<BFunction>(m_scopeStack.back());
            functionScope->verifyParameters();
            functionScope->verifyReturns();
            m_scopeStack.pop_back();
        }
    };

    /**************************************
     *          VARIABLES DECLARATION
     **************************************/
    m_startVar = [&](int phase, LexicalTokens &lexicalVector, int index, bool stable){
        if(phase == BashClass::PHASE_CREATE) {
            m_focusVariable = std::make_shared<BVariable>();
        }
    };

    m_varType = [&](int phase, LexicalTokens &lexicalVector, int index, bool stable){
        if(phase == BashClass::PHASE_CREATE) {
            m_focusVariable->setType(lexicalVector[index]);
        }
    };

    m_varName = [&](int phase, LexicalTokens &lexicalVector, int index, bool stable){
        if(phase == BashClass::PHASE_CREATE) {

            // Set variable name
            m_focusVariable->setName(lexicalVector[index]);

            // Register variable
            m_scopeStack.back()->registerVariable(lexicalVector[index], m_focusVariable);

            // Clear focus
            m_focusVariable = nullptr;
        }
    };

    m_endVar = [&](int phase, LexicalTokens &lexicalVector, int index, bool stable){
        // Do nothing ...
    };

    /**************************************
     *          PARAMETERS
     **************************************/
    m_startParam = [&](int phase, LexicalTokens &lexicalVector, int index, bool stable){
        if(phase == BashClass::PHASE_CREATE) {
            m_focusVariable = std::make_shared<BVariable>();
            m_focusVariable->setIsParam(true);
        }
    };

    m_paramType = [&](int phase, LexicalTokens &lexicalVector, int index, bool stable){
        if(phase == BashClass::PHASE_CREATE) {
            m_focusVariable->setType(lexicalVector[index]);
        }
    };

    m_paramName = [&](int phase, LexicalTokens &lexicalVector, int index, bool stable){
        if(phase == BashClass::PHASE_CREATE) {

            // Set parameter name
            m_focusVariable->setName(lexicalVector[index]);

            // Register parameter
            m_scopeStack.back()->registerVariable(lexicalVector[index], m_focusVariable);

            // Clear focus
            m_focusVariable = nullptr;
        }
    };

    m_endParam = [&](int phase, LexicalTokens &lexicalVector, int index, bool stable){
        // Do nothing ...
    };

    /**************************************
     *          WHILE
     **************************************/

    m_startWhile = [&](int phase, LexicalTokens &lexicalVector, int index, bool stable){
        if(phase == BashClass::PHASE_CREATE) {
            auto newWhile = std::make_shared<BWhile>();
            m_scopeStack.back()->registerScope(lexicalVector[index], newWhile);
            m_scopeStack.push_back(newWhile);
        } else if(phase == BashClass::PHASE_EVAL) {
            auto createdWhile = m_scopeStack.back()->getScopeByToken(lexicalVector[index]);
            m_scopeStack.push_back(createdWhile);
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
            whileScope->setExpression(m_expressionOperandStack.back());
            m_expressionOperandStack.pop_back();
        }
    };

    /**************************************
     *          IF
     **************************************/

    m_startIf = [&](int phase, LexicalTokens &lexicalVector, int index, bool stable){
        if(phase == BashClass::PHASE_CREATE) {
            auto newIf = std::make_shared<BIf>();
            m_scopeStack.back()->registerScope(lexicalVector[index], newIf);
            m_scopeStack.push_back(newIf);
        } else if(phase == BashClass::PHASE_EVAL) {
            auto createdIf = m_scopeStack.back()->getScopeByToken(lexicalVector[index]);
            m_scopeStack.push_back(createdIf);
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
            ifScope->setExpression(m_expressionOperandStack.back());
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
            m_chainBuilderStack.pop_back();
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
     *          RETURN
     **************************************/

    m_startReturn = [&](int phase, LexicalTokens &lexicalVector, int index, bool stable) {
        if(phase == BashClass::PHASE_EVAL) {
            m_focusReturnToken = lexicalVector[index];
        }
    };

    m_returnExpr = [&](int phase, LexicalTokens &lexicalVector, int index, bool stable){
        if(phase == BashClass::PHASE_EVAL) {

            // Get current function scope
            auto functionScope = std::dynamic_pointer_cast<BFunction>(m_scopeStack.back());

            // Create and configure return statement
            auto returnComp = std::make_shared<BReturn>();
            returnComp->setExpression(m_expressionOperandStack.back());

            // Register return statement
            functionScope->registerReturn(m_focusReturnToken, returnComp);

            // Remove consumed expression
            m_expressionOperandStack.pop_back();

            // Clear focus
            m_focusReturnToken = nullptr;
        }
    };

    m_endReturn = [&](int phase, LexicalTokens &lexicalVector, int index, bool stable){
        // Do nothing ...
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

            // Get function call
            auto functionCall = std::dynamic_pointer_cast<BFunctionCall>(m_chainBuilderStack.back()->last());

            // Verify provided arguments
            functionCall->verifyArguments();
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

            // Configure variable call
            auto variableCall = std::dynamic_pointer_cast<BVariableCall>(m_chainBuilderStack.back()->last());
            variableCall->setExpression(m_expressionOperandStack.back());

            // Pop consumed expression
            m_expressionOperandStack.pop_back();

            // Register variable chain call
            m_scopeStack.back()->registerChainCall(lexicalVector[index], m_chainBuilderStack.back());
        }
    };

    m_functionExec = [&](int phase, LexicalTokens &lexicalVector, int index, bool stable){
        if(phase == BashClass::PHASE_EVAL) {
            // Register function chain call
            m_scopeStack.back()->registerChainCall(lexicalVector[index], m_chainBuilderStack.back());
        }
    };
}
