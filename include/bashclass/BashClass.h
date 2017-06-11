#ifndef BASHCLASS_BASHCLASS_H
#define BASHCLASS_BASHCLASS_H

#include <easycc/EasyCCDev.h>
#include <stack>
#include <memory>
#include <bashclass/BGlobal.h>
#include <bashclass/BChain.h>
#include <bashclass/BVariableChainAccess.h>
#include <bashclass/BFunctionChainCall.h>
#include <bashclass/BTokenUse.h>
#include <bashclass/BArithOperation.h>
#include <bashclass/BClass.h>
#include <bashclass/BIf.h>

typedef std::vector<std::shared_ptr<ecc::LexicalToken>> LexicalTokens;
typedef std::function<void(int, LexicalTokens&, int, bool)> SemanticActionHandler;

class BashClass {
public:

    // Different phases
    static const int PHASE_CREATE = 1;
    static const int PHASE_EVAL = 2;
    static const int PHASE_GENERATE = 3;

    // Error code
    static const int ERR_CODE_SEMANTIC = 4;

    /**
     * Initialize members
     */
    BashClass();

    // Print the structure of the code
    void printStructure();

    // Semantic action handlers
    SemanticActionHandler m_start;
    SemanticActionHandler m_end;
    SemanticActionHandler m_newKey;

    SemanticActionHandler m_startClass;
    SemanticActionHandler m_className;
    SemanticActionHandler m_endClass;

    SemanticActionHandler m_startFunction;
    SemanticActionHandler m_functionType;
    SemanticActionHandler m_functionName;
    SemanticActionHandler m_endFunction;

    SemanticActionHandler m_startReturn;
    SemanticActionHandler m_returnExpr;
    SemanticActionHandler m_endReturn;

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

    SemanticActionHandler m_varChainAccess;
    SemanticActionHandler m_functionChainCall;
    SemanticActionHandler m_tokenUse;
    SemanticActionHandler m_thisChainAccess;
    SemanticActionHandler m_thisAccess;

    SemanticActionHandler m_bashCode;

    SemanticActionHandler m_putOp;

    SemanticActionHandler m_startWhile;
    SemanticActionHandler m_whileCond;
    SemanticActionHandler m_endWhile;

    SemanticActionHandler m_startIf;
    SemanticActionHandler m_ifCond;
    SemanticActionHandler m_endIf;

    SemanticActionHandler m_startElif;
    SemanticActionHandler m_elifCond;
    SemanticActionHandler m_endElif;

    SemanticActionHandler m_startElse;
    SemanticActionHandler m_endElse;

    SemanticActionHandler m_startExpr;
    SemanticActionHandler m_createExpr;
    SemanticActionHandler m_endExpr;

    SemanticActionHandler m_varAssign;
    SemanticActionHandler m_varAccess;
    SemanticActionHandler m_functionCall;
    SemanticActionHandler m_functionExec;

    SemanticActionHandler m_setArgument;
    SemanticActionHandler m_startArgument;
    SemanticActionHandler m_endArgument;

private:

    // Key updated by a semantic action handler
    unsigned int m_referenceKey;

    // Hold nesting scopes
    std::vector<std::shared_ptr<BScope>> m_scopeStack;

    // Hold calls in order to build chains
    // e.g. a.b(c.d(e.f())) => [a, b], [b, c], [e, f]
    // e.g. a.b = 123;      => [a, b], [123]
    std::vector<std::shared_ptr<BChain>> m_chainBuilderStack;

    // Hold variable currently being defined
    std::shared_ptr<BVariable> m_focusVariable;

    // Hold function currently being defined
    std::shared_ptr<BFunction> m_focusFunction;

    // Hold class currently being defined
    std::shared_ptr<BClass> m_focusClass;

    // Hold if scope being defined
    std::shared_ptr<BIf> m_focusIf;

    // Hold chains and expressions operands
    std::vector<std::shared_ptr<IBExpression>> m_expressionOperandStack;

    // Hold expression operators
    std::vector<std::shared_ptr<ecc::LexicalToken>> m_expressionOperatorStack;

    /**
     * Couple of checks required at each beginning of a phase
     */
    void onPhaseStartCheck();

    /**
     * Initialize semantic action handlers
     */
    void initHandlers();
};

#endif
