#include <bashclass/BElementType.h>
#include <bashclass/BException.h>

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
