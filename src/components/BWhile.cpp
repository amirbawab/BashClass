#include <bashclass/BWhile.h>
#include <bashclass/BTypes.h>
#include <iostream>
#include <bashclass/BReport.h>

std::stringstream BWhile::getLabel() {
    std::stringstream stream = m_parentScope->getLabel();
    stream << "_while_";
    return stream;
}

void BWhile::setExpression(std::shared_ptr<IBCallable> expression) {

    // Store the condition/expression
    m_expression = expression;

    // Verify the type of the condition is boolean
    std::string expressionType = expression->getTypeValueAsString();
    if(expressionType == BType::UNDEFINED) {
        BReport::getInstance().error()
                << "While statement condition cannot be of undefined type" << std::endl;
        BReport::getInstance().printError();
    } else if(expressionType != BType::TYPE_VALUE_BOOLEAN) {
        BReport::getInstance().error()
                << "A while condition must evaluate to a boolean instead of " << expressionType << std::endl;
        BReport::getInstance().printError();
    }
}
