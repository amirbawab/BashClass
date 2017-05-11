#ifndef BASHCLASS_BSCOPEFUNC_H
#define BASHCLASS_BSCOPEFUNC_H

#include <bashclass/BFunction.h>
#include <bashclass/BScope.h>

class BScopeFunc : public BScope {
protected:
    std::vector<std::shared_ptr<BFunction>> m_functions;
public:
    virtual std::shared_ptr<BFunction> createFunction();
};

#endif
