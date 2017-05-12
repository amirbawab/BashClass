#ifndef BASHCLASS_BSCOPE_H
#define BASHCLASS_BSCOPE_H

#include <vector>
#include <memory>
#include <bashclass/BVariable.h>

class BScope {
protected:
    std::vector<std::shared_ptr<BVariable>> m_variables;
public:
    virtual std::shared_ptr<BVariable> createVariable();
    const std::vector<std::shared_ptr<BVariable>> &getVariables() const {return m_variables;}
    std::vector<std::shared_ptr<BVariable>> findAllVariables(std::string name);
};

#endif
