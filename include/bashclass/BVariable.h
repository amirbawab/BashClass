#ifndef BASHCLASS_BVARIABLE_H
#define BASHCLASS_BVARIABLE_H

#include <string>

class BVariable {
private:
    std::string m_type;
    std::string m_name;
public:
    std::string getName() const {return m_name;}
    void setName(std::string name) {m_name = name;}
    std::string getType() const {return m_type;}
    void setType(std::string type) {m_type = type;}
};

#endif
