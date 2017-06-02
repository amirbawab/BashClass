#ifndef BASHCLASS_BGLOBAL_H
#define BASHCLASS_BGLOBAL_H

#include <vector>
#include <memory>
#include <bashclass/BFunction.h>

class BGlobal : public BScope {
private:

    // Disable constructor
    BGlobal(){}
public:

    /**
     * Get single instance of this class
     * @return pointer to a singleton instance
     */
    static std::shared_ptr<BGlobal> getInstance() {
        static std::shared_ptr<BGlobal> instance(new BGlobal());
        return instance;
    }

    // Delete constructor and operator
    BGlobal(BGlobal const&) = delete;
    void operator=(BGlobal const&) = delete;

    /**
     * Get global label
     */
    std::stringstream getLabel();

    /**
     * Run a depth-first traversal on variable types and detect
     * all circular references
     */
    void detectCircularReference();

    /**
     * Link types of all variables and functions
     */
    void linkTypes();

    /**
     * Write classes headers in bash
     */
    void bashifyClassesHeaders();
};

#endif
