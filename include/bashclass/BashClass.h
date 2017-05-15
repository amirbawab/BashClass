#ifndef BASHCLASS_BASHCLASS_H
#define BASHCLASS_BASHCLASS_H

#include <easycc/EasyCCDev.h>
#include <stack>
#include <memory>
#include <bashclass/BGlobal.h>

typedef std::vector<std::shared_ptr<ecc::LexicalToken>> LexicalTokens;
typedef std::function<void(int, LexicalTokens&, int, bool)> SemanticActionHandler;

class BashClass {
public:
    static const int PHASE_CREATE = 1;
    static const int PHASE_EVAL_GEN = 2;
    BashClass();

    // Print the structure of the code
    void printStructure();

    // Semantic action handlers
    SemanticActionHandler m_start;
    SemanticActionHandler m_end;

    SemanticActionHandler m_startClass;
    SemanticActionHandler m_className;
    SemanticActionHandler m_endClass;

    SemanticActionHandler m_startFunction;
    SemanticActionHandler m_functionType;
    SemanticActionHandler m_functionName;
    SemanticActionHandler m_endFunction;

    SemanticActionHandler m_startVar;
    SemanticActionHandler m_varType;
    SemanticActionHandler m_varName;
    SemanticActionHandler m_endVar;

    SemanticActionHandler m_startParam;
    SemanticActionHandler m_paramType;
    SemanticActionHandler m_paramName;
    SemanticActionHandler m_endParam;

    SemanticActionHandler m_startWhile;
    SemanticActionHandler m_endWhile;

    SemanticActionHandler m_startIf;
    SemanticActionHandler m_endIf;
private:
    std::shared_ptr<BGlobal> m_global;
    std::vector<std::shared_ptr<BScope>> m_scopeStack;
    std::shared_ptr<BScope> m_focusScope;
    std::shared_ptr<BVariable> m_focusVariable;
    void initHandlers();
};

#endif
