#ifndef BASHCLASS_IBSIMPLECALLABLE_H
#define BASHCLASS_IBSIMPLECALLABLE_H

#include <bashclass/IBCallable.h>
#include <bashclass/BClass.h>
#include <memory>

class IBSimpleCallable : public IBCallable {
public:
    virtual bool isKnown()=0;
    virtual std::shared_ptr<BClass> getTypeScope()=0;
};

#endif
