#ifndef BASHCLASS_BTYPES_H
#define BASHCLASS_BTYPES_H

#include <string>

class BType{
public:

    /**
     * Built int types
     */
    static const std::string TYPE_NAME_INT;
    static const std::string TYPE_NAME_STRING;
    static const std::string TYPE_NAME_ANY;
    static const std::string TYPE_NAME_VOID;
    static const std::string UNDEFINED;

    /**
     * Built in data type
     */
    static const std::string DATA_TYPE_NAME_INT;
    static const std::string DATA_TYPE_NAME_STRING;

    /**
     * Check if the provided type name is built-in
     * @param type name
     */
    static bool isBuiltInType(std::string type);
};
#endif
