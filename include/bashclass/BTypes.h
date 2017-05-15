#ifndef BASHCLASS_BTYPES_H
#define BASHCLASS_BTYPES_H

struct BType{

    // Language built-in types
    static const char* TYPE_INT;
    static const char* TYPE_STRING;
    static const char* TYPE_ANY;
    static const char* TYPE_VOID;

    static bool isBuiltInType(std::string type) {
        return type == TYPE_INT || type == TYPE_STRING || type == TYPE_ANY || type == TYPE_VOID;
    }
};

const char* BType::TYPE_INT = "int";
const char* BType::TYPE_STRING = "string";
const char* BType::TYPE_ANY = "any";
const char* BType::TYPE_VOID = "void";
#endif
