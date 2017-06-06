#ifndef BASHCLASS_BFUNCTIONCHAINCALL_H
#define BASHCLASS_BFUNCTIONCHAINCALL_H

#include <bashclass/BFunction.h>
#include <bashclass/IBChainable.h>
#include <bashclass/IBExpression.h>
#include <memory>
#include <vector>

class BFunctionChainCall : public IBChainable {
private:

    // The function this function call refers to.
    // If this variable is not set, then the actual function
    // was not found
    std::shared_ptr<BFunction> m_function;

    // Arguments passed to this function call
    std::vector<std::shared_ptr<IBExpression>> m_arguments;

    // Lexical token for the function call.
    // The value of this token and the actual function
    // should always match
    std::shared_ptr<ecc::LexicalToken> m_lexicalToken;
public:

    /**
     * Get function
     * @return function
     */
    std::shared_ptr<BFunction> getFunction() {return m_function;}

    /**
     * Set function
     * @param function
     */
    void setFunction(std::shared_ptr<BFunction> function) {m_function = function;}

    /**
     * Add argument
     * @param argument
     */
    void addArgument(std::shared_ptr<IBExpression> argument);

    /**
     * Get function type
     * @return function type
     */
    std::string getTypeValueAsString();

    /**
     * Set lexical token
     * @param lexicalToken
     */
    void setLexicalToken(std::shared_ptr<ecc::LexicalToken> lexicalToken) {m_lexicalToken = lexicalToken;}

    /**
     * Get lexical token
     * @return lexical token
     */
    std::shared_ptr<ecc::LexicalToken> getLexicalToken() {return m_lexicalToken;}

    /**
     * Argument number should match the number of parameters
     */
    void verifyArguments();

    /**
     * Check if the actual function exists
     * @return true if the actual function exists
     */
    bool isKnown();

    /**
     * Get type of the actual function
     * @return type of the actual function
     */
    std::shared_ptr<BClass> getTypeScope();
};

#endif
