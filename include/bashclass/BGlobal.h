#ifndef BASHCLASS_BGLOBAL_H
#define BASHCLASS_BGLOBAL_H

#include <vector>
#include <memory>
#include <bashclass/BFunction.h>

class BGlobal : public BScope {
public:

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
};

#endif
