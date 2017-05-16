#ifndef BASHCLASS_BFUNCTION_H
#define BASHCLASS_BFUNCTION_H

#include <vector>
#include <memory>
#include <bashclass/BScope.h>
#include <bashclass/IBCallable.h>

class BFunction : public BScope, public IBCallable {
private:
    std::string m_name;
    std::string m_type;
    std::shared_ptr<BScope> m_typeScope;
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
     * Get variable type scope
     * @return type
     */
    std::shared_ptr<BScope> getTypeScope() const {return m_typeScope;}

    /**
     * Set variable type scope
     * @param type
     */
    void setTypeScope(std::shared_ptr<BScope> scope) {m_typeScope = scope;}

    /**
     * Get function label
     */
    std::stringstream getLabel();
};

#endif
