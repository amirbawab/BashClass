#ifndef BASHCLASS_BCLASS_H
#define BASHCLASS_BCLASS_H

#include <vector>
#include <memory>
#include <bashclass/BScope.h>

class BClass : public BScope {
private:
    std::string m_name;
public:
    std::string getName() const { return m_name;}
    void setName(std::string name) {m_name = name;}

    /**
     * Get class label
     */
    std::stringstream getLabel();
};

#endif
