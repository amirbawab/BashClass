#include <bashclass/IBType.h>

/*********************
 *  TOKEN TYPE NAME
 *********************/
const std::string IBType::TYPE_NAME_INT = "int_type";
const std::string IBType::TYPE_NAME_CHAR = "char_type";
const std::string IBType::TYPE_NAME_BOOLEAN = "boolean_type";
const std::string IBType::TYPE_NAME_STRING = "string_type";
const std::string IBType::TYPE_NAME_ANY = "any_type";
const std::string IBType::TYPE_NAME_VOID = "void_type";
const std::string IBType::TYPE_NAME_IDENTIFIER = "identifier";

/*********************
 *  TOKEN TYPE VALUE
 *********************/
const std::string IBType::TYPE_VALUE_INT = "int";
const std::string IBType::TYPE_VALUE_CHAR = "char";
const std::string IBType::TYPE_VALUE_BOOLEAN = "boolean";
const std::string IBType::TYPE_VALUE_STRING = "string";
const std::string IBType::TYPE_VALUE_ANY = "any";
const std::string IBType::TYPE_VALUE_VOID = "void";

/*********************
 *  TOKEN TYPE DATA
 *********************/
const std::string IBType::DATA_TYPE_NAME_INT = "integer";
const std::string IBType::DATA_TYPE_NAME_CHAR = "character";
const std::string IBType::DATA_TYPE_NAME_BOOLEAN = "truefalse";
const std::string IBType::DATA_TYPE_NAME_STRING = "string";
const std::string IBType::DATA_TYPE_NAME_BASH_SUB = "bash_sub";
const std::string IBType::DATA_TYPE_NAME_BASH_INLINE = "bash_inline";
const std::string IBType::DATA_TYPE_NAME_BASH_BLOCK = "bash_block";

/*********************
 * SPECIAL TOKEN TYPE
 *********************/
const std::string IBType::UNDEFINED = "undefined";
const std::string IBType::NULL_VALUE = "null";

bool IBType::isBuiltInType() {
    return isInt() || isString() || isAny() || isVoid() || isBoolean() || isChar();
}

bool IBType::hasKnownType() {
    return isBuiltInType() || (isIdentifier() && m_typeScope);
}

bool IBType::isCompatible(std::shared_ptr<IBType> type) {
    return isAny() || (isIdentifier() && type->isNull()) || getTypeValue() == type->getTypeValue();
}

void IBType::incrementDimension() {
    ++m_dimension;
}
