#include <bashclass/BVariableCall.h>
#include <bashclass/BTypes.h>
#include <iostream>
#include <bashclass/BReport.h>

std::string BVariableCall::getTypeValueAsString() {
    if(!m_variable || !m_variable->hasKnownType()) {
        return BType::UNDEFINED;
    }
    return m_variable->getType()->getValue();
}

void BVariableCall::setExpression(std::shared_ptr<IBCallable> expression) {

    // Set expression
    m_expression = expression;

    // Variable is required to compare the type of the expression
    if(m_variable) {
        // Compare types
        std::string expressionType = expression->getTypeValueAsString();
        std::string variableType = m_variable->getType()->getValue();
        if(!m_variable->hasKnownType()) {
            BReport::getInstance().error()
                    << "Cannot assign expression to an undefined type for variable "
                    << m_variable->getName()->getValue() << std::endl;
            BReport::getInstance().printError();
        } else if(expressionType == BType::UNDEFINED) {
            BReport::getInstance().error()
                    << "Variable " << m_variable->getName()->getValue()
                    << " cannot be assigned an undefined expression" << std::endl;
            BReport::getInstance().printError();
        } else if(variableType != BType::TYPE_VALUE_ANY && variableType != expressionType) {
            BReport::getInstance().error()
                    << "Variable " << m_variable->getName()->getValue() << " expects an expression of type "
                    << variableType << " but given " << expressionType << std::endl;
            BReport::getInstance().printError();
        }
    } else {
        BReport::getInstance().error()
                << "Expression cannot be evaluated because it is assigned to an undefined variable "
                << m_lexicalToken->getValue() << std::endl;
        BReport::getInstance().printError();
    }
}