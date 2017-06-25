#ifndef BASHCLASS_BARITHOPERATION_H
#define BASHCLASS_BARITHOPERATION_H

#include <easycc/LexicalToken.h>
#include <bashclass/IBExpression.h>
#include <memory>

class BArithOperation : public IBExpression {
private:

    // Left operand of the expression
    std::shared_ptr<IBExpression> m_leftOperand;

    // Right operand of the expression
    std::shared_ptr<IBExpression> m_rightOperand;

    // Operator of the expression
    std::shared_ptr<ecc::LexicalToken> m_operatorToken;

    // Type of this arithmetic operation
    std::shared_ptr<IBType> m_type;

    /**
     * Evaluate arith operation with two operands
     * @return arith operation type
     */
    std::shared_ptr<IBType> _evaluateTwoOperands();

    /**
     * Evaluate arith operation with right operand only
     * @return arith operation type
     */
    std::shared_ptr<IBType> _evaluateRightOperand();

public:

    // Operators names
    static const std::string OP_LOGICAL_OR;
    static const std::string OP_LOGICAL_AND;
    static const std::string OP_IS_EQUAL;
    static const std::string OP_IS_NOT_EQUAL;
    static const std::string OP_LESS_THAN;
    static const std::string OP_GREATER_THAN;
    static const std::string OP_LESS_OR_EQUAL;
    static const std::string OP_GREATER_OR_EQUAL;
    static const std::string OP_NOT;
    static const std::string OP_BIT_OR;
    static const std::string OP_BIT_XOR;
    static const std::string OP_BIT_AND;
    static const std::string OP_LEFT_SHIFT;
    static const std::string OP_RIGHT_SHIFT;
    static const std::string OP_MINUS;
    static const std::string OP_MULTIPLY;
    static const std::string OP_DIVIDE;
    static const std::string OP_MOD;
    static const std::string OP_EXPONENTIAL;
    static const std::string OP_ASSIGN;
    static const std::string OP_PLUS;

    /**
     * Set left operator
     * @param leftOperand
     */
    void setLeftOperand(std::shared_ptr<IBExpression> leftOperand) { m_leftOperand = leftOperand;}

    /**
     * Get left operator
     * @return  leftOperator
     */
    std::shared_ptr<IBExpression> getLeftOperand() const {return m_leftOperand;}

    /**
     * Set right operand
     * @param rightOperand
     */
    void setRightOperand(std::shared_ptr<IBExpression> rightOperand) {m_rightOperand = rightOperand;}

    /**
     * Get right operand
     * @return rightOpeartor
     */
    std::shared_ptr<IBExpression> getRightOperand() const {return m_rightOperand;}

    /**
     * Set operator
     * @param operatorToken
     */
    void setOperator(std::shared_ptr<ecc::LexicalToken> operatorToken) {m_operatorToken = operatorToken;}

    /**
     * Get operator
     * @return operator
     */
    std::shared_ptr<ecc::LexicalToken> getOperator() {return m_operatorToken;}

    /**
     * Get type
     * @return type
     */
    std::shared_ptr<IBType> getType() { return m_type; }

    /**
     * Evaluate arithmetic operation
     */
    void evaluate();

    /**
     * Cast type
     * @param type
     */
    void castType(std::shared_ptr<BElementType> type);
};

#endif
