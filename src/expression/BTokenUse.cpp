#include <bashclass/BTokenUse.h>
#include <bashclass/BElementType.h>
#include <bashclass/BException.h>

std::string BTokenUse::getTypeValueAsString() {
    if(m_lexicalToken->getName() == BElementType::DATA_TYPE_NAME_INT) {
        return BElementType::TYPE_VALUE_INT;
    } else if(m_lexicalToken->getName() == BElementType::DATA_TYPE_NAME_STRING
              || m_lexicalToken->getName() == BElementType::DATA_TYPE_NAME_BASH_SUB) {
        return BElementType::TYPE_VALUE_STRING;
    } else if(m_lexicalToken->getName() == BElementType::DATA_TYPE_NAME_CHAR) {
        return BElementType::TYPE_VALUE_CHAR;
    } else if(m_lexicalToken->getName() == BElementType::DATA_TYPE_NAME_BOOLEAN) {
        return BElementType::TYPE_VALUE_BOOLEAN;
    } else if(m_lexicalToken->getName() == BElementType::NULL_VALUE) {
        return BElementType::NULL_VALUE;
    }
    throw BException("Token type value cannot be undefined");
}
