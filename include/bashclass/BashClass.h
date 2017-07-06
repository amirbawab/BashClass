#ifndef BASHCLASS_BASHCLASS_H
#define BASHCLASS_BASHCLASS_H

#include <easycc/IEasyCC.h>
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
#include <bashclass/IBType.h>

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
     * Set easycc
     * @param easycc
     */
    void setEasyCC(std::shared_ptr<ecc::IEasyCC> easycc) {m_easyCC = easycc;}

    /**
     * Initialize semantic action handlers
     */
    void initHandlers();

private:

    // Handles the lexical and syntax analysis
    std::shared_ptr<ecc::IEasyCC> m_easyCC;

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

    // Hold cast type
    std::shared_ptr<BElementType> m_focusCastType;

    // Hold chains and expressions operands
    std::vector<std::shared_ptr<IBExpression>> m_expressionOperandStack;

    // Hold expression operators
    std::vector<std::shared_ptr<ecc::LexicalToken>> m_expressionOperatorStack;

    /**
     * Couple of checks required at each beginning of a phase
     */
    void onPhaseStartCheck();
};

#endif
