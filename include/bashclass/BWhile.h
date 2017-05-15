#ifndef BASHCLASS_BWHILE_H
#define BASHCLASS_BWHILE_H

#include <bashclass/BScope.h>

class BWhile : public BScope {
public:
    std::stringstream getLabel();
};

#endif
