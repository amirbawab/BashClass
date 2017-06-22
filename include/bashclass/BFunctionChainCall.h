#ifndef BASHCLASS_BFUNCTIONCHAINCALL_H
#define BASHCLASS_BFUNCTIONCHAINCALL_H

#include <bashclass/BFunction.h>
#include <bashclass/IBChainable.h>
#include <bashclass/IBExpression.h>
#include <memory>
#include <vector>

class BFunctionChainCall : public IBChainable {
private:

    // The function this function chain call refers to.
    // If this variable is not set, then the actual function
    // was not found
    std::shared_ptr<BFunction> m_function;

    // Arguments passed to this function chain call
    std::vector<std::shared_ptr<IBExpression>> m_arguments;

    // Lexical token for the function chain call.
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
     * Add argument
     * @param argument
     */
    void addArgument(std::shared_ptr<IBExpression> argument);

    /**
     * Get arguments
     * @return arguments
     */
    std::vector<std::shared_ptr<IBExpression>> getArguments() const { return m_arguments;};

    /**
     * Argument number should match the number of parameters
     */
    void verifyArguments();

    /**
     * Check if reference found
     * @return true if found
     */
    bool isFound();

    /**
     * Get type
     * @return type
     */
    std::shared_ptr<IBType> getType();
};

#endif
