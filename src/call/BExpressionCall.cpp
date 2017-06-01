#include <bashclass/BExpressionCall.h>
#include <bashclass/BTypes.h>
#include <iostream>
#include <bashclass/BException.h>
#include <bashclass/BReport.h>

const std::string BExpressionCall::BOOL_LOGICAL_OR = "logical_or";
const std::string BExpressionCall::BOOL_LOGICAL_AND = "logical_and";
const std::string BExpressionCall::BOOL_IS_EQUAL = "is_equal";
const std::string BExpressionCall::BOOL_IS_NOT_EQUAL = "is_not_equal";
const std::string BExpressionCall::BOOL_LESS_THAN = "less_than";
const std::string BExpressionCall::BOOL_GREATER_THAN = "greater_than";
const std::string BExpressionCall::BOOL_LESS_OR_EQUAL = "less_equal_than";
const std::string BExpressionCall::BOOL_GREATER_OR_EQUAL = "greater_equal_than";
const std::string BExpressionCall::BOOL_NOT = "not";

const std::string BExpressionCall::INT_BIT_OR = "bit_or";
const std::string BExpressionCall::INT_BIT_XOR = "bit_xor";
const std::string BExpressionCall::INT_BIT_AND = "bit_and";
const std::string BExpressionCall::INT_LEFT_SHIFT = "left_shift";
const std::string BExpressionCall::INT_RIGHT_SHIFT = "right_shift";
const std::string BExpressionCall::INT_PLUS = "plus";
const std::string BExpressionCall::INT_MINUS = "minus";
const std::string BExpressionCall::INT_MULTIPLY = "multiply";
const std::string BExpressionCall::INT_DIVIDE = "divide";
const std::string BExpressionCall::INT_MOD = "mod";
const std::string BExpressionCall::INT_EXPONENTIAL = "exponential";

std::string BExpressionCall::getTypeValueAsString() {

    // Check if expression is composed of a one operand or two operands
    if(m_rightOperand) {
        std::string leftType = m_leftOperand->getTypeValueAsString();
        std::string rightType = m_rightOperand->getTypeValueAsString();

        // UNDEFINED
        if(leftType == BType::UNDEFINED || rightType == BType::UNDEFINED) {
            return BType::UNDEFINED;
        }

        // ==, !=
        if(m_operatorToken->getName() == BOOL_IS_EQUAL || m_operatorToken->getName() == BOOL_IS_NOT_EQUAL) {
            if(leftType == rightType) {
                return BType::TYPE_VALUE_BOOLEAN;
            }

            BReport::getInstance().error()
                    << "Boolean expression with " << m_operatorToken->getValue()
                    << " operator requires both operand to be of the same type." << std::endl;
            BReport::getInstance().printError();
            return BType::UNDEFINED;
        }

        // ||, &&
        if(m_operatorToken->getName() == BOOL_LOGICAL_OR || m_operatorToken->getName() == BOOL_LOGICAL_AND) {
            if(leftType == rightType && leftType == BType::TYPE_VALUE_BOOLEAN) {
                return BType::TYPE_VALUE_BOOLEAN;
            }

            BReport::getInstance().error()
                    << "Boolean expression with " << m_operatorToken->getValue()
                    << " operator requires both operands to be of boolean type" << std::endl;
            BReport::getInstance().printError();
            return BType::UNDEFINED;
        }

        // <, >, <=, >=
        if(m_operatorToken->getName() == BOOL_LESS_THAN || m_operatorToken->getName() == BOOL_GREATER_THAN
           || m_operatorToken->getName() == BOOL_LESS_OR_EQUAL || m_operatorToken->getName() == BOOL_GREATER_OR_EQUAL) {
            if(leftType == rightType && leftType == BType::TYPE_VALUE_INT) {
                return BType::TYPE_VALUE_BOOLEAN;
            }

            BReport::getInstance().error()
                    << "Boolean expression with " << m_operatorToken->getValue()
                    << " operator requires both operands to be of integer type" << std::endl;
            BReport::getInstance().printError();
            return BType::UNDEFINED;
        }

        // |, ^, &, <<, >>, -, *, /, %, **
        if(m_operatorToken->getName() == INT_BIT_OR || m_operatorToken->getName() == INT_BIT_XOR
           || m_operatorToken->getName() == INT_BIT_AND || m_operatorToken->getName() == INT_LEFT_SHIFT
           || m_operatorToken->getName() == INT_RIGHT_SHIFT || m_operatorToken->getName() == INT_MINUS
           || m_operatorToken->getName() == INT_MULTIPLY || m_operatorToken->getName() == INT_DIVIDE
           || m_operatorToken->getName() == INT_MOD || m_operatorToken->getName() == INT_EXPONENTIAL) {
            if(leftType == rightType && leftType == BType::TYPE_VALUE_INT) {
                return BType::TYPE_VALUE_INT;
            }

            BReport::getInstance().error()
                    << "Integer expression with " << m_operatorToken->getValue()
                    << " operator requires both operands to be of integer type" << std::endl;
            BReport::getInstance().printError();
            return BType::UNDEFINED;
        }

        // +
        if(m_operatorToken->getName() == INT_PLUS) {

            // If left is a string, then it is a string
            if(leftType == BType::TYPE_VALUE_STRING) {
                return BType::TYPE_VALUE_STRING;
            }

            if(leftType == rightType && leftType == BType::TYPE_VALUE_INT) {
                return BType::TYPE_VALUE_INT;
            }

            BReport::getInstance().error()
                    << "Operator " << m_operatorToken->getValue()
                    << " requires both operands to be of integer or left operand to be a string" << std::endl;
            BReport::getInstance().printError();
            return BType::UNDEFINED;
        }

        throw BException("Undefined operator in an expression with two operands");
    } else {
        std::string singleOperandType = m_leftOperand->getTypeValueAsString();

        // UNDEFINED
        if(singleOperandType == BType::UNDEFINED) {
            return BType::UNDEFINED;
        }

        // !
        if(m_operatorToken->getName() == BOOL_NOT) {
            if(singleOperandType == BType::TYPE_VALUE_BOOLEAN) {
                return BType::TYPE_VALUE_BOOLEAN;
            }

            BReport::getInstance().error()
                    << "Operator " << m_operatorToken->getValue()
                    << " operand to be of boolean type" << std::endl;
            BReport::getInstance().printError();
            return BType::UNDEFINED;
        }

        throw BException("Undefined operator in an expression with one operand");
    }
}