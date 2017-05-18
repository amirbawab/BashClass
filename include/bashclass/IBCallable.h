#ifndef BASHCLASS_IBCALLABLE_H
#define BASHCLASS_IBCALLABLE_H

#include <string>
#include <memory>
#include <easycc/LexicalToken.h>

class BScope;
class IBCallable {
public:
    virtual std::string getTypeValue() const=0;
};

#endif
