#ifndef BASHCLASS_BEXPRESSION_H
#define BASHCLASS_BEXPRESSION_H

#include <memory>
#include <vector>
#include <bashclass/BScope.h>
#include <bashclass/IBCallable.h>
#include <easycc/LexicalToken.h>

class BExpression {
private:
    std::vector<std::vector<std::shared_ptr<IBCallable>>> m_operandStack;
    std::vector<std::shared_ptr<ecc::LexicalToken>> m_operatorStack;
    std::string m_type;
    std::shared_ptr<BScope> m_typeScope;
    bool m_valid;
public:

    /**
     * Get type as a string
     */
    std::string getType() {return m_type;}

    /**
     * Get type scope as a pointer
     */
    std::shared_ptr<BScope> getTypeScope() {return m_typeScope;}

    /**
     * Evaluate expression and generate final type
     * @return success
     */
    void evaluate();

    /**
     * Check if the expression is valid
     * A valid expression does not have errors
     */
    bool isValid() {return m_valid;}

    /**
     * Add operand to the expression
     */
    void addOperand(std::vector<std::shared_ptr<IBCallable>> op);

    /**
     * Add operator to the expression
     */
    void addOperator(std::shared_ptr<ecc::LexicalToken> op);
};

#endif
