#ifndef BASHCLASS_BGLOBAL_H
#define BASHCLASS_BGLOBAL_H

#include <vector>
#include <memory>
#include <bashclass/BClass.h>
#include <bashclass/BFunction.h>

class BGlobal : public BScope {
public:

    /**
     * Get global label
     */
    std::stringstream getLabel();
};

#endif
