#ifndef BASHCLASS_BCLASS_H
#define BASHCLASS_BCLASS_H

#include <vector>
#include <memory>
#include <bashclass/BFunction.h>
#include <bashclass/BScopeFunc.h>

class BClass : public BScopeFunc {
private:
    std::string m_name;
public:
    std::string getName() const { return m_name;}
    void setName(std::string name) {m_name = name;}
};

#endif
