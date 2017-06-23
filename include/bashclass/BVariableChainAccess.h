#ifndef BASHCLASS_BVARIABLECHAINACCESS_H
#define BASHCLASS_BVARIABLECHAINACCESS_H

#include <bashclass/BVariable.h>
#include <bashclass/IBChainable.h>
#include <bashclass/BExpressionType.h>
#include <memory>

class BVariableChainAccess : public IBChainable {
private:

    // The variable this variable call refers to.
    // If this variable is not set, then the actual variable
    // was not found
    std::shared_ptr<BVariable> m_variable;

    // Lexical token for the variable call.
    // The value of this token and the actual variable
    // should always match
    std::shared_ptr<ecc::LexicalToken> m_lexicalToken;

    // Hold the integer indices
    std::vector<std::shared_ptr<IBExpression>> m_indices;

    // Hold the type of this variable access
    std::shared_ptr<BExpressionType> m_type;
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
    void setVariable(std::shared_ptr<BVariable> variable);

    /**
     * Get lexical token
     * @return lexical token
     */
    std::shared_ptr<ecc::LexicalToken> getLexicalToken() {return m_lexicalToken;}

    /**
     * Set lexical token
     * @param lexicalToken
     */
    void setLexicalToken(std::shared_ptr<ecc::LexicalToken> lexicalToken) { m_lexicalToken = lexicalToken;}

    /**
     * Check if reference is found
     * @return true if found
     */
    bool isFound();

    /**
     * Get variable type
     * @return type
     */
    std::shared_ptr<IBType> getType();

    /**
     * Add index
     * @param expression
     */
    void addIndex(std::shared_ptr<IBExpression> expression);

    /**
     * Get indices
     * @return indices
     */
    std::vector<std::shared_ptr<IBExpression>> getIndices() const { return m_indices;}
};


#endif
