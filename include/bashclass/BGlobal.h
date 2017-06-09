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

    // Set main function name
    static const std::string MAIN_FUNCTION;

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
     * There is no ancestor function scope for a global scope
     * @return nullptr
     */
    std::shared_ptr<BFunction> findClosestFunction() {return nullptr;}

    /**
     * There is no ancestor class scope for a global scope
     * @return nullptr
     */
    std::shared_ptr<BClass> findClosestClass() {return nullptr;}

    /**
     * Verify that the main function exists in the global scope
     */
    void verifyMain();
};

#endif
