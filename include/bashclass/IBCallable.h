#ifndef BASHCLASS_IBCALLABLE_H
#define BASHCLASS_IBCALLABLE_H

#include <string>

class IBCallable {
public:

    /**
     * Get the type of the callable element
     * @return type value as a string
     */
    virtual std::string getTypeValueAsString()=0;
};

#endif
