#include <bashclass/BTypes.h>

const std::string BType::TYPE_NAME_INT = "int_type";
const std::string BType::TYPE_NAME_CHAR = "char_type";
const std::string BType::TYPE_NAME_BOOLEAN = "boolean_type";
const std::string BType::TYPE_NAME_STRING = "string_type";
const std::string BType::TYPE_NAME_ANY = "any_type";
const std::string BType::TYPE_NAME_VOID = "void_type";
const std::string BType::TYPE_NAME_BASH_SUB = "bash_sub";

const std::string BType::TYPE_VALUE_INT = "int";
const std::string BType::TYPE_VALUE_CHAR = "char";
const std::string BType::TYPE_VALUE_BOOLEAN = "boolean";
const std::string BType::TYPE_VALUE_STRING = "string";
const std::string BType::TYPE_VALUE_ANY = "any";
const std::string BType::TYPE_VALUE_VOID = "void";

const std::string BType::DATA_TYPE_NAME_INT = "integer";
const std::string BType::DATA_TYPE_NAME_CHAR = "character";
const std::string BType::DATA_TYPE_NAME_BOOLEAN = "truefalse";
const std::string BType::DATA_TYPE_NAME_STRING = "string";

const std::string BType::UNDEFINED = "undefined";

bool BType::isBuiltInType(std::string type) {
    return type == TYPE_NAME_INT
           || type == TYPE_NAME_STRING
           || type == TYPE_NAME_ANY
           || type == TYPE_NAME_VOID
           || type == TYPE_NAME_BOOLEAN
           || type == TYPE_NAME_CHAR;
}