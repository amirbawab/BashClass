#include <bashclass/BFor.h>
#include <bashclass/BException.h>
#include <bashclass/BReport.h>

std::stringstream BFor::getLabel() {
    std::stringstream stream = m_parentScope->getLabel();
    stream << "_for_";
    return stream;
}

std::shared_ptr<BFunction> BFor::findClosestFunction() {
    if(!m_parentScope) {
        throw BException("Cannot find closest function for a for statement with an undefined parent scope");
    }
    return m_parentScope->findClosestFunction();
}

std::shared_ptr<BClass> BFor::findClosestClass() {
    if(!m_parentScope) {
        throw BException("Cannot find closest class for a for statement with an undefined parent scope");
    }
    return m_parentScope->findClosestClass();
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