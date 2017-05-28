#ifndef BASHCLASS_BTOKENCALL_H
#define BASHCLASS_BTOKENCALL_H

#include <bashclass/IBCallable.h>
#include <easycc/LexicalToken.h>
#include <memory>

class BTokenCall : public IBCallable {
private:
    std::shared_ptr<ecc::LexicalToken> m_lexicalToken;
public:

    /**
     * Get lexical token
     * @return  lexical token
     */
    std::shared_ptr<ecc::LexicalToken> getLexicalToken() const {return m_lexicalToken;}

    /**
     * Set lexical token
     * @param lexicalToken
     */
    void setLexicalToken(std::shared_ptr<ecc::LexicalToken> lexicalToken) { m_lexicalToken = lexicalToken;}
};


#endif
