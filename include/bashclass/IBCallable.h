#ifndef BASHCLASS_IBCALLABLE_H
#define BASHCLASS_IBCALLABLE_H

#include <string>
#include <memory>
#include <easycc/LexicalToken.h>

class BScope;
class IBCallable {
public:
    virtual std::shared_ptr<ecc::LexicalToken> getType() const=0;
    virtual std::shared_ptr<BScope> getTypeScope() const=0;
};

#endif
