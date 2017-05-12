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

    // Semantic action handlers
    SemanticActionHandler m_startClass;
    SemanticActionHandler m_className;
    SemanticActionHandler m_endClass;

    SemanticActionHandler m_startFunction;
    SemanticActionHandler m_functionType;
    SemanticActionHandler m_functionName;
    SemanticActionHandler m_endFunction;

    SemanticActionHandler m_createVar;
    SemanticActionHandler m_varType;
    SemanticActionHandler m_varName;
private:
    std::shared_ptr<BGlobal> m_global;
    std::stack<std::shared_ptr<BScope>> m_scopeStack;
    void initHandlers();
};

#endif
