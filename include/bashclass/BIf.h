#ifndef BASHCLASS_BIF_H
#define BASHCLASS_BIF_H

#include <bashclass/BScope.h>

class BIf : public BScope {
private:
public:
    /**
     * Get if label
     */
    std::stringstream getLabel();
};

#endif
