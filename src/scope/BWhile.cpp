#include <bashclass/BWhile.h>
#include <iostream>
#include <bashclass/BReport.h>
#include <bashclass/BException.h>

std::stringstream BWhile::getLabel() {
    std::stringstream stream = m_parentScope->getLabel();
    stream << "_while_";
    return stream;
}

void BWhile::setExpression(std::shared_ptr<IBExpression> expression) {

    // Store the condition/expression
    m_expression = expression;

    // Verify the type of the condition is boolean
    std::shared_ptr<IBType> expressionType = expression->getType();
    if(expressionType->isUndefined()) {
        BReport::getInstance().error()
                << "While statement condition cannot be of undefined type" << std::endl;
        BReport::getInstance().printError();
    } else if(!expressionType->isBoolean()) {
        BReport::getInstance().error()
                << "A while condition must evaluate to a boolean instead of " << expressionType << std::endl;
        BReport::getInstance().printError();
    }
}

std::shared_ptr<BClass> BWhile::findClosestClass() {
    if(!m_parentScope) {
        throw BException("Cannot find closest class for a while statement with an undefined parent scope");
    }
    return m_parentScope->findClosestClass();
}
