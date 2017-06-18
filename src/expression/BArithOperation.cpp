#include <bashclass/BArithOperation.h>
#include <bashclass/BTypes.h>
#include <iostream>
#include <bashclass/BException.h>
#include <bashclass/BReport.h>
#include <bashclass/BVariableAccess.h>

const std::string BArithOperation::OP_LOGICAL_OR = "logical_or";
const std::string BArithOperation::OP_LOGICAL_AND = "logical_and";
const std::string BArithOperation::OP_IS_EQUAL = "is_equal";
const std::string BArithOperation::OP_IS_NOT_EQUAL = "is_not_equal";
const std::string BArithOperation::OP_LESS_THAN = "less_than";
const std::string BArithOperation::OP_GREATER_THAN = "greater_than";
const std::string BArithOperation::OP_LESS_OR_EQUAL = "less_equal_than";
const std::string BArithOperation::OP_GREATER_OR_EQUAL = "greater_equal_than";
const std::string BArithOperation::OP_NOT = "not";
const std::string BArithOperation::OP_BIT_OR = "bit_or";
const std::string BArithOperation::OP_BIT_XOR = "bit_xor";
const std::string BArithOperation::OP_BIT_AND = "bit_and";
const std::string BArithOperation::OP_LEFT_SHIFT = "left_shift";
const std::string BArithOperation::OP_RIGHT_SHIFT = "right_shift";
const std::string BArithOperation::OP_MINUS = "minus";
const std::string BArithOperation::OP_MULTIPLY = "multiply";
const std::string BArithOperation::OP_DIVIDE = "divide";
const std::string BArithOperation::OP_MOD = "mod";
const std::string BArithOperation::OP_EXPONENTIAL = "exponential";
const std::string BArithOperation::OP_PLUS = "plus";
const std::string BArithOperation::OP_ASSIGN = "assign";


std::string BArithOperation::_evaluateRightOperand() {
    std::string singleOperandType = m_rightOperand->getTypeValueAsString();

    // UNDEFINED
    if(BType::isUndefined(singleOperandType)) {
        return BType::UNDEFINED;
    }

    // !
    if(m_operatorToken->getName() == OP_NOT) {
        if(singleOperandType == BType::TYPE_VALUE_BOOLEAN) {
            return BType::TYPE_VALUE_BOOLEAN;
        }

        BReport::getInstance().error()
                << "Operator " << m_operatorToken->getValue()
                << " expects operand to be a boolean" << std::endl;
        BReport::getInstance().printError();
        return BType::UNDEFINED;
    }

    // +, -
    if(m_operatorToken->getName() == OP_PLUS || m_operatorToken->getName() == OP_MINUS) {
        if(singleOperandType == BType::TYPE_VALUE_INT) {
            return BType::TYPE_VALUE_INT;
        }

        BReport::getInstance().error()
                << "Operator " << m_operatorToken->getValue()
                << " expects operand to be an integer" << std::endl;
        BReport::getInstance().printError();
        return BType::UNDEFINED;
    }

    throw BException("Undefined operator in an expression with one operand");
}

std::string BArithOperation::_evaluateTwoOperands() {
    std::string leftType = m_leftOperand->getTypeValueAsString();
    std::string rightType = m_rightOperand->getTypeValueAsString();

    // UNDEFINED
    if(BType::isUndefined(leftType) || BType::isUndefined(rightType)) {
        return BType::UNDEFINED;
    }

    // ==, !=
    if(m_operatorToken->getName() == OP_IS_EQUAL || m_operatorToken->getName() == OP_IS_NOT_EQUAL) {
        if(leftType == rightType
           || (BType::isUserDefinedType(leftType) && rightType == BType::NULL_VALUE)
           || (BType::isUserDefinedType(rightType) && leftType == BType::NULL_VALUE)) {
            return BType::TYPE_VALUE_BOOLEAN;
        }

        BReport::getInstance().error()
                << "Boolean expression with " << m_operatorToken->getValue()
                << " operator requires both operand to be of the same type." << std::endl;
        BReport::getInstance().printError();
        return BType::UNDEFINED;
    }

    // ||, &&
    if(m_operatorToken->getName() == OP_LOGICAL_OR || m_operatorToken->getName() == OP_LOGICAL_AND) {
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
    if(m_operatorToken->getName() == OP_LESS_THAN || m_operatorToken->getName() == OP_GREATER_THAN
       || m_operatorToken->getName() == OP_LESS_OR_EQUAL || m_operatorToken->getName() == OP_GREATER_OR_EQUAL) {
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
    if(m_operatorToken->getName() == OP_BIT_OR || m_operatorToken->getName() == OP_BIT_XOR
       || m_operatorToken->getName() == OP_BIT_AND || m_operatorToken->getName() == OP_LEFT_SHIFT
       || m_operatorToken->getName() == OP_RIGHT_SHIFT || m_operatorToken->getName() == OP_MINUS
       || m_operatorToken->getName() == OP_MULTIPLY || m_operatorToken->getName() == OP_DIVIDE
       || m_operatorToken->getName() == OP_MOD || m_operatorToken->getName() == OP_EXPONENTIAL) {
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
    if(m_operatorToken->getName() == OP_PLUS) {

        // If left or right is a string, then it is a string
        if(leftType == BType::TYPE_VALUE_STRING || rightType == BType::TYPE_VALUE_STRING) {
            return BType::TYPE_VALUE_STRING;
        }

        if(leftType == rightType && leftType == BType::TYPE_VALUE_INT) {
            return BType::TYPE_VALUE_INT;
        }

        BReport::getInstance().error()
                << "Operator " << m_operatorToken->getValue()
                << " requires both operands to be of integer or one operand to be a string" << std::endl;
        BReport::getInstance().printError();
        return BType::UNDEFINED;
    }

    // =
    if(m_operatorToken->getName() == OP_ASSIGN) {

        // Left hand side must be a variable access
        auto variableAccessCast = std::dynamic_pointer_cast<BVariableAccess>(m_leftOperand);
        if(variableAccessCast) {

            if(!variableAccessCast->last()->getVariable()) {
                throw BException("Cannot evaluate assignment type of an undefined variable access");
            }

            // Check if type is compatible
            if(leftType == BType::TYPE_VALUE_ANY
               || (BType::isUserDefinedType(variableAccessCast->last()->getVariable()->getType()->getName())
                   &&  rightType == BType::NULL_VALUE)
               || leftType == rightType) {
                return leftType;
            }

            BReport::getInstance().error()
                    << "Variable " << variableAccessCast->last()->getVariable()->getName()->getValue()
                    << " expects an expression of type "
                    << leftType << " but given " << rightType << std::endl;
            BReport::getInstance().printError();
            return BType::UNDEFINED;
        }

        BReport::getInstance().error()
                << "Operator " << m_operatorToken->getValue()
                << " requires left operand to be a variable" << std::endl;
        BReport::getInstance().printError();
        return BType::UNDEFINED;
    }
    throw BException("Undefined operator in an expression with two operands");
}

void BArithOperation::evaluate() {

    // Check if expression is composed of a one operand or two operands
    if(m_leftOperand && m_rightOperand) {
        m_typeValue = _evaluateTwoOperands();
    } else if(m_rightOperand) {
        m_typeValue = _evaluateRightOperand();
    } else {
        throw BException("Arithmetic operation is not composed correctly");
    }
}