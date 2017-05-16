#ifndef BASHCLASS_IBCALLABLE_H
#define BASHCLASS_IBCALLABLE_H

#include <string>
#include <memory>

class BScope;
class IBCallable {
public:
    virtual std::string getType() const=0;
    virtual std::shared_ptr<BScope> getTypeScope() const=0;
};

#endif
