#ifndef BASHCLASS_BTOKENUSE_H
#define BASHCLASS_BTOKENUSE_H

#include <bashclass/IBExpression.h>
#include <bashclass/BClass.h>
#include <easycc/LexicalToken.h>

class BTokenUse : public IBExpression {
private:

    // Lexical token corresponding to this instance
    std::shared_ptr<ecc::LexicalToken> m_lexicalToken;
public:

    /**
     * Get lexical token
     * @return lexical token
     */
    std::shared_ptr<ecc::LexicalToken> getLexicalToken() const {return m_lexicalToken;}

    /**
     * Set lexical token
     * @param lexicalToken
     */
    void setLexicalToken(std::shared_ptr<ecc::LexicalToken> lexicalToken) { m_lexicalToken = lexicalToken;}

    /**
     * Get the type of token
     * @return  BType::TYPE_VALUE_INT
     *          BType::TYPE_VALUE_STRING
     *          BType::TYPE_VALUE_CHAR
     *          BType::TYPE_VALUE_BOOLEAN
     *          BType::UNDEFINED
     */
    std::string getTypeValueAsString();
};


#endif
