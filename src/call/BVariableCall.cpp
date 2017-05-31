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

    if(!m_variable) {
        std::cerr << "Expression cannot be assigned to the undefined variable "
                  << m_lexicalToken->getValue() << std::endl;
        return;
    }

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

    // Set expression
    m_expression = expression;
}