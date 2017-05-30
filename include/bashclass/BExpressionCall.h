#ifndef BASHCLASS_BEXPRESSIONCALL_H
#define BASHCLASS_BEXPRESSIONCALL_H

#include <bashclass/IBCallable.h>
#include <easycc/LexicalToken.h>
#include <memory>

class BExpressionCall : public IBCallable {
private:
    std::shared_ptr<IBCallable> m_leftOperand;
    std::shared_ptr<IBCallable> m_rightOperand;
    std::shared_ptr<ecc::LexicalToken> m_operatorToken;

    // Boolean expression
    static const std::string BOOL_LOGICAL_OR;
    static const std::string BOOL_LOGICAL_AND;
    static const std::string BOOL_IS_EQUAL;
    static const std::string BOOL_IS_NOT_EQUAL;
    static const std::string BOOL_LESS_THAN;
    static const std::string BOOL_GREATER_THAN;
    static const std::string BOOL_LESS_OR_EQUAL;
    static const std::string BOOL_GREATER_OR_EQUAL;
    static const std::string BOOL_NOT;

    // Integer expression
    static const std::string INT_BIT_OR;
    static const std::string INT_BIT_XOR;
    static const std::string INT_BIT_AND;
    static const std::string INT_LEFT_SHIFT;
    static const std::string INT_RIGHT_SHIFT;
    static const std::string INT_PLUS;
    static const std::string INT_MINUS;
    static const std::string INT_MULTIPLY;
    static const std::string INT_DIVIDE;
    static const std::string INT_MOD;
    static const std::string INT_EXPONENTIAL;

public:

    /**
     * Set left operator
     * @param leftOperand
     */
    void setLeftOperand(std::shared_ptr<IBCallable> leftOperand) { m_leftOperand = leftOperand;}

    /**
     * Get left operator
     * @return  leftOperator
     */
    std::shared_ptr<IBCallable> getLeftOperand() const {return m_leftOperand;}

    /**
     * Set right operand
     * @param rightOperand
     */
    void setRightOperand(std::shared_ptr<IBCallable> rightOperand) {m_rightOperand = rightOperand;}

    /**
     * Get right operand
     * @return rightOpeartor
     */
    std::shared_ptr<IBCallable> getRightOperand() const {return m_rightOperand;}

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
     * Get expression type
     * @return expression type
     */
    std::string getTypeValueAsString();
};

#endif
