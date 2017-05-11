#ifndef BASHCLASS_BFUNCTION_H
#define BASHCLASS_BFUNCTION_H

#include <vector>
#include <bashclass/BVariable.h>

class BFunction {
private:
    std::vector<std::shared_ptr<BVariable>> m_variables;
};

#endif
