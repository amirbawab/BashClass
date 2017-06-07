#ifndef BASHCLASS_BFUNCTIONCALL_H
#define BASHCLASS_BFUNCTIONCALL_H

#include <bashclass/IBExpression.h>
#include <bashclass/BChain.h>
#include <bashclass/BFunctionChainCall.h>

class BFunctionCall : public IBExpression {
private:

    // Hold chain for the function call
    std::shared_ptr<BChain> m_chain;

    // Hold parent scope
    std::shared_ptr<BScope> m_parentScope;
public:

    /**
     * Get the type value of the last element
     * in the chain which is a function call
     * @return type as string
     */
    std::string getTypeValueAsString();

    /**
     * Get the last element in the chain
     * @return function chain call
     */
    std::shared_ptr<BFunctionChainCall> last();

    /**
     * Get parent scope
     * @return parent scope
     */
    std::shared_ptr<BScope> getParentScope() {return m_parentScope;}

    /**
     * Set parent scope
     * @param parentScope
     */
    void setParentScope(std::shared_ptr<BScope> parentScope) {m_parentScope = parentScope;}

    /**
     * Get chain
     * @return chain
     */
    std::shared_ptr<BChain> getChain() { return m_chain;}

    /**
     * Set chain
     * @param chain
     */
    void setChain(std::shared_ptr<BChain> chain) {m_chain = chain;}
};

#endif
