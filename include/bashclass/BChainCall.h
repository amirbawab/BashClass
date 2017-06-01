#ifndef BASHCLASS_BCHAINCALL_H
#define BASHCLASS_BCHAINCALL_H

#include <memory>
#include <vector>
#include <bashclass/IBCallable.h>
#include <bashclass/BScope.h>
#include <bashclass/BTokenCall.h>

class BChainCall : public IBCallable {
private:

    // Vector containing the variables and functions calls
    // in their order of insertion.
    // e.g. a.b().c.d() => [a,b(),c,d()]
    std::vector<std::shared_ptr<IBCallable>> m_callables;
public:

    /**
     * Get chain size
     * @return size
     */
    int size() {
        return m_callables.size();
    }

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
    std::shared_ptr<IBCallable> last() {
        if(m_callables.empty()) {
            throw std::runtime_error("Accessing last element of an empty chain");
        }
        return m_callables.back();
    }

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
    void addFunction(std::shared_ptr<BScope> globalScope, std::shared_ptr<ecc::LexicalToken> token);

    /**
     * Add token to the chain
     * @param token
     */
    void addToken(std::shared_ptr<BTokenCall> token);
};

#endif
