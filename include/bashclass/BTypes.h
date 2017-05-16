#ifndef BASHCLASS_BTYPES_H
#define BASHCLASS_BTYPES_H

#include <string>

class BType{
public:
    // Language built-in types
    static const std::string TYPE_INT;
    static const std::string TYPE_STRING;
    static const std::string TYPE_ANY;
    static const std::string TYPE_VOID;

    static bool isBuiltInType(std::string type) {
        return type == TYPE_INT || type == TYPE_STRING || type == TYPE_ANY || type == TYPE_VOID;
    }
};
#endif
