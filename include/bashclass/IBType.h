#ifndef BASHCLASS_IBTYPE_H
#define BASHCLASS_IBTYPE_H

#include <string>
#include <memory>

class BClass;
class BElementType;
class IBType {
protected:

    // Hold type scope
    std::shared_ptr<BClass> m_typeScope;

    // Hold the dimension if the type is an array
    int m_dimension;
public:

    /**
     * Initialize members
     */
    IBType() : m_dimension(0){}

    // Values defining the built-in types names. For example:
    // var [int] a;
    // lexicalToken->getName()
    static const std::string TYPE_NAME_INT;
    static const std::string TYPE_NAME_CHAR;
    static const std::string TYPE_NAME_BOOLEAN;
    static const std::string TYPE_NAME_VOID;
    static const std::string TYPE_NAME_IDENTIFIER;

    // Values defining the built-in types values. For example:
    // var [int] a;
    // lexicalToken->getValue()
    static const std::string TYPE_VALUE_INT;
    static const std::string TYPE_VALUE_CHAR;
    static const std::string TYPE_VALUE_BOOLEAN;
    static const std::string TYPE_VALUE_VOID;

    // Undefined type is used by the compiler when an expression has an unknown type
    // e.g. Divide a string by another
    // When "undefined" is used by the user as input, it will behave as an alias
    // to "null"
    // "undefined" must be a reserved word to avoid a user defined class called
    // undefined
    static const std::string UNDEFINED;

    // Null is used to unlink a reference of an object
    static const std::string NULL_VALUE;

    // Those values represent the type name of data
    // a = [1234];
    // lexicalToken->getName()
    static const std::string DATA_TYPE_NAME_INT;
    static const std::string DATA_TYPE_NAME_STRING;
    static const std::string DATA_TYPE_NAME_CHAR;
    static const std::string DATA_TYPE_NAME_BOOLEAN;
    static const std::string DATA_TYPE_NAME_BASH_SUB;
    static const std::string DATA_TYPE_NAME_BASH_INLINE;
    static const std::string DATA_TYPE_NAME_BASH_BLOCK;

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
    std::shared_ptr<BClass> getTypeScope() {return m_typeScope;}

    /**
     * Int check
     * @return boolean
     */
    bool isInt() { return getTypeName() == IBType::TYPE_NAME_INT;};

    /**
     * Void check
     * @return boolean
     */
    bool isVoid() {return getTypeName() == IBType::TYPE_NAME_VOID;}

    /**
     * Char check
     * @return boolean
     */
    bool isChar() {return getTypeName() == IBType::TYPE_NAME_CHAR;}

    /**
     * Boolean check
     * @return boolean
     */
    bool isBoolean() {return getTypeName() == IBType::TYPE_NAME_BOOLEAN;}

    /**
     * Identifier check
     * @return boolean
     */
    bool isIdentifier() { return getTypeName() == IBType::TYPE_NAME_IDENTIFIER; }

    /**
     * Null check
     * @return boolean
     */
    bool isNull() {return getTypeName() == IBType::NULL_VALUE;}

    /**
     * Undefined check
     * @return boolean
     */
    bool isUndefined() { return getTypeName() == IBType::UNDEFINED;}

    /**
     * Check if the type is built-in
     */
    bool isBuiltInType();

    /**
     * Check if type is known
     */
    bool hasKnownType();

    /**
     * Set dimension
     * @param dimension
     */
    void setDimension(int dimension) { m_dimension = dimension;}

    /**
     * Get dimension
     * @return dimension
     */
    int getDimension() {return m_dimension;}

    /**
     * Check if type is an array
     * @return true if it is an array
     */
    bool isArray() {return m_dimension > 0;}

    /**
     * Check if type is compatible with the provided one
     * e.g. a = b;
     * Then the check is as follow: a.isCompatible(b)
     * @return true if it is compatible
     */
    bool isCompatible(std::shared_ptr<IBType> type);

    /**
     * To string representation
     * @return string
     */
    std::string toString();

    /**
     * Cast type
     * @param type
     */
    virtual void cast(std::shared_ptr<BElementType> type);
};

#endif
