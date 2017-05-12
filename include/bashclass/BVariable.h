#ifndef BASHCLASS_BVARIABLE_H
#define BASHCLASS_BVARIABLE_H

#include <string>
#include <memory>

class BScope;
class BVariable {
private:
    std::string m_type;
    std::string m_name;
    std::string m_value;
    std::shared_ptr<BScope> m_parentScope;
public:

    /**
     * Get variable name
     * @return name
     */
    std::string getName() const {return m_name;}

    /**
     * Set variable name
     * @param name
     */
    void setName(std::string name) {m_name = name;}

    /**
     * Get variable type
     * @return type
     */
    std::string getType() const {return m_type;}

    /**
     * Set variable type
     * @param type
     */
    void setType(std::string type) {m_type = type;}

    /**
     * Get variable value
     * @return value
     */
    std::string getValue() const {return m_value;}

    /**
     * Set variable value
     * @param value
     */
    void setValue(std::string value) {m_value = value;}

    /**
     * Get parent scope
     * @return pointer to parent scope
     */
    std::shared_ptr<BScope> getParentScope() {return m_parentScope;};

    /**
     * Set parent scope
     * @param parent scope pointer
     */
    void setParentScope(std::shared_ptr<BScope> scope) {m_parentScope = scope;}
};

#endif
