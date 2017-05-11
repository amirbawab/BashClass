#ifndef BASHCLASS_BCLASS_H
#define BASHCLASS_BCLASS_H

#include <vector>
#include <memory>
#include <bashclass/BFunction.h>
#include <bashclass/BScope.h>

class BClass : public BScope {
private:
    std::string m_name;
    std::vector<std::shared_ptr<BFunction>> m_functions;
public:
    std::shared_ptr<BFunction> createFunction();
    std::string getName() const { return m_name;}
    void setName(std::string name) {m_name = name;}
};

#endif
