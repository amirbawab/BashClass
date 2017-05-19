#include <bashclass/BExpression.h>
#include <bashclass/BTypes.h>
#include <bashclass/BFunction.h>

void BExpression::addOperand(std::vector<std::shared_ptr<IBCallable>> op)  {
    m_operandStack.push_back(op);
}

void BExpression::addOperator(std::shared_ptr<ecc::LexicalToken> op)  {
    m_operatorStack.push_back(op);
}

bool BExpression::evaluate() {

    // If there are more than one element in the expression,
    // then all of them should be integers
    if(m_operandStack.size() > 1) {
        for(auto operand : m_operandStack) {
            if(!operand.back() || operand.back()->getTypeValue() != BType::TYPE_VALUE_INT) {
                m_valid = false;
                m_dominantType = BType::UNDEFINED;
                return false;
            }
        }
        m_valid = true;
        m_dominantType = BType::TYPE_VALUE_INT;
        return true;
    } else {

        // If the type of the callable element is undefined,
        // then the expression is not valid, but evaluates correctly
        m_dominantType = m_operandStack.back().back()->getTypeValue();
        m_valid = m_dominantType != BType::UNDEFINED;
        return true;
    }
}