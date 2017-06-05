#ifndef BASHCLASS_BVARIABLECHAINCALL_H
#define BASHCLASS_BVARIABLECHAINCALL_H

#include <bashclass/BVariable.h>
#include <bashclass/IBSimpleCallable.h>
#include <bashclass/IBCompositeCallable.h>
#include <memory>

class BVariableChainCall : public IBSimpleCallable {
private:

    // The variable this variable call refers to.
    // If this variable is not set, then the actual variable
    // was not found
    std::shared_ptr<BVariable> m_variable;

    // Expression assigned to this variable call
    std::shared_ptr<IBCompositeCallable> m_expression;

    // Lexical token for the variable call.
    // The value of this token and the actual variable
    // should always match
    std::shared_ptr<ecc::LexicalToken> m_lexicalToken;
public:

    /**
     * Get variable
     * @return  variable
     */
    std::shared_ptr<BVariable> getVariable() { return m_variable;}

    /**
     * Set variable
     * @param variable
     */
    void setVariable(std::shared_ptr<BVariable> variable) {m_variable = variable;}

    /**
     * Get expression
     * @return expression
     */
    std::shared_ptr<IBCompositeCallable> getExpression() {return m_expression;}

    /**
     * Set expression
     * @param expression
     */
    void setExpression(std::shared_ptr<IBCompositeCallable> expression);

    /**
     * Get type of the variable
     * @return
     */
    std::string getTypeValueAsString();

    /**
     * Set lexical token
     * @param lexicalToken
     */
    void setLexicalToken(std::shared_ptr<ecc::LexicalToken> lexicalToken) { m_lexicalToken = lexicalToken;}

    /**
     * Get lexical token
     * @return lexical token
     */
    std::shared_ptr<ecc::LexicalToken> getLexicalToken() {return m_lexicalToken;}

    /**
     * Check if the actual variable exists
     * @return true if the actual variable exists
     */
    bool isKnown();

    /**
     * Get type of the actual variable
     * @return type of the actual variable
     */
    std::shared_ptr<BClass> getTypeScope();
};


#endif
