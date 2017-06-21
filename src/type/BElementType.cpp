#include <bashclass/BElementType.h>
#include <bashclass/BException.h>

const std::string BElementType::TYPE_NAME_INT = "int_type";
const std::string BElementType::TYPE_NAME_CHAR = "char_type";
const std::string BElementType::TYPE_NAME_BOOLEAN = "boolean_type";
const std::string BElementType::TYPE_NAME_STRING = "string_type";
const std::string BElementType::TYPE_NAME_ANY = "any_type";
const std::string BElementType::TYPE_NAME_VOID = "void_type";

const std::string BElementType::TYPE_VALUE_INT = "int";
const std::string BElementType::TYPE_VALUE_CHAR = "char";
const std::string BElementType::TYPE_VALUE_BOOLEAN = "boolean";
const std::string BElementType::TYPE_VALUE_STRING = "string";
const std::string BElementType::TYPE_VALUE_ANY = "any";
const std::string BElementType::TYPE_VALUE_VOID = "void";

const std::string BElementType::DATA_TYPE_NAME_INT = "integer";
const std::string BElementType::DATA_TYPE_NAME_CHAR = "character";
const std::string BElementType::DATA_TYPE_NAME_BOOLEAN = "truefalse";
const std::string BElementType::DATA_TYPE_NAME_STRING = "string";
const std::string BElementType::DATA_TYPE_NAME_BASH_SUB = "bash_sub";
const std::string BElementType::DATA_TYPE_NAME_BASH_INLINE = "bash_inline";
const std::string BElementType::DATA_TYPE_NAME_BASH_BLOCK = "bash_block";

const std::string BElementType::UNDEFINED = "undefined";

const std::string BElementType::NULL_VALUE = "null";

bool BElementType::isBuiltInType(std::string type) {
    return type == TYPE_NAME_INT
           || type == TYPE_NAME_STRING
           || type == TYPE_NAME_ANY
           || type == TYPE_NAME_VOID
           || type == TYPE_NAME_BOOLEAN
           || type == TYPE_NAME_CHAR;
}

bool BElementType::isUserDefinedType(std::string type) {
    return !isBuiltInType(type) && type != NULL_VALUE;
}

bool BElementType::isUndefined(std::string type) {
    return type == UNDEFINED;
}

std::string BElementType::getTypeValue() {
    if(!m_lexicalToken) {
        throw BException("Cannot get type value of lexical token that was not set");
    }
    return m_lexicalToken->getValue();
}

std::string BElementType::getTypeName() {
    if(!m_lexicalToken) {
        throw BException("Cannot get type name of lexical token that was not set");
    }
    return m_lexicalToken->getName();
}
