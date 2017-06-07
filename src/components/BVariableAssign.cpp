#include <bashclass/BVariableAssign.h>
#include <bashclass/BReport.h>
#include <bashclass/BTypes.h>
#include <bashclass/BException.h>

void BVariableAssign::setExpression(std::shared_ptr<IBExpression> expression) {

    if(!m_variableAccess) {
        throw BException("Cannot assign expression to a variable is not set yet");
    }

    // Set expression
    m_expression = expression;

    // Store the last variable access
    std::shared_ptr<BVariableChainAccess> variableChainAccess = m_variableAccess->last();

    // Variable is required to compare the type of the expression
    if(variableChainAccess->getVariable()) {

        // Compare types
        std::string expressionType = expression->getTypeValueAsString();
        std::string variableType = variableChainAccess->getVariable()->getType()->getValue();
        if(!variableChainAccess->getVariable()->hasKnownType()) {
            BReport::getInstance().error()
                    << "Cannot assign expression to an undefined type for variable "
                    << variableChainAccess->getVariable()->getName()->getValue() << std::endl;
            BReport::getInstance().printError();
        } else if(expressionType == BType::UNDEFINED) {
            BReport::getInstance().error()
                    << "Variable " << variableChainAccess->getVariable()->getName()->getValue()
                    << " cannot be assigned an undefined expression" << std::endl;
            BReport::getInstance().printError();
        } else if(variableType != BType::TYPE_VALUE_ANY && variableType != expressionType) {
            BReport::getInstance().error()
                    << "Variable " << variableChainAccess->getVariable()->getName()->getValue() << " expects an expression of type "
                    << variableType << " but given " << expressionType << std::endl;
            BReport::getInstance().printError();
        }
    } else {
        BReport::getInstance().error()
                << "Expression cannot be evaluated because it is assigned to an undefined variable "
                << variableChainAccess->getLexicalToken()->getValue() << std::endl;
        BReport::getInstance().printError();
    }
}
