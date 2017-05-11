#include <bashclass/BashClass.h>
#include <iostream>

BashClass::BashClass() {
    m_global = std::make_shared<BGlobal>();
    m_scopeStack.push(m_global);
    initHandlers();
}

void BashClass::initHandlers() {
    m_startClass = [&](int phase, LexicalTokens &lexicalVector, int index, bool stable){
        if(phase == BashClass::PHASE_CREATE) {
            auto newClass = m_global->createClass();
            m_scopeStack.push(newClass);
        }
    };

    m_className = [&](int phase, LexicalTokens &lexicalVector, int index, bool stable){
        if(phase == BashClass::PHASE_CREATE) {
            auto createdClass = std::dynamic_pointer_cast<BClass>(m_scopeStack.top());
            createdClass->setName(lexicalVector[index]->getValue());
        }
    };

    m_endClass = [&](int phase, LexicalTokens &lexicalVector, int index, bool stable){
        if(phase == BashClass::PHASE_CREATE) {
            m_scopeStack.pop();
        }
    };

    m_startFunction = [&](int phase, LexicalTokens &lexicalVector, int index, bool stable){
        if(phase == BashClass::PHASE_CREATE) {
            auto scopeFunc = std::dynamic_pointer_cast<BScopeFunc>(m_scopeStack.top());
            auto newFunction = scopeFunc->createFunction();
            m_scopeStack.push(newFunction);
        }
    };

    m_functionType = [&](int phase, LexicalTokens &lexicalVector, int index, bool stable){
        if(phase == BashClass::PHASE_CREATE) {
            auto createdFunction = std::dynamic_pointer_cast<BFunction>(m_scopeStack.top());
            createdFunction->setType(lexicalVector[index]->getValue());
        }
    };

    m_functionName = [&](int phase, LexicalTokens &lexicalVector, int index, bool stable){
        if(phase == BashClass::PHASE_CREATE) {
            auto createdFunction = std::dynamic_pointer_cast<BFunction>(m_scopeStack.top());
            createdFunction->setName(lexicalVector[index]->getValue());
        }
    };

    m_endFunction = [&](int phase, LexicalTokens &lexicalVector, int index, bool stable){
        if(phase == BashClass::PHASE_CREATE) {
            m_scopeStack.pop();
        }
    };
}
