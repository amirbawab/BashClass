#include <bashclass/BArithOperation.h>
#include <iostream>
#include <bashclass/BException.h>
#include <bashclass/BReport.h>
#include <bashclass/BVariableAccess.h>
#include <bashclass/BTypeFactory.h>

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


std::shared_ptr<IBType> BArithOperation::_evaluateRightOperand() {
    std::shared_ptr<IBType> singleOperandType = m_rightOperand->getType();

    // UNDEFINED
    if(singleOperandType->isUndefined()) {
        return singleOperandType;
    }

    // !
    if(m_operatorToken->getName() == OP_NOT) {
        if(singleOperandType->isBoolean() && !singleOperandType->isArray()) {
            return singleOperandType;
        }

        BReport::getInstance().error()
                << "Operator " << m_operatorToken->getValue()
                << " expects operand to be a boolean" << std::endl;
        BReport::getInstance().printError();
        return BTypeFactory::createUndefinedExpressionType();
    }

    // +, -
    if(m_operatorToken->getName() == OP_PLUS || m_operatorToken->getName() == OP_MINUS) {
        if(singleOperandType->isInt() && !singleOperandType->isArray()) {
            return singleOperandType;
        }

        BReport::getInstance().error()
                << "Operator " << m_operatorToken->getValue()
                << " expects operand to be an integer" << std::endl;
        BReport::getInstance().printError();
        return BTypeFactory::createUndefinedExpressionType();
    }

    throw BException("Undefined operator in an expression with one operand");
}

std::shared_ptr<IBType> BArithOperation::_evaluateTwoOperands() {
    std::shared_ptr<IBType> leftType = m_leftOperand->getType();
    std::shared_ptr<IBType> rightType = m_rightOperand->getType();

    // UNDEFINED
    if(leftType->isUndefined() || rightType->isUndefined()) {
        return leftType;
    }

    // ==, !=
    if(m_operatorToken->getName() == OP_IS_EQUAL || m_operatorToken->getName() == OP_IS_NOT_EQUAL) {
        if((leftType->getTypeValue() == rightType->getTypeValue()
            && leftType->getDimension() == rightType->getDimension())
           || (leftType->isArray() && rightType->isNull())
           || (rightType->isArray() && leftType->isNull())
           || (leftType->isIdentifier() && rightType->isNull())
           || (rightType->isIdentifier() && leftType->isNull())) {
            return BTypeFactory::createBooleanExpressionType();
        }

        BReport::getInstance().error()
                << "Boolean expression with " << m_operatorToken->getValue()
                << " operator requires both operand to be of the same type." << std::endl;
        BReport::getInstance().printError();
        return BTypeFactory::createUndefinedExpressionType();
    }

    // ||, &&
    if(m_operatorToken->getName() == OP_LOGICAL_OR || m_operatorToken->getName() == OP_LOGICAL_AND) {
        if(leftType->isBoolean() && rightType->isBoolean() && !leftType->isArray() && !rightType->isArray()) {
            return leftType;
        }

        BReport::getInstance().error()
                << "Boolean expression with " << m_operatorToken->getValue()
                << " operator requires both operands to be of boolean type" << std::endl;
        BReport::getInstance().printError();
        return BTypeFactory::createUndefinedExpressionType();
    }

    // <, >, <=, >=
    if(m_operatorToken->getName() == OP_LESS_THAN || m_operatorToken->getName() == OP_GREATER_THAN
       || m_operatorToken->getName() == OP_LESS_OR_EQUAL || m_operatorToken->getName() == OP_GREATER_OR_EQUAL) {

        if(leftType->isInt() && rightType->isInt() && !leftType->isArray() && !rightType->isArray()) {
            return BTypeFactory::createBooleanExpressionType();
        }

        BReport::getInstance().error()
                << "Boolean expression with " << m_operatorToken->getValue()
                << " operator requires both operands to be of integer type" << std::endl;
        BReport::getInstance().printError();
        return BTypeFactory::createUndefinedExpressionType();
    }

    // |, ^, &, <<, >>, -, *, /, %, **
    if(m_operatorToken->getName() == OP_BIT_OR || m_operatorToken->getName() == OP_BIT_XOR
       || m_operatorToken->getName() == OP_BIT_AND || m_operatorToken->getName() == OP_LEFT_SHIFT
       || m_operatorToken->getName() == OP_RIGHT_SHIFT || m_operatorToken->getName() == OP_MINUS
       || m_operatorToken->getName() == OP_MULTIPLY || m_operatorToken->getName() == OP_DIVIDE
       || m_operatorToken->getName() == OP_MOD || m_operatorToken->getName() == OP_EXPONENTIAL) {

        if(leftType->isInt() && rightType->isInt() && !leftType->isArray() && !rightType->isArray()) {
            return leftType;
        }

        BReport::getInstance().error()
                << "Integer expression with " << m_operatorToken->getValue()
                << " operator requires both operands to be of integer type" << std::endl;
        BReport::getInstance().printError();
        return BTypeFactory::createUndefinedExpressionType();
    }

    // +
    if(m_operatorToken->getName() == OP_PLUS) {

        if(leftType->isInt() && rightType->isInt() && !leftType->isArray() && !rightType->isArray()) {
            return leftType;
        }

        BReport::getInstance().error()
                << "Operator " << m_operatorToken->getValue()
                << " requires both operands to be integers" << std::endl;
        BReport::getInstance().printError();
        return BTypeFactory::createUndefinedExpressionType();
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
            if(leftType->isCompatible(rightType)) {
                return leftType;
            }

            BReport::getInstance().error()
                    << "Variable " << variableAccessCast->last()->getVariable()->getName()->getValue()
                    << " expects an expression of type "
                    << leftType->toString() << " but given " << rightType->toString() << std::endl;
            BReport::getInstance().printError();
            return BTypeFactory::createUndefinedExpressionType();
        }

        BReport::getInstance().error()
                << "Operator " << m_operatorToken->getValue()
                << " requires left operand to be a variable" << std::endl;
        BReport::getInstance().printError();
        return BTypeFactory::createUndefinedExpressionType();
    }
    throw BException("Undefined operator in an expression with two operands");
}

void BArithOperation::evaluate() {

    // Check if expression is composed of one operand or two operands
    if(m_leftOperand && m_rightOperand) {
        m_type = _evaluateTwoOperands();
    } else if(m_rightOperand) {
        m_type = _evaluateRightOperand();
    } else {
        throw BException("Arithmetic operation is not composed correctly");
    }
}

void BArithOperation::castType(std::shared_ptr<BElementType> type) {
    m_type->cast(type);
}