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

    /**
     * Get function name
     * @return name
     */
    std::string getName() const{return m_name;}

    /**
     * Set function name
     * @param name
     */
    void setName(std::string name) { m_name = name;}

    /**
     * Get function type
     * @return type
     */
    std::string getType() const {return m_type; }

    /**
     * Set function type
     * @param type
     */
    void setType(std::string type) { m_type = type;}

    /**
     * Get function label
     */
    std::stringstream getLabel();
};

#endif
