#include <bashclass/BTokenUse.h>
#include <bashclass/BTypes.h>
#include <bashclass/BException.h>

std::string BTokenCall::getTypeValueAsString() {
    if(m_lexicalToken->getName() == BType::DATA_TYPE_NAME_INT) {
        return BType::TYPE_VALUE_INT;
    } else if(m_lexicalToken->getName() == BType::DATA_TYPE_NAME_STRING
              || m_lexicalToken->getName() == BType::TYPE_NAME_BASH_SUB) {
        return BType::TYPE_VALUE_STRING;
    } else if(m_lexicalToken->getName() == BType::DATA_TYPE_NAME_CHAR) {
        return BType::TYPE_VALUE_CHAR;
    } else if(m_lexicalToken->getName() == BType::DATA_TYPE_NAME_BOOLEAN) {
        return BType::TYPE_VALUE_BOOLEAN;
    }
    throw BException("Token type value cannot be undefined");
}
