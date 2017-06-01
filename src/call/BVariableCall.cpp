#include <bashclass/BVariableCall.h>
#include <bashclass/BTypes.h>
#include <iostream>

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
            std::cerr << "Cannot assign expression to an undefined type for variable "
                      << m_variable->getName()->getValue() << std::endl;
        } else if(expressionType == BType::UNDEFINED) {
            std::cerr << "Variable " << m_variable->getName()->getValue()
                      << " cannot be assigned an undefined expression" << std::endl;
        } else if(variableType != BType::TYPE_VALUE_ANY && variableType != expressionType) {
            std::cerr << "Variable " << m_variable->getName()->getValue() << " expects an expression of type "
                      << variableType << " but given " << expressionType << std::endl;
        }
    } else {
        std::cerr << "Expression cannot be evaluated because it is assigned to an undefined variable "
                  << m_lexicalToken->getValue() << std::endl;
    }
}