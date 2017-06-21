#ifndef BASHCLASS_IBTYPE_H
#define BASHCLASS_IBTYPE_H

#include <string>
#include <memory>
#include <bashclass/BClass.h>

class IBType {
protected:

    // Hold type scope
    std::shared_ptr<BClass> m_typeScope;
public:

    /**
     * Get type name
     * @return type name
     */
    virtual std::string getTypeName()=0;

    /**
     * Get type value
     * @return type value
     */
    virtual std::string getTypeValue()=0;

    /**
     * Get type scope
     * @return type scope
     */
    virtual std::shared_ptr<BClass> getTypeScope() {return m_typeScope;}
};

#endif
