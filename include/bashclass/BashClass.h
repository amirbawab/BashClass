#ifndef BASHCLASS_BASHCLASS_H
#define BASHCLASS_BASHCLASS_H

#include <easycc/EasyCCDev.h>
#include <stack>
#include <memory>
#include <bashclass/BGlobal.h>
#include <bashclass/BChainCall.h>
#include <bashclass/BVariableCall.h>
#include <bashclass/BFunctionCall.h>
#include <bashclass/BTokenCall.h>

typedef std::vector<std::shared_ptr<ecc::LexicalToken>> LexicalTokens;
typedef std::function<void(int, LexicalTokens&, int, bool)> SemanticActionHandler;

class BashClass {
public:
    static const int PHASE_CREATE = 1;
    static const int PHASE_EVAL = 2;
    static const int PHASE_GENERATE = 3;
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

    SemanticActionHandler m_returnExpr;

    SemanticActionHandler m_startVar;
    SemanticActionHandler m_varType;
    SemanticActionHandler m_varName;
    SemanticActionHandler m_endVar;

    SemanticActionHandler m_startParam;
    SemanticActionHandler m_paramType;
    SemanticActionHandler m_paramName;
    SemanticActionHandler m_endParam;

    SemanticActionHandler m_startOuterCall;
    SemanticActionHandler m_endOuterCall;

    SemanticActionHandler m_startInnerCall;
    SemanticActionHandler m_endInnerCall;

    SemanticActionHandler m_varCall;
    SemanticActionHandler m_functionCall;
    SemanticActionHandler m_tokenCall;

    SemanticActionHandler m_putOp;

    SemanticActionHandler m_startWhile;
    SemanticActionHandler m_endWhile;

    SemanticActionHandler m_startIf;
    SemanticActionHandler m_ifCond;
    SemanticActionHandler m_whileCond;
    SemanticActionHandler m_endIf;

    SemanticActionHandler m_startExpr;
    SemanticActionHandler m_endExpr;

    SemanticActionHandler m_varAssign;
    SemanticActionHandler m_setArgument;
    SemanticActionHandler m_startArgument;
    SemanticActionHandler m_endArgument;

private:

    // Global scope
    std::shared_ptr<BGlobal> m_global;

    // Hold nesting scopes
    std::vector<std::shared_ptr<BScope>> m_scopeStack;

    // Hold in order to build them
    // e.g. a.b(c.d(e.f())) => [a, b], [b, c], [e, f]
    // e.g. a.b = 123;      => [a, b], [123]
    std::vector<std::shared_ptr<BChainCall>> m_chainBuilderStack;

    // Hold variable currently being defined
    std::shared_ptr<BVariable> m_focusVariable;

    /**
     * Initialize semantic action handlers
     */
    void initHandlers();
};

#endif
