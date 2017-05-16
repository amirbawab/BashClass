#ifndef BASHCLASS_BCALLABLETOKEN_H
#define BASHCLASS_BCALLABLETOKEN_H

#include <memory>
#include <bashclass/IBCallable.h>
#include <easycc/LexicalToken.h>

class BCallableToken : public IBCallable {
private:
    std::shared_ptr<ecc::LexicalToken> m_lexicalToken;
public:

    /**
     * Check for built-in types
     */
    std::string getType() const;

    /**
     * Tokens don't have type scope
     * @return nullptr
     */
    std::shared_ptr<BScope> getTypeScope() const {return nullptr;};

    /**
     * Get lexical token
     */
    std::shared_ptr<ecc::LexicalToken> getLexicalToken() { return m_lexicalToken;}

    /**
     * Set lexical token
     */
    void setLexicalToken(std::shared_ptr<ecc::LexicalToken> lexicalToken) {m_lexicalToken = lexicalToken;}
};

#endif
