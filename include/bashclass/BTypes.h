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
    static const std::string TYPE_NAME_BASH_SUB;

    // Values defining the built-in types values. For example:
    // var [int] a;
    // lexicalToken->getValue()
    static const std::string TYPE_VALUE_INT;
    static const std::string TYPE_VALUE_CHAR;
    static const std::string TYPE_VALUE_BOOLEAN;
    static const std::string TYPE_VALUE_STRING;
    static const std::string TYPE_VALUE_ANY;
    static const std::string TYPE_VALUE_VOID;

    // This type is used when a variable has an unknown type
    // "undefined" is a reserved word, therefore it cannot refer to a class
    // defined by the user
    static const std::string UNDEFINED;

    // Those values represent the type name of data
    // a = [1234];
    // lexicalToken->getName()
    static const std::string DATA_TYPE_NAME_INT;
    static const std::string DATA_TYPE_NAME_STRING;
    static const std::string DATA_TYPE_NAME_CHAR;
    static const std::string DATA_TYPE_NAME_BOOLEAN;

    /**
     * Check if the provided type name is built-in
     * @param type name
     */
    static bool isBuiltInType(std::string type);
};
#endif
