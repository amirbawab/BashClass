#include <bashclass/BFor.h>
#include <bashclass/BException.h>
#include <bashclass/BReport.h>

std::stringstream BFor::getLabel() {
    std::stringstream stream = m_parentScope->getLabel();
    stream << "_for_";
    return stream;
}

void BFor::setCondition(std::shared_ptr<IBExpression> expression) {
    // Set condition anw
    m_cond = expression;

    // Condition type must be boolean
    if(!expression->getType()->isBoolean()) {
        BReport::getInstance().error()
                << "A for statement condition should evaluate to a boolean" << std::endl;
        BReport::getInstance().printError();
    }
}