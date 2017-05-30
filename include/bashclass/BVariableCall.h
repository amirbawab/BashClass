#ifndef BASHCLASS_BVARIABLECALL_H
#define BASHCLASS_BVARIABLECALL_H

#include <bashclass/BVariable.h>
#include <bashclass/IBCallable.h>
#include <memory>

class BVariableCall : public IBCallable {
private:
    std::shared_ptr<BVariable> m_variable;
    std::shared_ptr<IBCallable> m_expression;
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
    std::shared_ptr<IBCallable> getExpression() {return m_expression;}

    /**
     * Set expression
     * @param expression
     */
    void setExpression(std::shared_ptr<IBCallable> expression) {m_expression = expression;}

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
};


#endif
