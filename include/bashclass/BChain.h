#ifndef BASHCLASS_BCHAIN_H
#define BASHCLASS_BCHAIN_H

#include <memory>
#include <vector>
#include <bashclass/BScope.h>
#include <bashclass/BThisChainAccess.h>
#include <bashclass/IBChainable.h>

class BChain {
private:

    // Vector containing 'this' references, variables and functions calls
    // in their order of insertion.
    // e.g. a.b().c.d() => [a,b(),c,d()]
    std::vector<std::shared_ptr<IBChainable>> m_callables;
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
     * Get the last element or throw an exception
     * @return last
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
     * Add this reference to the chain
     * @param scope
     * @param thisReference
     */
    void addThis(std::shared_ptr<BScope> scope, std::shared_ptr<BThisChainAccess> thisReference);
};

#endif
