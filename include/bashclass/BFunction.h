#ifndef BASHCLASS_BFUNCTION_H
#define BASHCLASS_BFUNCTION_H

#include <vector>
#include <memory>
#include <bashclass/BScope.h>

class BFunction : public BScope {
private:
    std::string m_name;
    std::string m_type;
public:
    std::string getName() const{return m_name;}
    void setName(std::string name) { m_name = name;}
    std::string getType() const {return m_type; }
    void setType(std::string type) { m_type = type;}
};

#endif
