#ifndef BASHCLASS_BELEMENTTYPE_H
#define BASHCLASS_BELEMENTTYPE_H

#include <easycc/LexicalToken.h>
#include <bashclass/BExpressionType.h>

class BElementType : public IBType {
private:

    // Hold lexical token
    std::shared_ptr<ecc::LexicalToken> m_lexicalToken;
public:

    /**
     * Get lexical ltoken
     * @return lexical token
     */
    std::shared_ptr<ecc::LexicalToken> getLexicalToken() {return m_lexicalToken;}

    /**
     * Set lexical token
     * @param lexicalToken
     */
    void setLexicalToken(std::shared_ptr<ecc::LexicalToken> lexicalToken) { m_lexicalToken = lexicalToken; }

    /**
     * Get type name
     * @return type name
     */
    std::string getTypeName();

    /**
     * Get type value
     * @return type value
     */
    std::string getTypeValue();

    /**
     * Link type
     */
    void linkType();

    /**
     * Create a clone of this element type into expression type
     * @return expression type
     */
    std::shared_ptr<BExpressionType> cloneToExpressionType();

    /**
     * Cast type
     * @param type
     */
    void cast(std::shared_ptr<BElementType> type);
};
#endif
