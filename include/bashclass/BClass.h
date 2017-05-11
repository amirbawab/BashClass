#ifndef BASHCLASS_BCLASS_H
#define BASHCLASS_BCLASS_H

#include <vector>
#include <memory>
#include <bashclass/BFunction.h>
#include <bashclass/BScope.h>

class BClass : public BScope {
private:
    std::vector<std::shared_ptr<BFunction>> m_functions;
public:
    std::shared_ptr<BFunction> createFunction();
};

#endif
