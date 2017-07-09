#include <bashclass/BashClass.h>
#include <bashclass/BWhile.h>
#include <iostream>
#include <bashclass/BException.h>
#include <bashclass/BBashHelper.h>
#include <bashclass/BThisAccess.h>
#include <bashclass/BVariableAccess.h>
#include <bashclass/BGenerateCode.h>
#include <bashclass/BArrayUse.h>
#include <bashclass/BReport.h>

void BashClass::openOutput() {
    // Open output file
    BGenerateCode::get().openFile(m_outputFile);

    // Generate code required before any input
    BBashHelper::header();

    // Generate code for the program array
    BBashHelper::declareCounters();

    // Generate code for required bash functions
    BBashHelper::writeBashFunctions();
}

void BashClass::closeOutput() {
    // Generate code required after the input
    BBashHelper::footer();

    // Close output file
    BGenerateCode::get().closeFile();
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

int BashClass::compile(std::vector<std::string> inputFiles, std::string outputFile) {

    // Initialize semantic action handlers
    initHandlers();

    // Set output file
    m_outputFile = outputFile;

    // Configure easycc on syntax error
    m_easyCC->setOnSyntaxError([&](){
        m_easyCC->setSilentSemanticEvents(true);
    });

    // Define the compiler phases
    std::vector<int> phases = {
            BashClass::PHASE_CREATE,
            BashClass::PHASE_EVAL,
            BashClass::PHASE_GENERATE
    };

    // Open file
    openOutput();

    // Start compiling
    for(int phase : phases) {

        // Don't generate code if semantic errors were reported
        if(phase == BashClass::PHASE_GENERATE) {
            if(BReport::getInstance().hasError()) {
                return BashClass::ERR_CODE_SEMANTIC;
            }
        }
        
        // Reset the reference key value
        m_referenceKey = 0;

        // Set the phase number
        m_easyCC->setParsingPhase(phase);

        // Show error message on create phase only
        m_easyCC->setSilentSyntaxErrorMessages(phase != BashClass::PHASE_CREATE);

        // Compile all files passed as arguments
        for(std::string fileName : inputFiles) {

            // Set file name being processed
            BReport::getInstance().setFileName(fileName);

            // Compile file
            int code = m_easyCC->compile(fileName);

            // Store compiling if a file has syntax errors
            if(code != ecc::IEasyCC::OK_CODE) {
                return code;
            }
        }

        if(phase == BashClass::PHASE_EVAL) {

            // Link types of functions and variables
            BGlobal::getInstance()->linkTypes();

            // Verify main function
            BGlobal::getInstance()->verifyMain();
        }
    }

    // Close file
    closeOutput();
    return 0;
}

void BashClass::initHandlers() {

    /**************************************
     *          PROGRAM
     **************************************/
    m_easyCC->registerSemanticAction("#start#", [&](int phase, LexicalTokens &lexicalVector, int index){

        // Run checks
        onPhaseStartCheck();

        // Push global scope
        m_scopeStack.push_back(BGlobal::getInstance());
    });

    m_easyCC->registerSemanticAction("#end#",[&](int phase, LexicalTokens &lexicalVector, int index){

        // Pop the global scope in all phases
        m_scopeStack.pop_back();
    });

    /**************************************
     *          REFERENCE KEY
     **************************************/
    m_easyCC->registerSemanticAction("#newKey#",[&](int phase, LexicalTokens &lexicalVector, int index){

        // Update reference key
        ++m_referenceKey;
    });

    /**************************************
     *              BASH CODE
     **************************************/
    m_easyCC->registerSemanticAction("#bashCode#",[&](int phase, LexicalTokens &lexicalVector, int index){
        if(phase == BashClass::PHASE_GENERATE) {
            BBashHelper::bash(m_scopeStack.back(), lexicalVector[index]);
        }
    });

    /**************************************
     *          CLASS DECLARATION
     **************************************/
    m_easyCC->registerSemanticAction("#startClass#",[&](int phase, LexicalTokens &lexicalVector, int index){
        if(phase == BashClass::PHASE_CREATE) {
            m_focusClass = std::make_shared<BClass>();
        }
    });

    m_easyCC->registerSemanticAction("#className#",[&](int phase, LexicalTokens &lexicalVector, int index){
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
    });

    m_easyCC->registerSemanticAction("#classExtends#",[&](int phase, LexicalTokens &lexicalVector, int index){
        if(phase == BashClass::PHASE_EVAL) {
            auto cls = std::static_pointer_cast<BClass>(m_scopeStack.back());
            cls->setExtends(lexicalVector[index]);
        }
    });

    m_easyCC->registerSemanticAction("#endClass#",[&](int phase, LexicalTokens &lexicalVector, int index){
        if(phase == BashClass::PHASE_CREATE || phase == BashClass::PHASE_EVAL || phase == BashClass::PHASE_GENERATE) {
            m_scopeStack.pop_back();
        }
    });

    /**************************************
     *          FUNCTION DECLARATION
     **************************************/
    m_easyCC->registerSemanticAction("#startFunction#",[&](int phase, LexicalTokens &lexicalVector, int index){
        if(phase == BashClass::PHASE_CREATE) {
            m_focusFunction = std::make_shared<BFunction>();
        } else if(phase == BashClass::PHASE_GENERATE) {
            auto functionScope = std::static_pointer_cast<BFunction>(
                    m_scopeStack.back()->getScopeByReferenceKey(m_referenceKey));
            BBashHelper::createFunction(functionScope);
        }
    });

    m_easyCC->registerSemanticAction("#functionType#",[&](int phase, LexicalTokens &lexicalVector, int index){
        if(phase == BashClass::PHASE_CREATE) {
            m_focusFunction->getType()->setLexicalToken(lexicalVector[index]);
        }
    });

    m_easyCC->registerSemanticAction("#functionConstructor#",[&](int phase, LexicalTokens &lexicalVector, int index){
        if(phase == BashClass::PHASE_CREATE) {
            m_focusFunction->setIsConstructor(true);
        }
    });

    m_easyCC->registerSemanticAction("#functionName#",[&](int phase, LexicalTokens &lexicalVector, int index){
        if(phase == BashClass::PHASE_CREATE) {

            // Set function name
            m_focusFunction->setName(lexicalVector[index]);

            // Register function
            m_scopeStack.back()->registerFunction(m_referenceKey, m_focusFunction);

            // Push function scope
            m_scopeStack.push_back(m_focusFunction);

            // Clear focus
            m_focusFunction = nullptr;
        } else if(phase == BashClass::PHASE_EVAL) {

            // Push function scope
            auto createdFunction =
                    std::static_pointer_cast<BFunction>(m_scopeStack.back()->getScopeByReferenceKey(m_referenceKey));
            m_scopeStack.push_back(createdFunction);

            // Verify function override
            createdFunction->verifyOverride();

        } else if(phase == BashClass::PHASE_GENERATE) {

            // Push function scope
            auto createdFunction = m_scopeStack.back()->getScopeByReferenceKey(m_referenceKey);
            m_scopeStack.push_back(createdFunction);
        }
    });

    m_easyCC->registerSemanticAction("#endFunction#",[&](int phase, LexicalTokens &lexicalVector, int index){
        if(phase == BashClass::PHASE_CREATE) {
            m_scopeStack.pop_back();
        } else if(phase == BashClass::PHASE_EVAL) {
            auto functionScope = std::static_pointer_cast<BFunction>(m_scopeStack.back());

            // Verify function return
            functionScope->verifyReturns();

            // Verify function parameters
            functionScope->verifyParameters();
            m_scopeStack.pop_back();
        } else if(phase == BashClass::PHASE_GENERATE) {
            auto functionScope = std::static_pointer_cast<BFunction>(m_scopeStack.back());
            BBashHelper::closeFunction(functionScope);
            m_scopeStack.pop_back();
        }
    });

    /**************************************
     *     CLASS VARIABLE DECLARATION
     **************************************/
    m_easyCC->registerSemanticAction("#startClassVar#",[&](int phase, LexicalTokens &lexicalVector, int index){
        if(phase == BashClass::PHASE_CREATE) {
            m_focusVariable = std::make_shared<BVariable>();
        }
        // Don't generate code for class members
    });

    m_easyCC->registerSemanticAction("#classVarType#",[&](int phase, LexicalTokens &lexicalVector, int index){
        if(phase == BashClass::PHASE_CREATE) {
            m_focusVariable->getType()->setLexicalToken(lexicalVector[index]);
        }
    });

    m_easyCC->registerSemanticAction("#classVarName#",[&](int phase, LexicalTokens &lexicalVector, int index){
        if(phase == BashClass::PHASE_CREATE) {

            // Set variable name
            m_focusVariable->setName(lexicalVector[index]);

            // Register variable
            m_scopeStack.back()->registerVariable(m_referenceKey, m_focusVariable);

            // Clear focus
            m_focusVariable = nullptr;
        } else if(phase == BashClass::PHASE_EVAL) {

            // Used by another handler
            m_focusVariable = m_scopeStack.back()->getVariableByReferenceKey(m_referenceKey);
        }
    });

    m_easyCC->registerSemanticAction("#endClassVar#",[&](int phase, LexicalTokens &lexicalVector, int index){
        if(phase == BashClass::PHASE_EVAL) {
            // Clear focus
            m_focusVariable = nullptr;
        }
    });

    /**************************************
     *          VARIABLE DECLARATION
     **************************************/
    m_easyCC->registerSemanticAction("#startVar#",[&](int phase, LexicalTokens &lexicalVector, int index){
        if(phase == BashClass::PHASE_EVAL) {
            m_focusVariable = std::make_shared<BVariable>();
        } else if(phase == BashClass::PHASE_GENERATE) {

            // Generate code for the current variable
            BBashHelper::createVar(m_scopeStack.back()->getVariableByReferenceKey(m_referenceKey));
        }
    });

    m_easyCC->registerSemanticAction("#varType#",[&](int phase, LexicalTokens &lexicalVector, int index){
        if(phase == BashClass::PHASE_EVAL) {
            m_focusVariable->getType()->setLexicalToken(lexicalVector[index]);
        }
    });

    m_easyCC->registerSemanticAction("#varName#",[&](int phase, LexicalTokens &lexicalVector, int index){
        if(phase == BashClass::PHASE_EVAL) {
            // Set variable name
            m_focusVariable->setName(lexicalVector[index]);

            // Register variable
            m_scopeStack.back()->registerVariable(m_referenceKey, m_focusVariable);

            // Link variable
            m_focusVariable->getType()->linkType();
        }
    });

    m_easyCC->registerSemanticAction("#endVar#",[&](int phase, LexicalTokens &lexicalVector, int index){
        if(phase == BashClass::PHASE_EVAL) {
            // Clear focus
            m_focusVariable = nullptr;
        }
    });

    /**************************************
     *          PARAMETER DECLARATION
     **************************************/
    m_easyCC->registerSemanticAction("#startParam#",[&](int phase, LexicalTokens &lexicalVector, int index){
        if(phase == BashClass::PHASE_EVAL) {
            m_focusVariable = std::make_shared<BVariable>();
            m_focusVariable->setIsParam(true);
        }
    });

    m_easyCC->registerSemanticAction("#paramType#",[&](int phase, LexicalTokens &lexicalVector, int index){
        if(phase == BashClass::PHASE_EVAL) {
            m_focusVariable->getType()->setLexicalToken(lexicalVector[index]);
        }
    });

    m_easyCC->registerSemanticAction("#paramName#",[&](int phase, LexicalTokens &lexicalVector, int index){
        if(phase == BashClass::PHASE_EVAL) {

            // Set parameter name
            m_focusVariable->setName(lexicalVector[index]);

            // Register parameter
            m_scopeStack.back()->registerVariable(m_referenceKey, m_focusVariable);

            // Link variable
            m_focusVariable->getType()->linkType();
        }
    });

    m_easyCC->registerSemanticAction("#endParam#",[&](int phase, LexicalTokens &lexicalVector, int index){
        if(phase == BashClass::PHASE_EVAL) {
            // Clear focus
            m_focusVariable = nullptr;
        }
    });

    /**************************************
     *           FOR STATEMENT
     **************************************/
    m_easyCC->registerSemanticAction("#startFor#",[&](int phase, LexicalTokens &lexicalVector, int index){
        if(phase == BashClass::PHASE_CREATE) {
            auto newFor = std::make_shared<BFor>();
            m_scopeStack.back()->registerScope(m_referenceKey, newFor);
            m_scopeStack.push_back(newFor);
        } else if(phase == BashClass::PHASE_EVAL) {
            m_scopeStack.push_back(m_scopeStack.back()->getScopeByReferenceKey(m_referenceKey));
        } else if(phase == BashClass::PHASE_GENERATE) {
            auto createdFor = std::static_pointer_cast<BFor>(
                    m_scopeStack.back()->getScopeByReferenceKey(m_referenceKey));
            m_scopeStack.push_back(createdFor);
            BBashHelper::createFor(createdFor);
        }
    });

    m_easyCC->registerSemanticAction("#forPreCond#",[&](int phase, LexicalTokens &lexicalVector, int index){
        if(phase == BashClass::PHASE_EVAL) {

            // Set pre condition
            auto createdFor = std::static_pointer_cast<BFor>(m_scopeStack.back());
            createdFor->setPreCondition(m_expressionOperandStack.back());

            // Remove consumed expression
            m_expressionOperandStack.pop_back();
        }
    });

    m_easyCC->registerSemanticAction("#forCond#",[&](int phase, LexicalTokens &lexicalVector, int index){
        if(phase == BashClass::PHASE_EVAL) {
            // Set condition
            auto createdFor = std::static_pointer_cast<BFor>(m_scopeStack.back());
            createdFor->setCondition(m_expressionOperandStack.back());

            // Remove consumed expression
            m_expressionOperandStack.pop_back();
        }
    });

    m_easyCC->registerSemanticAction("#forPostCond#",[&](int phase, LexicalTokens &lexicalVector, int index){
        if(phase == BashClass::PHASE_EVAL) {
            // Set post condition
            auto createdFor = std::static_pointer_cast<BFor>(m_scopeStack.back());
            createdFor->setPostCondition(m_expressionOperandStack.back());

            // Remove consumed expression
            m_expressionOperandStack.pop_back();
        }
    });

    m_easyCC->registerSemanticAction("#endFor#",[&](int phase, LexicalTokens &lexicalVector, int index){
        if(phase == BashClass::PHASE_CREATE || phase == BashClass::PHASE_EVAL) {
            m_scopeStack.pop_back();
        } else if(phase == BashClass::PHASE_GENERATE) {
            BBashHelper::closeFor(std::static_pointer_cast<BFor>(m_scopeStack.back()));
            m_scopeStack.pop_back();
        }
    });


    /**************************************
     *          WHILE STATEMENT
     **************************************/
    m_easyCC->registerSemanticAction("#startWhile#",[&](int phase, LexicalTokens &lexicalVector, int index){
        if(phase == BashClass::PHASE_CREATE) {
            auto newWhile = std::make_shared<BWhile>();
            m_scopeStack.back()->registerScope(m_referenceKey, newWhile);
            m_scopeStack.push_back(newWhile);
        } else if(phase == BashClass::PHASE_EVAL) {
            m_scopeStack.push_back(m_scopeStack.back()->getScopeByReferenceKey(m_referenceKey));
        } else if( phase == BashClass::PHASE_GENERATE) {
            auto createdWhile = std::static_pointer_cast<BWhile>(
                    m_scopeStack.back()->getScopeByReferenceKey(m_referenceKey));
            m_scopeStack.push_back(createdWhile);
            BBashHelper::createWhile(createdWhile);
        }
    });

    m_easyCC->registerSemanticAction("#whileCond#",[&](int phase, LexicalTokens &lexicalVector, int index){
        if(phase == BashClass::PHASE_EVAL) {
            auto whileScope = std::static_pointer_cast<BWhile>(m_scopeStack.back());
            whileScope->setExpression(m_expressionOperandStack.back());
            m_expressionOperandStack.pop_back();
        }
    });

    m_easyCC->registerSemanticAction("#endWhile#",[&](int phase, LexicalTokens &lexicalVector, int index){
        if(phase == BashClass::PHASE_CREATE || phase == BashClass::PHASE_EVAL) {
            m_scopeStack.pop_back();
        } else if(phase == BashClass::PHASE_GENERATE) {
            BBashHelper::closeWhile(std::static_pointer_cast<BWhile>(m_scopeStack.back()));
            m_scopeStack.pop_back();
        }
    });

    /**************************************
     *          IF STATEMENT
     **************************************/
    m_easyCC->registerSemanticAction("#startIf#",[&](int phase, LexicalTokens &lexicalVector, int index){
        if(phase == BashClass::PHASE_CREATE) {
            auto newIf = std::make_shared<BIf>();
            m_scopeStack.back()->registerScope(m_referenceKey, newIf);
            m_scopeStack.push_back(newIf);

        } else if(phase == BashClass::PHASE_EVAL) {
            auto createdIf = m_scopeStack.back()->getScopeByReferenceKey(m_referenceKey);
            m_scopeStack.push_back(createdIf);

        } else if(phase == BashClass::PHASE_GENERATE) {
            auto createdIf = std::static_pointer_cast<BIf>(m_scopeStack.back()->getScopeByReferenceKey(m_referenceKey));
            m_scopeStack.push_back(createdIf);
            BBashHelper::createIf(createdIf);
        }
    });

    m_easyCC->registerSemanticAction("#ifCond#",[&](int phase, LexicalTokens &lexicalVector, int index){
        if(phase == BashClass::PHASE_EVAL) {
            auto ifScope = std::static_pointer_cast<BIf>(m_scopeStack.back());
            ifScope->setExpression(m_expressionOperandStack.back());
            m_expressionOperandStack.pop_back();
        }
    });

    m_easyCC->registerSemanticAction("#endIf#",[&](int phase, LexicalTokens &lexicalVector, int index){
        if(phase == BashClass::PHASE_CREATE) {
            m_focusIf = std::static_pointer_cast<BIf>(m_scopeStack.back());
            m_scopeStack.pop_back();
        } else if(phase == BashClass::PHASE_EVAL) {
            m_scopeStack.pop_back();
        } else if(phase == BashClass::PHASE_GENERATE) {
            auto ifScope = std::static_pointer_cast<BIf>(m_scopeStack.back());
            BBashHelper::closeIf(ifScope);
            m_scopeStack.pop_back();
        }
    });

    /**************************************
     *          ELIF STATEMENT
     **************************************/
    m_easyCC->registerSemanticAction("#startElif#",[&](int phase, LexicalTokens &lexicalVector, int index){
        if(phase == BashClass::PHASE_CREATE) {
            auto newElif = std::make_shared<BElif>();
            m_scopeStack.back()->registerScope(m_referenceKey, newElif);
            m_scopeStack.push_back(newElif);

            // Link elif to the if scope
            m_focusIf->addElif(newElif);

            // Clear focus
            m_focusIf = nullptr;
        } else if(phase == BashClass::PHASE_EVAL) {
            auto createdElif = m_scopeStack.back()->getScopeByReferenceKey(m_referenceKey);
            m_scopeStack.push_back(createdElif);

        } else if(phase == BashClass::PHASE_GENERATE) {
            auto createdElif = std::static_pointer_cast<BElif>(m_scopeStack.back()->getScopeByReferenceKey(m_referenceKey));
            m_scopeStack.push_back(createdElif);
            BBashHelper::createElif(createdElif);
        }
    });

    m_easyCC->registerSemanticAction("#elifCond#",[&](int phase, LexicalTokens &lexicalVector, int index){
        if(phase == BashClass::PHASE_EVAL) {
            auto elifScope = std::static_pointer_cast<BElif>(m_scopeStack.back());
            elifScope->setExpression(m_expressionOperandStack.back());
            m_expressionOperandStack.pop_back();
        }
    });

    m_easyCC->registerSemanticAction("#endElif#",[&](int phase, LexicalTokens &lexicalVector, int index){
        if(phase == BashClass::PHASE_CREATE) {
            // Update focus since a nested if statement might have updated it
            m_focusIf = std::static_pointer_cast<BElif>(m_scopeStack.back())->getParentIf();

            m_scopeStack.pop_back();
        } else if(phase == BashClass::PHASE_EVAL) {
            m_scopeStack.pop_back();
        } else if(phase == BashClass::PHASE_GENERATE) {
            auto elifScope = std::static_pointer_cast<BElif>(m_scopeStack.back());

            // Generate code to close elif statement
            BBashHelper::closeElif(elifScope);
            m_scopeStack.pop_back();
        }
    });

    /**************************************
     *          ELSE STATEMENT
     **************************************/
    m_easyCC->registerSemanticAction("#startElse#",[&](int phase, LexicalTokens &lexicalVector, int index){
        if(phase == BashClass::PHASE_CREATE) {
            auto newElse = std::make_shared<BElse>();
            m_scopeStack.back()->registerScope(m_referenceKey, newElse);
            m_scopeStack.push_back(newElse);

            // Link else to the if scope
            m_focusIf->setElse(newElse);

            // Clear focus
            m_focusIf = nullptr;

        } else if(phase == BashClass::PHASE_EVAL) {
            auto createdElse = m_scopeStack.back()->getScopeByReferenceKey(m_referenceKey);
            m_scopeStack.push_back(createdElse);

        } else if(phase == BashClass::PHASE_GENERATE) {
            auto createdElse = std::static_pointer_cast<BElse>(m_scopeStack.back()->getScopeByReferenceKey(m_referenceKey));
            m_scopeStack.push_back(createdElse);
            BBashHelper::createElse(createdElse);
        }
    });

    m_easyCC->registerSemanticAction("#endElse#",[&](int phase, LexicalTokens &lexicalVector, int index){
        if(phase == BashClass::PHASE_CREATE || phase == BashClass::PHASE_EVAL) {
            m_scopeStack.pop_back();
        } else if(phase == BashClass::PHASE_GENERATE) {
            auto elseScope = std::static_pointer_cast<BElse>(m_scopeStack.back());

            // Generate code for closing the else statement
            BBashHelper::closeElse(elseScope);
            m_scopeStack.pop_back();
        }
    });

    /**************************
     *      CHAIN ELEMENTS
     **************************/

    m_easyCC->registerSemanticAction("#startChain#",[&](int phase, LexicalTokens &lexicalVector, int index){
        if(phase == BashClass::PHASE_EVAL) {
            m_chainBuilderStack.push_back(std::make_shared<BChain>());
        }
    });

    m_easyCC->registerSemanticAction("#endChain#",[&](int phase, LexicalTokens &lexicalVector, int index){
        if(phase == BashClass::PHASE_EVAL) {
            m_chainBuilderStack.pop_back();
        }
    });

    m_easyCC->registerSemanticAction("#varChainAccess#",[&](int phase, LexicalTokens &lexicalVector, int index){
        if(phase == BashClass::PHASE_EVAL) {
            m_chainBuilderStack.back()->addVariable(m_scopeStack.back(), lexicalVector[index]);
        }
    });

    m_easyCC->registerSemanticAction("#functionChainCall#",[&](int phase, LexicalTokens &lexicalVector, int index){
        if(phase == BashClass::PHASE_EVAL) {
            m_chainBuilderStack.back()->addFunction(m_scopeStack.back(), lexicalVector[index]);
        }
    });

    m_easyCC->registerSemanticAction("#constructorChainCall#",[&](int phase, LexicalTokens &lexicalVector, int index){
        if(phase == BashClass::PHASE_EVAL) {
            m_chainBuilderStack.back()->addConstructor(m_scopeStack.back(), lexicalVector[index]);
        }
    });

    m_easyCC->registerSemanticAction("#thisChainAccess#",[&](int phase, LexicalTokens &lexicalVector, int index){
        if(phase == BashClass::PHASE_EVAL) {
            auto thisCall = std::make_shared<BThisChainAccess>();
            thisCall->setLexicalToken(lexicalVector[index]);
            m_chainBuilderStack.back()->addThis(m_scopeStack.back(), thisCall);
        }
    });

    m_easyCC->registerSemanticAction("#superChainAccess#",[&](int phase, LexicalTokens &lexicalVector, int index){
        if(phase == BashClass::PHASE_EVAL) {
            auto superCall = std::make_shared<BSuperChainAccess>();
            superCall->setLexicalToken(lexicalVector[index]);
            m_chainBuilderStack.back()->addSuper(m_scopeStack.back(), superCall);
        }
    });

    m_easyCC->registerSemanticAction("#superConstructorChainCall#",[&](int phase, LexicalTokens &lexicalVector, int index){
        if(phase == BashClass::PHASE_EVAL) {
            m_chainBuilderStack.back()->addSuperConstructor(m_scopeStack.back(), lexicalVector[index]);
        }
    });

    /**************************************
     *          RETURN STATEMENT
     **************************************/

    m_easyCC->registerSemanticAction("#startReturn#",[&](int phase, LexicalTokens &lexicalVector, int index) {
        // Do nothing ...
    });

    m_easyCC->registerSemanticAction("#returnVoid#",[&](int phase, LexicalTokens &lexicalVector, int index) {
        if(phase == BashClass::PHASE_EVAL) {

            // Create a void return
            auto returnComp = std::make_shared<BReturn>();
            m_scopeStack.back()->setReturn(returnComp);
        }
    });

    m_easyCC->registerSemanticAction("#returnExpr#",[&](int phase, LexicalTokens &lexicalVector, int index){
        if(phase == BashClass::PHASE_EVAL) {

            // Create and configure return statement
            auto returnComp = std::make_shared<BReturn>();
            returnComp->setExpression(m_expressionOperandStack.back());

            // Register return statement
            m_scopeStack.back()->setReturn(returnComp);

            // Remove consumed expression
            m_expressionOperandStack.pop_back();
        }
    });

    m_easyCC->registerSemanticAction("#endReturn#",[&](int phase, LexicalTokens &lexicalVector, int index){
        if(phase == BashClass::PHASE_GENERATE) {
            // Generate return statement
            BBashHelper::writeReturn(m_scopeStack.back()->getReturn());
        }
    });

    /**************************************
     *          ARGUMENT PASS
     **************************************/

    m_easyCC->registerSemanticAction("#startArgument#",[&](int phase, LexicalTokens &lexicalVector, int index){
        // Do nothing ...
    });

    m_easyCC->registerSemanticAction("#setArgument#",[&](int phase, LexicalTokens &lexicalVector, int index){
        if(phase == BashClass::PHASE_EVAL) {

            // Get function that is currently being built
            auto functionCall = std::static_pointer_cast<BFunctionChainCall>(m_chainBuilderStack.back()->last());

            // Add the argument to it
            functionCall->addArgument(m_expressionOperandStack.back());

            // Remove consumed expression
            m_expressionOperandStack.pop_back();
        }
    });

    m_easyCC->registerSemanticAction("#endArgument#",[&](int phase, LexicalTokens &lexicalVector, int index){
        if(phase == BashClass::PHASE_EVAL) {

            // Get function chain call
            auto functionCall = std::static_pointer_cast<BFunctionChainCall>(m_chainBuilderStack.back()->last());

            // Verify provided arguments
            functionCall->verifyArguments();
        }
    });

    /**************************************
     *          EXPRESSION
     **************************************/

    m_easyCC->registerSemanticAction("#createExpr1#",[&](int phase, LexicalTokens &lexicalVector, int index){
        if(phase == BashClass::PHASE_EVAL) {

            // Get all expression elements
            auto rightOperand = m_expressionOperandStack.back();
            m_expressionOperandStack.pop_back();
            auto operatorToken = m_expressionOperatorStack.back();
            m_expressionOperatorStack.pop_back();

            // Create expression
            auto expression = std::make_shared<BArithOperation>();
            expression->setRightOperand(rightOperand);
            expression->setOperator(operatorToken);

            // Evaluate expression
            expression->evaluate();

            // Push expression again as an operand
            m_expressionOperandStack.push_back(expression);
        }
    });

    m_easyCC->registerSemanticAction("#createExpr2#",[&](int phase, LexicalTokens &lexicalVector, int index){
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

            // Evaluate expression
            expression->evaluate();

            // Push expression again as an operand
            m_expressionOperandStack.push_back(expression);
        }
    });

    m_easyCC->registerSemanticAction("#putOp#",[&](int phase, LexicalTokens &lexicalVector, int index){
        if(phase == BashClass::PHASE_EVAL) {
            m_expressionOperatorStack.push_back(lexicalVector[index]);
        }
    });

    m_easyCC->registerSemanticAction("#tokenUse#",[&](int phase, LexicalTokens &lexicalVector, int index){
        if(phase == BashClass::PHASE_EVAL) {
            auto token = std::make_shared<BTokenUse>();
            token->setLexicalToken(lexicalVector[index]);
            m_expressionOperandStack.push_back(token);
        }
    });

    m_easyCC->registerSemanticAction("#thisAccess#",[&](int phase, LexicalTokens &lexicalVector, int index){
        if(phase == BashClass::PHASE_EVAL) {
            auto thisChainAccess = std::make_shared<BThisChainAccess>();
            thisChainAccess->setLexicalToken(lexicalVector[index]);
            m_chainBuilderStack.back()->addThis(m_scopeStack.back(), thisChainAccess);
            auto thisAccess = std::make_shared<BThisAccess>();
            thisAccess->setChain(m_chainBuilderStack.back());
            m_expressionOperandStack.push_back(thisAccess);
        }
    });

    m_easyCC->registerSemanticAction("#varAccess#",[&](int phase, LexicalTokens &lexicalVector, int index){
        if(phase == BashClass::PHASE_EVAL) {
            auto variableAccess = std::make_shared<BVariableAccess>();
            variableAccess->setChain(m_chainBuilderStack.back());
            m_expressionOperandStack.push_back(variableAccess);
        }
    });

    m_easyCC->registerSemanticAction("#functionCall#",[&](int phase, LexicalTokens &lexicalVector, int index){
        if(phase == BashClass::PHASE_EVAL) {
            auto functionCall = std::make_shared<BFunctionCall>();
            functionCall->setChain(m_chainBuilderStack.back());
            m_expressionOperandStack.push_back(functionCall);
        }
    });

    m_easyCC->registerSemanticAction("#evalExpr#",[&](int phase, LexicalTokens &lexicalVector, int index) {
        if(phase == BashClass::PHASE_EVAL) {
            auto expression = m_expressionOperandStack.back();
            m_scopeStack.back()->registerExpression(m_referenceKey, expression);

            // Remove consumed expression
            m_expressionOperandStack.pop_back();
        } else if(phase == BashClass::PHASE_GENERATE) {
            auto expression = m_scopeStack.back()->getExpressionByReferenceKey(m_referenceKey);
            BBashHelper::writeExpression(m_scopeStack.back(), expression);
        }
    });

    m_easyCC->registerSemanticAction("#varInit#",[&](int phase, LexicalTokens &lexicalVector, int index){
        if(phase == BashClass::PHASE_EVAL) {

            // Register expression in variable
            m_focusVariable->setExpression(m_expressionOperandStack.back());

            // Remove consumed expression
            m_expressionOperandStack.pop_back();
        }
    });

    m_easyCC->registerSemanticAction("#varAsOperand#",[&](int phase, LexicalTokens &lexicalVector, int index){
        if(phase == BashClass::PHASE_EVAL) {

            // Create variable access, the operand
            std::shared_ptr<BVariableAccess> variableAccess = std::make_shared<BVariableAccess>();
            std::shared_ptr<BChain> variableAccessChain = std::make_shared<BChain>();
            variableAccessChain->addVariable(m_scopeStack.back(), lexicalVector[index]);
            variableAccess->setChain(variableAccessChain);

            // Push operand
            m_expressionOperandStack.push_back(variableAccess);
        }
    });

    /**************************************
     *              Arrays
     **************************************/
    m_easyCC->registerSemanticAction("#arrayClassVar#",[&](int phase, LexicalTokens &lexicalVector, int index){
        if(phase == BashClass::PHASE_CREATE) {
            m_focusVariable->getType()->setDimension(m_focusVariable->getType()->getDimension()+1);
        }
    });

    m_easyCC->registerSemanticAction("#arrayVar#",[&](int phase, LexicalTokens &lexicalVector, int index){
        if(phase == BashClass::PHASE_EVAL) {
            m_focusVariable->getType()->setDimension(m_focusVariable->getType()->getDimension()+1);
        }
    });

    m_easyCC->registerSemanticAction("#arrayFunc#",[&](int phase, LexicalTokens &lexicalVector, int index){
        if(phase == BashClass::PHASE_CREATE) {
            m_focusFunction->getType()->setDimension(m_focusFunction->getType()->getDimension()+1);
        }
    });

    m_easyCC->registerSemanticAction("#indexAccess#",[&](int phase, LexicalTokens &lexicalVector, int index){
        if(phase == BashClass::PHASE_EVAL) {
            auto chainAccess = m_chainBuilderStack.back()->last();
            chainAccess->addIndex(m_expressionOperandStack.back());
            m_expressionOperandStack.pop_back();
        }
    });

    m_easyCC->registerSemanticAction("#newArray#",[&](int phase, LexicalTokens &lexicalVector, int index){
        if(phase == BashClass::PHASE_EVAL) {
            m_expressionOperandStack.push_back(std::make_shared<BArrayUse>());
        }
    });

    m_easyCC->registerSemanticAction("#arrayUseType#",[&](int phase, LexicalTokens &lexicalVector, int index){
        if(phase == BashClass::PHASE_EVAL) {
            auto arrayUse = std::static_pointer_cast<BArrayUse>(m_expressionOperandStack.back());
            arrayUse->setTypeLexicalToken(lexicalVector[index]);
        }
    });

    m_easyCC->registerSemanticAction("#arrayUseDim#",[&](int phase, LexicalTokens &lexicalVector, int index){
        if(phase == BashClass::PHASE_EVAL) {
            auto arrayUse = std::static_pointer_cast<BArrayUse>(m_expressionOperandStack.back());
            arrayUse->getType()->setDimension(arrayUse->getType()->getDimension()+1);
        }
    });

    /**************************************
     *            CASTING
     **************************************/
    m_easyCC->registerSemanticAction("#castType#",[&](int phase, LexicalTokens &lexicalVector, int index){
        if(phase == BashClass::PHASE_EVAL) {

            // Create a new type
            m_focusCastType = std::make_shared<BElementType>();

            // Configure type
            m_focusCastType->setLexicalToken(lexicalVector[index]);

            // Link type
            m_focusCastType->linkType();
        }
    });

    m_easyCC->registerSemanticAction("#castArray#",[&](int phase, LexicalTokens &lexicalVector, int index){
        if(phase == BashClass::PHASE_EVAL) {
            m_focusCastType->setDimension(m_focusCastType->getDimension() + 1);
        }
    });

    m_easyCC->registerSemanticAction("#castExpr#",[&](int phase, LexicalTokens &lexicalVector, int index){
        if(phase == BashClass::PHASE_EVAL) {

            // Cast expression
            m_expressionOperandStack.back()->castType(m_focusCastType);

            // Clear focus
            m_focusCastType = nullptr;
        }
    });
}
