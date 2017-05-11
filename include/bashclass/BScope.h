#ifndef BASHCLASS_BSCOPE_H
#define BASHCLASS_BSCOPE_H

#include <vector>
#include <memory>
#include <bashclass/BVariable.h>

class BScope {
private:
    std::vector<std::shared_ptr<BVariable>> m_variables;
};

#endif
