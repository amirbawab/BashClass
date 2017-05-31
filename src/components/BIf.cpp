#include <bashclass/BIf.h>
#include <bashclass/BTypes.h>
#include <iostream>

std::stringstream BIf::getLabel() {
    std::stringstream stream = m_parentScope->getLabel();
    stream << "_if_";
    return stream;
}

void BIf::setExpression(std::shared_ptr<IBCallable> expression) {

    std::string expressionType = expression->getTypeValueAsString();
    if(expressionType == BType::UNDEFINED) {
        std::cerr << "If statement condition cannot be of undefined type" << std::endl;
    } else if(expressionType != BType::TYPE_VALUE_BOOLEAN) {
        std::cerr << "An if condition must evaluate to a boolean instead of " << expressionType << std::endl;
    }

    // Store the condition/expression anyway
    m_expression = expression;
}
