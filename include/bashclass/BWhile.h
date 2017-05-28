#ifndef BASHCLASS_BWHILE_H
#define BASHCLASS_BWHILE_H

#include <bashclass/BScope.h>

class BWhile : public BScope {
private:
public:
    /**
     * Get while label
     */
    std::stringstream getLabel();
};

#endif
