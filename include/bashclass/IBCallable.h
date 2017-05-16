#ifndef BASHCLASS_IBCALLABLE_H
#define BASHCLASS_IBCALLABLE_H

#include <string>

class IBCallable {
public:
    virtual std::string getName() const=0;
};

#endif
