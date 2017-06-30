#include <bashclass/BArrayUse.h>

BArrayUse::BArrayUse() {
    m_type = std::make_shared<BElementType>();
}

void BArrayUse::setTypeLexicalToken(std::shared_ptr<ecc::LexicalToken> lexicalToken) {

    // Set the type lexical token
    m_type->setLexicalToken(lexicalToken);

    // Link type
    m_type->linkType();
}

void BArrayUse::castType(std::shared_ptr<BElementType> type) {
    m_type->cast(type);
}