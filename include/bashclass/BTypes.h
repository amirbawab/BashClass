#ifndef BASHCLASS_BTYPES_H
#define BASHCLASS_BTYPES_H

#include <string>

class BType{
public:

    // Values defining the built-in types names. For example:
    // var [int] a;
    // lexicalToken->getName()
    static const std::string TYPE_NAME_INT;
    static const std::string TYPE_NAME_CHAR;
    static const std::string TYPE_NAME_BOOLEAN;
    static const std::string TYPE_NAME_STRING;
    static const std::string TYPE_NAME_ANY;
    static const std::string TYPE_NAME_VOID;

    // Values defining the built-in types values. For example:
    // var [int] a;
    // lexicalToken->getValue()
    static const std::string TYPE_VALUE_INT;
    static const std::string TYPE_VALUE_CHAR;
    static const std::string TYPE_VALUE_BOOLEAN;
    static const std::string TYPE_VALUE_STRING;
    static const std::string TYPE_VALUE_ANY;
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
     * Check if the provided type name is built-in
     * @param type name
     */
    static bool isBuiltInType(std::string type);

    /**
     * Check if the type is defined by the user
     * @param type
     * @return true if the type is defined by the user
     */
    static bool isUserDefinedType(std::string type);

    /**
     * Check if an expression type is undefined
     * @return true if expression type is undefined
     */
    static bool isUndefined(std::string type);
};
#endif
