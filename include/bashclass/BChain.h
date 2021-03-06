#ifndef BASHCLASS_BCHAIN_H
#define BASHCLASS_BCHAIN_H

#include <memory>
#include <vector>
#include <bashclass/BScope.h>
#include <bashclass/BThisChainAccess.h>
#include <bashclass/IBChainable.h>
#include <bashclass/BSuperChainAccess.h>

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
     * Add constructor to the chain
     * @param scope
     * @param token
     */
    void addConstructor(std::shared_ptr<BScope> scope, std::shared_ptr<ecc::LexicalToken> token);

    /**
     * Add super constructor to the chain
     * @param scope
     * @param token
     */
    void addSuperConstructor(std::shared_ptr<BScope> scope, std::shared_ptr<ecc::LexicalToken> token);

    /**
     * Add this reference to the chain
     * @param scope
     * @param thisReference
     */
    void addThis(std::shared_ptr<BScope> scope, std::shared_ptr<BThisChainAccess> thisReference);

    /**
     * Add super reference to the chain
     * @param scope
     * @param superReference
     */
    void addSuper(std::shared_ptr<BScope> scope, std::shared_ptr<BSuperChainAccess> superReference);
};

#endif
