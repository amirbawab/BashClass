#ifndef BASHCLASS_BCHAIN_H
#define BASHCLASS_BCHAIN_H

#include <memory>
#include <vector>
#include <bashclass/BScope.h>
#include <bashclass/BTokenUse.h>
#include <bashclass/BThisChainAccess.h>
#include <bashclass/IBChainable.h>

class BChain {
private:

    // Vector containing the variables and functions calls
    // in their order of insertion.
    // e.g. a.b().c.d() => [a,b(),c,d()]
    std::vector<std::shared_ptr<IBChainable>> m_callables;

    // Parent scope of this chain call
    std::shared_ptr<BScope> m_parentScope;
public:

    /**
     * Get chain size
     * @return size
     */
    size_t size() {
        return m_callables.size();
    }

    /**
     * Get callable element at the provided index
     * @param index
     * @return callable item
     */
    std::shared_ptr<IBChainable> operator[](int index);

    /**
     * Check if chain is empty
     * @return true if empty
     */
    bool empty() {
        return m_callables.empty();
    }

    /**
     * Get the last element or nullptr
     * @return last or nullptr
     */
    std::shared_ptr<IBChainable> last();

    /**
     * Get chain type
     * @return chain type
     */
    std::string getTypeValueAsString();

    /**
     * Add variable to the chain
     * @param scope
     * @param token
     */
    void addVariable(std::shared_ptr<BScope> scope, std::shared_ptr<ecc::LexicalToken> token);

    /**
     * Add function to the chain
     * @param scope
     * @param token
     */
    void addFunction(std::shared_ptr<BScope> scope, std::shared_ptr<ecc::LexicalToken> token);

    /**
     * Add token to the chain
     * @param token
     */
    void addToken(std::shared_ptr<BTokenCall> token);

    /**
     * Add this reference to the chain
     * @param scope
     * @param thisReference
     */
    void addThis(std::shared_ptr<BScope> scope, std::shared_ptr<BThisChainCall> thisReference);

    /**
     * Set parent scope
     * @param parentScope
     */
    void setParentScope(std::shared_ptr<BScope> parentScope) {m_parentScope = parentScope;}

    /**
     * Get parent scope
     * @return parent scope
     */
    std::shared_ptr<BScope> getParentScope() {return m_parentScope;}
};

#endif
