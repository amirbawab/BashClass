#ifndef BASHCLASS_BTHISCALL_H
#define BASHCLASS_BTHISCALL_H

#include <bashclass/IBCallable.h>
#include <bashclass/BClass.h>
#include <easycc/LexicalToken.h>
#include <memory>

class BThisCall : public IBCallable {
private:

    // The class that this reference points to
    std::shared_ptr<BClass> m_reference;

    // Hold the lexical token for this reference
    std::shared_ptr<ecc::LexicalToken> m_lexicalToken;
public:

    /**
     * Get type of this
     * @return string type
     */
    std::string getTypeValueAsString();

    /**
     * Set lexical token for this reference
     * @param lexicalToken
     */
    void setLexicalToken(std::shared_ptr<ecc::LexicalToken> lexicalToken) { m_lexicalToken = lexicalToken;}

    /**
     * Get lexical token of this reference
     * @return lexicalToken
     */
    std::shared_ptr<ecc::LexicalToken> getLexicalToken() {return m_lexicalToken;}

    /**
     * Get reference class
     * @return reference
     */
    std::shared_ptr<BClass> getReference() {return m_reference;}

    /**
     * Set reference class
     * @param reference
     */
    void setReference(std::shared_ptr<BClass> reference) {m_reference = reference;}
};

#endif
