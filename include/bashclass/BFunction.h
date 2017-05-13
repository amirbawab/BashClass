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
     * Find all parameter variables
     * @param name Name of the parameter variable | nullptr
     * @return if name is a nullptr, then return all the parameter variables
     * in their order of insertion, otherwise return the parameter variables
     * that matches the passed name
     */
    std::vector<std::shared_ptr<BVariable>> findAllParameters(char* name = nullptr);
};

#endif
