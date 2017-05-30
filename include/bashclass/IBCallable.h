#ifndef BASHCLASS_IBCALLABLE_H
#define BASHCLASS_IBCALLABLE_H

#include <string>

class IBCallable {
public:
    virtual std::string getTypeValueAsString()=0;
};

#endif
