#ifndef BASHCLASS_BGLOBAL_H
#define BASHCLASS_BGLOBAL_H

#include <vector>
#include <memory>
#include <bashclass/BClass.h>
#include <bashclass/BFunction.h>
#include <bashclass/BVariable.h>

class BGlobal {
private:
    std::vector<std::shared_ptr<BClass>> m_classes;
    std::vector<std::shared_ptr<BFunction>> m_functions;
    std::vector<std::shared_ptr<BVariable>> m_variables;
public:
    std::shared_ptr<BClass> createClass();
    std::shared_ptr<BFunction> createFunction();
    std::shared_ptr<BVariable> createVariable();
};

#endif
