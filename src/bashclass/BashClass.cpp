#include <bashclass/BashClass.h>
#include <bashclass/BIf.h>
#include <bashclass/BWhile.h>
#include <iostream>
#include <bashclass/BException.h>
#include <bashclass/BBashHelper.h>
#include <bashclass/BReturn.h>
#include <bashclass/BThisAccess.h>
#include <bashclass/BVariableAssign.h>
#include <bashclass/BFunctionCall.h>

BashClass::BashClass() {
    initHandlers();
}

void BashClass::printStructure() {
    std::cout << BGlobal::getInstance()->getStructure().str() << std::endl;
}

void BashClass::onPhaseStartCheck() {
    if(!m_chainBuilderStack.empty()) {
        throw BException("Chain builder stack is not empty");
    }

    if(!m_expressionOperandStack.empty()) {
        throw BException("Expression operand stack is not empty");
    }

    if(!m_expressionOperatorStack.empty()) {
        throw BException("Expression operator stack is not empty");
    }

    if(!m_scopeStack.empty()) {
        throw BException("Scope stack size is not empty");
    }
}

void BashClass::initHandlers() {

    /**************************************
     *          PROGRAM
     **************************************/
    m_start = [&](int phase, LexicalTokens &lexicalVector, int index, bool stable){

        // Reset the reference key value
        m_referenceKey = 0;

        // Run checks
        onPhaseStartCheck();

        // Push global scope
        m_scopeStack.push_back(BGlobal::getInstance());

        // Connect elements after building the structure
        if(phase == BashClass::PHASE_EVAL) {

            // Link types of functions and variables
            BGlobal::getInstance()->linkTypes();

            // Detect circular references
            BGlobal::getInstance()->detectCircularReference();
        } else if(phase == BashClass::PHASE_GENERATE) {

            // Generate code required before any input
            BBashHelper::header();

            // Generate classes headers
            for(auto cls : BGlobal::getInstance()->findAllClasses()) {
                BBashHelper::declareClass(cls);
            }
        }
    };

    m_end = [&](int phase, LexicalTokens &lexicalVector, int index, bool stable){

        // Pop the global scope in all phases
        m_scopeStack.pop_back();

        if(phase == BashClass::PHASE_GENERATE) {

            // Generate code required after the input
            BBashHelper::footer();
        }
    };

    /**************************************
     *          REFERENCE KEY
     **************************************/
    m_newKey = [&](int phase, LexicalTokens &lexicalVector, int index, bool stable){

        // Update reference key
        ++m_referenceKey;
    };

    /**************************************
     *              BASH CODE
     **************************************/
    m_bashCode = [&](int phase, LexicalTokens &lexicalVector, int index, bool stable){
        if(phase == BashClass::PHASE_GENERATE) {
            BBashHelper::bash(lexicalVector[index]);
        }
    };

    /**************************************
     *          CLASS DECLARATION
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
            m_scopeStack.back()->registerClass(m_referenceKey, m_focusClass);

            // Push class scope
            m_scopeStack.push_back(m_focusClass);

            // Clear focus
            m_focusClass = nullptr;
        } else if(phase == BashClass::PHASE_EVAL || phase == BashClass::PHASE_GENERATE) {

            // Push class scope
            auto classScope = m_scopeStack.back()->getScopeByReferenceKey(m_referenceKey);
            m_scopeStack.push_back(classScope);
        }
    };

    m_endClass = [&](int phase, LexicalTokens &lexicalVector, int index, bool stable){
        if(phase == BashClass::PHASE_CREATE || phase == BashClass::PHASE_EVAL || phase == BashClass::PHASE_GENERATE) {
            m_scopeStack.pop_back();
        }
    };

    /**************************************
     *          FUNCTION DECLARATION
     **************************************/
    m_startFunction = [&](int phase, LexicalTokens &lexicalVector, int index, bool stable){
        if(phase == BashClass::PHASE_CREATE) {
            m_focusFunction = std::make_shared<BFunction>();
        } else if(phase == BashClass::PHASE_GENERATE) {
            auto functionScope = std::static_pointer_cast<BFunction>(
                    m_scopeStack.back()->getScopeByReferenceKey(m_referenceKey));
            BBashHelper::createFunction(functionScope);
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
            m_scopeStack.back()->registerFunction(m_referenceKey, m_focusFunction);

            // Push function scope
            m_scopeStack.push_back(m_focusFunction);

            // Clear focus
            m_focusFunction = nullptr;
        } else if(phase == BashClass::PHASE_EVAL || phase == BashClass::PHASE_GENERATE) {

            // Push function scope
            auto createdFunction = m_scopeStack.back()->getScopeByReferenceKey(m_referenceKey);
            m_scopeStack.push_back(createdFunction);
        }
    };

    m_endFunction = [&](int phase, LexicalTokens &lexicalVector, int index, bool stable){
        if(phase == BashClass::PHASE_CREATE) {
            m_scopeStack.pop_back();
        } else if(phase == BashClass::PHASE_EVAL) {
            auto functionScope = std::static_pointer_cast<BFunction>(m_scopeStack.back());
            functionScope->verifyReturns();
            m_scopeStack.pop_back();
        } else if(phase == BashClass::PHASE_GENERATE) {
            auto functionScope = std::static_pointer_cast<BFunction>(m_scopeStack.back());
            BBashHelper::closeFunction(functionScope);
            m_scopeStack.pop_back();
        }
    };

    /**************************************
     *          VARIABLE DECLARATION
     **************************************/
    m_startVar = [&](int phase, LexicalTokens &lexicalVector, int index, bool stable){
        if(phase == BashClass::PHASE_CREATE) {
            m_focusVariable = std::make_shared<BVariable>();
        } else if(phase == BashClass::PHASE_GENERATE) {

            // Generate code for the current variable
            auto variable = m_scopeStack.back()->getVariableByReferenceKey(m_referenceKey);
            if(m_scopeStack.back() == BGlobal::getInstance()) {
                BBashHelper::createGlobalVar(variable);
            } else if(!variable->isClassMember()) {
                BBashHelper::createLocalVar(variable);
            }

            // Don't generate code for class members
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
            m_scopeStack.back()->registerVariable(m_referenceKey, m_focusVariable);

            // Clear focus
            m_focusVariable = nullptr;
        }
    };

    m_endVar = [&](int phase, LexicalTokens &lexicalVector, int index, bool stable){
        // Do nothing ...
    };

    /**************************************
     *          PARAMETER DECLARATION
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
            m_scopeStack.back()->registerVariable(m_referenceKey, m_focusVariable);

            // Clear focus
            m_focusVariable = nullptr;
        }
    };

    m_endParam = [&](int phase, LexicalTokens &lexicalVector, int index, bool stable){
        // Do nothing ...
    };

    /**************************************
     *          WHILE STATEMENT
     **************************************/

    m_startWhile = [&](int phase, LexicalTokens &lexicalVector, int index, bool stable){
        if(phase == BashClass::PHASE_CREATE) {
            auto newWhile = std::make_shared<BWhile>();
            m_scopeStack.back()->registerScope(m_referenceKey, newWhile);
            m_scopeStack.push_back(newWhile);
        } else if(phase == BashClass::PHASE_EVAL || phase == BashClass::PHASE_GENERATE) {
            auto createdWhile = m_scopeStack.back()->getScopeByReferenceKey(m_referenceKey);
            m_scopeStack.push_back(createdWhile);
        }
    };

    m_whileCond = [&](int phase, LexicalTokens &lexicalVector, int index, bool stable){
        if(phase == BashClass::PHASE_EVAL) {
            auto whileScope = std::static_pointer_cast<BWhile>(m_scopeStack.back());
            whileScope->setExpression(m_expressionOperandStack.back());
            m_expressionOperandStack.pop_back();
        }
    };

    m_endWhile = [&](int phase, LexicalTokens &lexicalVector, int index, bool stable){
        if(phase == BashClass::PHASE_CREATE || phase == BashClass::PHASE_EVAL || phase == BashClass::PHASE_GENERATE) {
            m_scopeStack.pop_back();
        }
    };

    /**************************************
     *          IF STATEMENT
     **************************************/

    m_startIf = [&](int phase, LexicalTokens &lexicalVector, int index, bool stable){
        if(phase == BashClass::PHASE_CREATE) {
            auto newIf = std::make_shared<BIf>();
            m_scopeStack.back()->registerScope(m_referenceKey, newIf);
            m_scopeStack.push_back(newIf);
        } else if(phase == BashClass::PHASE_EVAL || phase == BashClass::PHASE_GENERATE) {
            auto createdIf = m_scopeStack.back()->getScopeByReferenceKey(m_referenceKey);
            m_scopeStack.push_back(createdIf);
        }
    };

    m_ifCond = [&](int phase, LexicalTokens &lexicalVector, int index, bool stable){
        if(phase == BashClass::PHASE_EVAL) {
            auto ifScope = std::static_pointer_cast<BIf>(m_scopeStack.back());
            ifScope->setExpression(m_expressionOperandStack.back());
            m_expressionOperandStack.pop_back();
        }
    };

    m_endIf = [&](int phase, LexicalTokens &lexicalVector, int index, bool stable){
        if(phase == BashClass::PHASE_CREATE || phase == BashClass::PHASE_EVAL || phase == BashClass::PHASE_GENERATE) {
            m_scopeStack.pop_back();
        }
    };

    /******************************************************
     *     VARIABLE CHAIN ACCESS AND FUNCTION CHAIN CALL
     ******************************************************/

    m_startInnerCall = [&](int phase, LexicalTokens &lexicalVector, int index, bool stable){
        if(phase == BashClass::PHASE_EVAL) {
            m_chainBuilderStack.push_back(std::make_shared<BChain>());
        }
    };

    m_endInnerCall = [&](int phase, LexicalTokens &lexicalVector, int index, bool stable){
        if(phase == BashClass::PHASE_EVAL) {
            m_chainBuilderStack.pop_back();
        }
    };

    m_varChainAccess = [&](int phase, LexicalTokens &lexicalVector, int index, bool stable){
        if(phase == BashClass::PHASE_EVAL) {
            m_chainBuilderStack.back()->addVariable(m_scopeStack.back(), lexicalVector[index]);
        }
    };

    m_functionChainCall = [&](int phase, LexicalTokens &lexicalVector, int index, bool stable){
        if(phase == BashClass::PHASE_EVAL) {
            m_chainBuilderStack.back()->addFunction(m_scopeStack.back(), lexicalVector[index]);
        }
    };

    m_thisChainAccess = [&](int phase, LexicalTokens &lexicalVector, int index, bool stable){
        if(phase == BashClass::PHASE_EVAL) {
            auto thisCall = std::make_shared<BThisChainAccess>();
            thisCall->setLexicalToken(lexicalVector[index]);
            m_chainBuilderStack.back()->addThis(m_scopeStack.back(), thisCall);
        }
    };

    /***********************************************************
     *     VARIABLE ASSIGN, VARIABLE ACCESS AND FUNCTION CALL
     ***********************************************************/

    m_startOuterCall = [&](int phase, LexicalTokens &lexicalVector, int index, bool stable){
        if(phase == BashClass::PHASE_EVAL) {
            m_chainBuilderStack.push_back(std::make_shared<BChain>());
        }
    };

    m_endOuterCall = [&](int phase, LexicalTokens &lexicalVector, int index, bool stable){
        if(phase == BashClass::PHASE_EVAL) {
            m_chainBuilderStack.pop_back();
        }
    };

    m_tokenUse = [&](int phase, LexicalTokens &lexicalVector, int index, bool stable){
        if(phase == BashClass::PHASE_EVAL) {
            auto token = std::make_shared<BTokenUse>();
            token->setLexicalToken(lexicalVector[index]);
            m_expressionOperandStack.push_back(token);
        }
    };

    m_thisAccess = [&](int phase, LexicalTokens &lexicalVector, int index, bool stable){
        if(phase == BashClass::PHASE_EVAL) {
            auto thisChainAccess = std::make_shared<BThisChainAccess>();
            thisChainAccess->setLexicalToken(lexicalVector[index]);
            auto thisAccess = std::make_shared<BThisAccess>();
            thisAccess->setThisChainAccess(thisChainAccess);
            m_expressionOperandStack.push_back(thisAccess);
        }
    };

    m_varAssign = [&](int phase, LexicalTokens &lexicalVector, int index, bool stable){
        if(phase == BashClass::PHASE_EVAL) {

            // Configure variable assignment
            auto variableAccess = std::make_shared<BVariableAccess>();
            variableAccess->setChain(m_chainBuilderStack.back());
            auto variableAssign = std::make_shared<BVariableAssign>();
            variableAssign->setVariableAccess(variableAccess);
            variableAssign->setExpression(m_expressionOperandStack.back());

            // Pop consumed expression
            m_expressionOperandStack.pop_back();

            // Register variable assignment
            m_scopeStack.back()->registerVariableAssign(m_referenceKey, variableAssign);
        } else if(phase == BashClass::PHASE_GENERATE) {

            // Generate assign statement
            BBashHelper::assignVariable(m_scopeStack.back()->getVariableAssignByReferenceKey(m_referenceKey));
        }
    };

    m_varAccess = [&](int phase, LexicalTokens &lexicalVector, int index, bool stable){
        if(phase == BashClass::PHASE_EVAL) {
            auto variableAccess = std::make_shared<BVariableAccess>();
            variableAccess->setChain(m_chainBuilderStack.back());
            m_expressionOperandStack.push_back(variableAccess);
        }
    };

    m_functionCall = [&](int phase, LexicalTokens &lexicalVector, int index, bool stable){
        if(phase == BashClass::PHASE_EVAL) {
            auto functionCall = std::make_shared<BFunctionCall>();
            functionCall->setChain(m_chainBuilderStack.back());

            // Push as operand
            m_expressionOperandStack.push_back(functionCall);
        }
    };

    m_functionExec = [&](int phase, LexicalTokens &lexicalVector, int index, bool stable){
        if(phase == BashClass::PHASE_EVAL) {
            auto functionCall = std::make_shared<BFunctionCall>();
            functionCall->setChain(m_chainBuilderStack.back());

            // Register function call
            m_scopeStack.back()->registerFunctionCall(m_referenceKey, functionCall);
        } else if(phase == BashClass::PHASE_GENERATE) {

            // Generate function execution statement
            BBashHelper::functionExec(m_scopeStack.back()->getFunctionCallByReferenceKey(m_referenceKey));
        }
    };

    /**************************************
     *          RETURN STATEMENT
     **************************************/

    m_startReturn = [&](int phase, LexicalTokens &lexicalVector, int index, bool stable) {
        // Do nothing ...
    };

    m_returnExpr = [&](int phase, LexicalTokens &lexicalVector, int index, bool stable){
        if(phase == BashClass::PHASE_EVAL) {

            // Create and configure return statement
            auto returnComp = std::make_shared<BReturn>();
            returnComp->setExpression(m_expressionOperandStack.back());

            // Register return statement
            m_scopeStack.back()->setReturn(returnComp);

            // Remove consumed expression
            m_expressionOperandStack.pop_back();
        }
    };

    m_endReturn = [&](int phase, LexicalTokens &lexicalVector, int index, bool stable){
        // Do nothing ...
    };

    /**************************************
     *          ARGUMENT PASS
     **************************************/

    m_startArgument = [&](int phase, LexicalTokens &lexicalVector, int index, bool stable){
        // Do nothing ...
    };

    m_setArgument = [&](int phase, LexicalTokens &lexicalVector, int index, bool stable){
        if(phase == BashClass::PHASE_EVAL) {

            // Get function that is currently being built
            auto functionCall = std::static_pointer_cast<BFunctionChainCall>(m_chainBuilderStack.back()->last());

            // Add the argument to it
            functionCall->addArgument(m_expressionOperandStack.back());

            // Remove consumed expression
            m_expressionOperandStack.pop_back();
        }
    };

    m_endArgument = [&](int phase, LexicalTokens &lexicalVector, int index, bool stable){
        if(phase == BashClass::PHASE_EVAL) {

            // Get function call
            auto functionCall = std::static_pointer_cast<BFunctionChainCall>(m_chainBuilderStack.back()->last());

            // Verify provided arguments
            functionCall->verifyArguments();
        }
    };

    /**************************************
     *          EXPRESSION
     **************************************/

    m_startExpr = [&](int phase, LexicalTokens &lexicalVector, int index, bool stable){
        // Do nothing ...
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
            auto expression = std::make_shared<BArithOperation>();
            expression->setRightOperand(rightOperand);
            expression->setLeftOperand(leftOperand);
            expression->setOperator(operatorToken);

            // Push expression again as an operand
            m_expressionOperandStack.push_back(expression);
        }
    };

    m_putOp = [&](int phase, LexicalTokens &lexicalVector, int index, bool stable){
        if(phase == BashClass::PHASE_EVAL) {
            m_expressionOperatorStack.push_back(lexicalVector[index]);
        }
    };

    m_endExpr = [&](int phase, LexicalTokens &lexicalVector, int index, bool stable){
        // Do nothing ...
    };
}
