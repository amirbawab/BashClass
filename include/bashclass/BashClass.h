#ifndef BASHCLASS_BASHCLASS_H
#define BASHCLASS_BASHCLASS_H

#include <easycc/EasyCCDev.h>
#include <stack>
#include <memory>
#include <bashclass/BGlobal.h>
#include <bashclass/IBCallable.h>
#include <bashclass/BExpression.h>

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

    SemanticActionHandler m_startWhile;
    SemanticActionHandler m_endWhile;

    SemanticActionHandler m_startIf;
    SemanticActionHandler m_endIf;

    SemanticActionHandler m_startExpr;
    SemanticActionHandler m_endExpr;

    SemanticActionHandler m_varAssign;
    SemanticActionHandler m_setArgument;
    SemanticActionHandler m_startArgument;
    SemanticActionHandler m_endArgument;

private:
    std::shared_ptr<BGlobal> m_global;
    std::vector<std::shared_ptr<BScope>> m_scopeStack;
    std::vector<std::vector<std::shared_ptr<IBCallable>>> m_callableChainStack;
    std::shared_ptr<BVariable> m_focusVariable;
    std::vector<std::shared_ptr<BExpression>> m_expressionStack;
    std::vector<std::vector<std::shared_ptr<BExpression>>> m_argumentListStack;
    void initHandlers();
};

#endif
