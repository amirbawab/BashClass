#include <bashclass/BArrayUse.h>
#include <bashclass/BElementType.h>

BArrayUse::BArrayUse() {
    m_type = std::make_shared<BElementType>();
}

void BArrayUse::setTypeLexicalToken(std::shared_ptr<ecc::LexicalToken> lexicalToken) {
    m_type->setLexicalToken(lexicalToken);
}