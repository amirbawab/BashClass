#include <bashclass/BIf.h>
#include <bashclass/BTypes.h>
#include <iostream>
#include <bashclass/BReport.h>

std::stringstream BIf::getLabel() {
    std::stringstream stream = m_parentScope->getLabel();
    stream << "_if_";
    return stream;
}

void BIf::setExpression(std::shared_ptr<IBCompositeCallable> expression) {

    // Store the condition/expression
    m_expression = expression;

    // Verify the type of the expression is boolean
    std::string expressionType = expression->getTypeValueAsString();
    if(expressionType == BType::UNDEFINED) {
        BReport::getInstance().error()
                << "If statement condition cannot be of undefined type" << std::endl;
        BReport::getInstance().printError();
    } else if(expressionType != BType::TYPE_VALUE_BOOLEAN) {
        BReport::getInstance().error()
                << "An if condition must evaluate to a boolean instead of " << expressionType << std::endl;
        BReport::getInstance().printError();
    }
}
