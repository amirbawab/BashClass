#include <bashclass/BCallableToken.h>
#include <bashclass/BTypes.h>

std::string BCallableToken::getTypeValue() const {
    if(m_lexicalToken->getName() == BType::DATA_TYPE_NAME_INT) {
        return BType::TYPE_VALUE_INT;
    } else if(m_lexicalToken->getName() == BType::DATA_TYPE_NAME_STRING
              || m_lexicalToken->getName() == BType::TYPE_NAME_BASH_SUB) {
        return BType::TYPE_VALUE_STRING;
    }
    return BType::UNDEFINED;
}



