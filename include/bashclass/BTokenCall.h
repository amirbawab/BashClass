#ifndef BASHCLASS_BTOKENCALL_H
#define BASHCLASS_BTOKENCALL_H

#include <bashclass/IBSimpleCallable.h>
#include <bashclass/BClass.h>
#include <easycc/LexicalToken.h>
#include <memory>

class BTokenCall : public IBSimpleCallable {
private:

    // Lexical token for the variable call.
    // The value of this token and the actual function
    // should always match
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

    /**
     * Get the type of token
     * @return  BType::TYPE_VALUE_INT
     *          BType::TYPE_VALUE_STRING
     *          BType::TYPE_VALUE_CHAR
     *          BType::TYPE_VALUE_BOOLEAN
     *          BType::UNDEFINED
     */
    std::string getTypeValueAsString();

    /**
     * A token call is always known because it does not refer to
     * any variable or other scope
     * e.g. 1, true, 'a', etc ...
     * @return true
     */
    bool isKnown();

    /**
     * A token call does not have any type scope
     */
    std::shared_ptr<BClass> getTypeScope();
};


#endif
