#ifndef BASHCLASS_BEXPRESSIONTYPE_H
#define BASHCLASS_BEXPRESSIONTYPE_H

#include <bashclass/IBType.h>
#include <bashclass/BClass.h>

class BExpressionType : public IBType {
private:

    // Hold type name
    std::string m_typeName;

    // Hold type value
    std::string m_typeValue;
public:

    /**
     * Get type name
     * @return type name
     */
    std::string getTypeName() {return m_typeName;}

    /**
     * Set type name
     * @param typeName
     */
    void setTypeName(std::string typeName) { m_typeName = typeName;}

    /**
     * Get type value
     * @return type value
     */
    std::string getTypeValue() { return m_typeValue; }

    /**
     * Set type value
     * @param typeValue
     */
    void setTypeValue(std::string typeValue) { m_typeValue = typeValue;}

    /**
     * Set type scope
     * @param cls
     */
    void setTypeScope(std::shared_ptr<BClass> cls) { m_typeScope = cls;}

    /**
     * Cast type
     * @param type
     */
    void cast(std::shared_ptr<BElementType> type);
};

#endif
