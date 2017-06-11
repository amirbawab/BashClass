#include <bashclass/BElif.h>
#include <bashclass/BTypes.h>
#include <iostream>
#include <bashclass/BReport.h>
#include <bashclass/BException.h>

std::stringstream BElif::getLabel() {
    std::stringstream stream = m_parentScope->getLabel();
    stream << "_elif_";
    return stream;
}

void BElif::setExpression(std::shared_ptr<IBExpression> expression) {

    // Store the condition/expression
    m_expression = expression;

    // Verify the type of the expression is boolean
    std::string expressionType = expression->getTypeValueAsString();
    if(expressionType == BType::UNDEFINED) {
        BReport::getInstance().error()
                << "Elif statement condition cannot be of undefined type" << std::endl;
        BReport::getInstance().printError();
    } else if(expressionType != BType::TYPE_VALUE_BOOLEAN) {
        BReport::getInstance().error()
                << "An elif condition must evaluate to a boolean instead of " << expressionType << std::endl;
        BReport::getInstance().printError();
    }
}

std::shared_ptr<BFunction> BElif::findClosestFunction() {
    if(!m_parentScope) {
        throw BException("Cannot find closest function for an elif statement with an undefined parent scope");
    }
    return m_parentScope->findClosestFunction();
}

std::shared_ptr<BClass> BElif::findClosestClass() {
    if(!m_parentScope) {
        throw BException("Cannot find closest class for an elif statement with an undefined parent scope");
    }
    return m_parentScope->findClosestClass();
}
