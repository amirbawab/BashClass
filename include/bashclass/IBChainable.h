#ifndef BASHCLASS_IBCHAINABLE_H
#define BASHCLASS_IBCHAINABLE_H

#include <memory>
#include <string>
#include <bashclass/BClass.h>
#include <bashclass/IBType.h>

class IBChainable {
protected:

    // Hold the integer indices
    std::vector<std::shared_ptr<IBExpression>> m_indices;

    // Lexical token for the chainable item
    std::shared_ptr<ecc::LexicalToken> m_lexicalToken;
public:

    /**
     * Check if the element is known
     * @return true if the element is recognized
     */
    virtual bool isFound()=0;

    /**
     * Get the type of the chainable element
     * @return type
     */
    virtual std::shared_ptr<IBType> getType()=0;

    /**
     * Add index
     * @param expression
     */
    void addIndex(std::shared_ptr<IBExpression> expression);

    /**
     * Get lexical token
     * @return lexicalToken
     */
    std::shared_ptr<ecc::LexicalToken> getLexicalToken() {return m_lexicalToken;}

    /**
     * Set lexical token
     * @param lexicalToken
     */
    void setLexicalToken(std::shared_ptr<ecc::LexicalToken> lexicalToken) { m_lexicalToken = lexicalToken;}

    /**
     * Get indices
     * @return indices
     */
    std::vector<std::shared_ptr<IBExpression>> getIndices() const { return m_indices;}
};

#endif
