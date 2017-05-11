#ifndef BASHCLASS_BCLASS_H
#define BASHCLASS_BCLASS_H

#include <vector>
#include <memory>
#include <bashclass/BFunction.h>
#include <bashclass/BVariable.h>

class BClass {
private:
    std::vector<std::shared_ptr<BFunction>> m_functions;
    std::vector<std::shared_ptr<BVariable>> m_variables;
};

#endif
