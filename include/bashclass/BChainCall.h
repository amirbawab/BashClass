#ifndef BASHCLASS_BCHAINCALL_H
#define BASHCLASS_BCHAINCALL_H

#include <memory>
#include <vector>
#include <bashclass/IBCallable.h>

class BChainCall : public IBCallable {
private:
    std::vector<std::shared_ptr<IBCallable>> m_callables;
public:

    /**
     * Get a call to the chain
     * @param call
     */
    void add(std::shared_ptr<IBCallable> call) {
        m_callables.push_back(call);
    }

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
};

#endif
