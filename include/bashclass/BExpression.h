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
    std::string m_dominantType;
    bool m_valid = false;
public:

    /**
     * Get dominant type token
     */
    std::string getDominantType() {return m_dominantType;}

    /**
     * Evaluate expression and generate final type
     * @return success
     */
    bool evaluate();

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
