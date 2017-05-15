#include <bashclass/BashClass.h>
#include <bashclass/BTypes.h>
#include <bashclass/BClass.h>
#include <iostream>

BashClass::BashClass() {
    m_global = std::make_shared<BGlobal>();
    m_scopeStack.push_back(m_global);
    m_focusScope = nullptr;
    m_focusVariable = nullptr;
    initHandlers();
}

void BashClass::printStructure() {
    std::cout << m_global->getStructure().str() << std::endl;
}

/**
 * Run a depth-first traversal and link all the variables
 * to their types
 */
void _linkTypes(std::shared_ptr<BScope> scope) {

    // Check if all variables types exist and set them
    std::stack<std::shared_ptr<BScope>> scopeStack;
    scopeStack.push(scope);
    while(!scopeStack.empty()) {

        // Get top scope
        auto top = scopeStack.top();
        scopeStack.pop();

        // Evaluate all variables in the top scope
        for(auto variable : top->findAllVariables()) {
            if(!BType::isBuiltInType(variable->getType())) {
                // Find class scope of that type
                auto cls = scope->findAllClasses(variable->getType().c_str());
                if(cls.empty()) {
                    std::cerr << "Undefined variable type '" << variable->getType() <<
                    "' for '" << variable->getName() << "'"<< std::endl;
                } else {
                    // In case the class was defined several times (which is a semantic error)
                    // take the front element
                    variable->setTypeScope(cls.front());
                }
            }
        }

        // Add all children scopes to the stack
        for(auto childScope : top->getAllScopes()) {
            scopeStack.push(childScope);
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
        std::cerr << "Function '" << token.getValue() << "' at line " << token.getLine()
        <<" was defined previously in the same scope" << std::endl;
    }
}

/**
 * Check if variable was already defined in the scope
 */
void _checkDuplicateVariable(ecc::LexicalToken &token, std::vector<std::shared_ptr<BScope>> &scopeStack) {
    auto getVar = scopeStack.back()->findAllVariables(token.getValue().c_str());
    if(!getVar.empty()) {
        std::cerr << "Variable '" << token.getValue() << "' at line " << token.getLine()
        << " was defined previously in the same scope" << std::endl;
    }
}

void BashClass::initHandlers() {

    /**************************************
     *          PROGRAM
     **************************************/
    m_start = [&](int phase, LexicalTokens &lexicalVector, int index, bool stable){
        if(phase == BashClass::PHASE_EVAL_GEN) {
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
            m_focusScope = newClass;
        } else if(phase == BashClass::PHASE_EVAL_GEN) {
            auto newClass = m_scopeStack.back()->getScope(lexicalVector[index]);
            m_scopeStack.push_back(newClass);
            m_focusScope = newClass;
        }
    };

    m_className = [&](int phase, LexicalTokens &lexicalVector, int index, bool stable){
        if(phase == BashClass::PHASE_CREATE) {

            // Check if class already exists
            _checkDuplicateClass(*lexicalVector[index], m_scopeStack);

            // Set class name
            auto createdClass = std::dynamic_pointer_cast<BClass>(m_focusScope);
            createdClass->setName(lexicalVector[index]->getValue());
        }
    };

    m_endClass = [&](int phase, LexicalTokens &lexicalVector, int index, bool stable){
        if(phase == BashClass::PHASE_CREATE || phase == BashClass::PHASE_EVAL_GEN) {
            m_scopeStack.pop_back();
            m_focusScope = nullptr;
        }
    };

    /**************************************
     *          FUNCTIONS
     **************************************/
    m_startFunction = [&](int phase, LexicalTokens &lexicalVector, int index, bool stable){
        if(phase == BashClass::PHASE_CREATE) {
            auto newFunction = m_scopeStack.back()->createFunction(lexicalVector[index]);
            m_scopeStack.push_back(newFunction);
            m_focusScope = newFunction;
        } else if(phase == BashClass::PHASE_EVAL_GEN) {
            auto newFunction = m_scopeStack.back()->getScope(lexicalVector[index]);
            m_scopeStack.push_back(newFunction);
            m_focusScope = newFunction;
        }
    };

    m_functionType = [&](int phase, LexicalTokens &lexicalVector, int index, bool stable){
        if(phase == BashClass::PHASE_CREATE) {
            auto createdFunction = std::dynamic_pointer_cast<BFunction>(m_focusScope);
            createdFunction->setType(lexicalVector[index]->getValue());
        }
    };

    m_functionName = [&](int phase, LexicalTokens &lexicalVector, int index, bool stable){
        if(phase == BashClass::PHASE_CREATE) {

            // Check if function already exists in the current scope
            _checkDuplicateFunction(*lexicalVector[index],m_scopeStack);

            auto createdFunction = std::dynamic_pointer_cast<BFunction>(m_focusScope);
            createdFunction->setName(lexicalVector[index]->getValue());
        }
    };

    m_endFunction = [&](int phase, LexicalTokens &lexicalVector, int index, bool stable){
        if(phase == BashClass::PHASE_CREATE || phase == BashClass::PHASE_EVAL_GEN) {
            m_scopeStack.pop_back();
            m_focusScope = nullptr;
        }
    };

    /**************************************
     *          VARIABLES
     **************************************/
    m_startVar = [&](int phase, LexicalTokens &lexicalVector, int index, bool stable){
        if(phase == BashClass::PHASE_CREATE) {
            auto variable = m_scopeStack.back()->createVariable(lexicalVector[index]);
            m_focusVariable = variable;
        }
    };

    m_varType = [&](int phase, LexicalTokens &lexicalVector, int index, bool stable){
        if(phase == BashClass::PHASE_CREATE) {
            m_focusVariable->setType(lexicalVector[index]->getValue());
        }
    };

    m_varName = [&](int phase, LexicalTokens &lexicalVector, int index, bool stable){
        if(phase == BashClass::PHASE_CREATE) {

            // Check if variable already exists in the current scope
            _checkDuplicateVariable(*lexicalVector[index], m_scopeStack);

            m_focusVariable->setName(lexicalVector[index]->getValue());
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
        }
    };

    m_paramType = [&](int phase, LexicalTokens &lexicalVector, int index, bool stable){
        if(phase == BashClass::PHASE_CREATE) {
            const auto &variables = m_scopeStack.back()->findAllVariables();
            variables[variables.size()-1]->setType(lexicalVector[index]->getValue());
        }
    };

    m_paramName = [&](int phase, LexicalTokens &lexicalVector, int index, bool stable){
        if(phase == BashClass::PHASE_CREATE) {
            const auto &variables = m_scopeStack.back()->findAllVariables();
            variables[variables.size()-1]->setName(lexicalVector[index]->getValue());
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
            m_focusScope = newWhile;
        } else if(phase == BashClass::PHASE_EVAL_GEN) {
            auto newWhile = m_scopeStack.back()->getScope(lexicalVector[index]);
            m_scopeStack.push_back(newWhile);
            m_focusScope = newWhile;
        }
    };

    m_endWhile = [&](int phase, LexicalTokens &lexicalVector, int index, bool stable){
        if(phase == BashClass::PHASE_CREATE || phase == BashClass::PHASE_EVAL_GEN) {
            m_scopeStack.pop_back();
            m_focusScope = nullptr;
        }
    };

    /**************************************
     *          IF
     **************************************/

    m_startIf = [&](int phase, LexicalTokens &lexicalVector, int index, bool stable){
        if(phase == BashClass::PHASE_CREATE) {
            auto newIf = m_scopeStack.back()->createIf(lexicalVector[index]);
            m_scopeStack.push_back(newIf);
            m_focusScope = newIf;
        } else if(phase == BashClass::PHASE_EVAL_GEN) {
            auto newIf = m_scopeStack.back()->getScope(lexicalVector[index]);
            m_scopeStack.push_back(newIf);
            m_focusScope = newIf;
        }
    };

    m_endIf = [&](int phase, LexicalTokens &lexicalVector, int index, bool stable){
        if(phase == BashClass::PHASE_CREATE || phase == BashClass::PHASE_EVAL_GEN) {
            m_scopeStack.pop_back();
            m_focusScope = nullptr;
        }
    };
}
