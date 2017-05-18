#include <bashclass/BTypes.h>

// Those types are the one understood by the language
// Those values can only be found in the lexical token's name
// e.g. lexicalToken->getName()
const std::string BType::TYPE_NAME_INT = "int_type";
const std::string BType::TYPE_NAME_STRING = "string_type";
const std::string BType::TYPE_NAME_ANY = "any_type";
const std::string BType::TYPE_NAME_VOID = "void_type";

// Those values represent the type name of data
// e.g. 1234 has a token name: integer
const std::string BType::DATA_TYPE_NAME_INT = "integer";
const std::string BType::DATA_TYPE_NAME_STRING = "string";

// This type is used when a variable has an unknown type
// "undefined" is a reserved word, therefore it cannot refer to a class
// defined by the user
const std::string BType::UNDEFINED = "undefined";

bool BType::isBuiltInType(std::string type) {
    return type == TYPE_NAME_INT
           || type == TYPE_NAME_STRING
           || type == TYPE_NAME_ANY
           || type == TYPE_NAME_VOID;
}