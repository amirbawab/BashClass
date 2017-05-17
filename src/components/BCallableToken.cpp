#include <bashclass/BCallableToken.h>
#include <bashclass/BTypes.h>

const char *LEXICAL_TOKEN_INTEGER = "integer";
const char *LEXICAL_TOKEN_STRING = "string";

std::string BCallableToken::getTypeValue() const {
    if(m_lexicalToken->getName() == LEXICAL_TOKEN_INTEGER) {
        return BType::TYPE_INT;
    } else if(m_lexicalToken->getName() == LEXICAL_TOKEN_STRING) {
        return BType::TYPE_STRING;
    } else {
        return "";
    }
}



