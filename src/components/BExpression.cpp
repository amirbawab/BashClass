#include <bashclass/BExpression.h>

void BExpression::addOperand(std::vector<std::shared_ptr<IBCallable>> op)  {
    m_operandStack.push_back(op);
}

void BExpression::addOperator(std::shared_ptr<ecc::LexicalToken> op)  {
    m_operatorStack.push_back(op);
}

void BExpression::evaluate() {
    std::shared_ptr<ecc::LexicalToken> dominantType = nullptr;
    std::shared_ptr<BScope> typeScope = nullptr;
    if(m_operandStack.empty()) {
        m_valid = false;
    } else {
        dominantType = m_operandStack.back().back()->getType();
    }

    // Set type
    m_dominantType = dominantType;
}