#ifndef BASHCLASS_BTOKENUSE_H
#define BASHCLASS_BTOKENUSE_H

#include <bashclass/IBExpression.h>
#include <bashclass/BClass.h>
#include <easycc/LexicalToken.h>
#include <bashclass/BExpressionType.h>

class BTokenUse : public IBExpression {
private:

    // Lexical token corresponding to this instance
    std::shared_ptr<ecc::LexicalToken> m_lexicalToken;

    // Hold the type of the token
    std::shared_ptr<IBType> m_type;
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
    void setLexicalToken(std::shared_ptr<ecc::LexicalToken> lexicalToken);

    /**
     * Get token type
     * @return type
     */
    std::shared_ptr<IBType> getType() { return m_type;}

    /**
     * Cast type
     * @param type
     */
    void castType(std::shared_ptr<BElementType> type);
};


#endif
