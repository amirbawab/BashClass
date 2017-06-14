#include <bashclass/BIf.h>
#include <bashclass/BTypes.h>
#include <iostream>
#include <bashclass/BReport.h>
#include <bashclass/BException.h>
#include <bashclass/BElif.h>
#include <bashclass/BElse.h>

std::stringstream BIf::getLabel() {
    std::stringstream stream = m_parentScope->getLabel();
    stream << "_if_";
    return stream;
}

void BIf::setExpression(std::shared_ptr<IBExpression> expression) {

    // Store the condition/expression
    m_expression = expression;

    // Verify the type of the expression is boolean
    std::string expressionType = expression->getTypeValueAsString();
    if(BType::isUndefined(expressionType)) {
        BReport::getInstance().error()
                << "If statement condition cannot be of undefined type" << std::endl;
        BReport::getInstance().printError();
    } else if(expressionType != BType::TYPE_VALUE_BOOLEAN) {
        BReport::getInstance().error()
                << "An if condition must evaluate to a boolean instead of " << expressionType << std::endl;
        BReport::getInstance().printError();
    }
}

std::shared_ptr<BFunction> BIf::findClosestFunction() {
    if(!m_parentScope) {
        throw BException("Cannot find closest function for an if statement with an undefined parent scope");
    }
    return m_parentScope->findClosestFunction();
}

std::shared_ptr<BClass> BIf::findClosestClass() {
    if(!m_parentScope) {
        throw BException("Cannot find closest class for an if statement with an undefined parent scope");
    }
  return m_parentScope->findClosestClass();
}

void BIf::addElif(std::shared_ptr<BElif> elifScope) {
    m_elifScopes.push_back(elifScope);
    elifScope->setParentIf(std::static_pointer_cast<BIf>(shared_from_this()));
}

void BIf::setElse(std::shared_ptr<BElse> elseScope) {
    m_elseScope = elseScope;
    // TODO Test casting
    elseScope->setParentIf(std::static_pointer_cast<BIf>(shared_from_this()));
}