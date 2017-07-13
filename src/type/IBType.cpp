#include <bashclass/IBType.h>
#include <bashclass/BElementType.h>

/*********************
 *  TOKEN TYPE NAME
 *********************/
const std::string IBType::TYPE_NAME_INT = "int_type";
const std::string IBType::TYPE_NAME_CHAR = "char_type";
const std::string IBType::TYPE_NAME_BOOLEAN = "boolean_type";
const std::string IBType::TYPE_NAME_VOID = "void_type";
const std::string IBType::TYPE_NAME_IDENTIFIER = "identifier";

/*********************
 *  TOKEN TYPE VALUE
 *********************/
const std::string IBType::TYPE_VALUE_INT = "int";
const std::string IBType::TYPE_VALUE_CHAR = "char";
const std::string IBType::TYPE_VALUE_BOOLEAN = "boolean";
const std::string IBType::TYPE_VALUE_VOID = "void";

/*********************
 *  TOKEN TYPE DATA
 *********************/
const std::string IBType::DATA_TYPE_NAME_INT = "integer";
const std::string IBType::DATA_TYPE_NAME_CHAR = "character";
const std::string IBType::DATA_TYPE_NAME_BOOLEAN = "truefalse";
const std::string IBType::DATA_TYPE_NAME_STRING = "string_literal";
const std::string IBType::DATA_TYPE_NAME_BASH_SUB = "bash_sub";
const std::string IBType::DATA_TYPE_NAME_BASH_SUB_INT = "bash_sub_int";
const std::string IBType::DATA_TYPE_NAME_BASH_INLINE = "bash_inline";
const std::string IBType::DATA_TYPE_NAME_BASH_BLOCK = "bash_block";

/*********************
 * SPECIAL TOKEN TYPE
 *********************/
const std::string IBType::UNDEFINED = "undefined";
const std::string IBType::NULL_VALUE = "null";

bool IBType::isBuiltInType() {
    return isInt() || isVoid() || isBoolean() || isChar();
}

bool IBType::hasKnownType() {
    return isBuiltInType() || (isIdentifier() && m_typeScope);
}

bool IBType::isCompatible(std::shared_ptr<IBType> type) {

    if(type->isUndefined() || type->isVoid()) {
        return false;
    }

    if(isArray() && type->isNull()) {
        return true;
    }

    if(isIdentifier() && type->isNull()) {
        return true;
    }

    if(isIdentifier() && getTypeScope()
       && type->isIdentifier() && type->getTypeScope()
       && type->getTypeScope()->inheritsFrom(getTypeScope())) {
        return true;
    }

    if(getTypeValue() != type->getTypeValue()) {
        return false;
    }

    return getDimension() == type->getDimension();
}

std::string IBType::toString() {
    std::string result = getTypeValue();
    int tmpDim = m_dimension;
    while (tmpDim-- > 0) {
        result += "[]";
    }
    return result;
}

void IBType::cast(std::shared_ptr<BElementType> type) {
    m_dimension = type->getDimension();
    m_typeScope = type->getTypeScope();
}

bool IBType::isSame(std::shared_ptr<IBType> type) {
    return getTypeValue() == type->getTypeValue()
           && getTypeName() == type->getTypeName()
           && getTypeScope() == type->getTypeScope()
           && getDimension() == type->getDimension();
}