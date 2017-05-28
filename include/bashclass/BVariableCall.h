#ifndef BASHCLASS_BVARIABLECALL_H
#define BASHCLASS_BVARIABLECALL_H

#include <bashclass/BVariable.h>
#include <bashclass/IBCallable.h>
#include <memory>

class BVariableCall : public IBCallable {
private:
    std::shared_ptr<BVariable> m_variable;
};


#endif
