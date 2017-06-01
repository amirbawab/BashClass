#include <bashclass/BFunctionCall.h>
#include <bashclass/BTypes.h>
#include <iostream>
#include <bashclass/BReport.h>

std::string BFunctionCall::getTypeValueAsString() {
    if(!m_function || !m_function->hasKnowType()) {
        return BType::UNDEFINED;
    }
    return m_function->getType()->getValue();
}

void BFunctionCall::verifyArguments() {

    // Check if the number of arguments is less than the number of parameters
    if(m_function && m_arguments.size() < m_function->findAllParameters().size()) {
        BReport::getInstance().error()
                << "Function call " << m_lexicalToken->getValue()
                << " is missing arguments" << std::endl;
        BReport::getInstance().printError();
    }

    // No need to check if for the number of arguments exceeding the number of parameters because
    // this is already handled by the addArgument() function
}

void BFunctionCall::addArgument(std::shared_ptr<IBCallable> argument) {

    // Check the index of the parameter to be compared with
    size_t paramIndex = m_arguments.size();

    // Add argument
    m_arguments.push_back(argument);

    // To verify the argument added, the function must exist
    if(m_function) {

        // Get function parameters
        auto parameters = m_function->findAllParameters();

        // Arguments number must match function parameters
        if(paramIndex >= parameters.size()) {
            BReport::getInstance().error()
                    << "Argument number " << paramIndex + 1 << " cannot be passed to the function "
                    << m_function->getName()->getValue() << " because the function expects "
                    << parameters.size() << " arguments" << std::endl;
            BReport::getInstance().printError();
        } else {

            std::string argumentType = argument->getTypeValueAsString();
            std::string parameterType = parameters[paramIndex]->getType()->getValue();
            if(!parameters[paramIndex]->hasKnownType()) {
                BReport::getInstance().error()
                        << "Cannot pass argument value to an undefined type for parameter "
                        << parameters[paramIndex]->getName()->getValue() << " in function "
                        << m_function->getName()->getValue() << std::endl;
                BReport::getInstance().printError();
            } else if(argumentType == BType::UNDEFINED) {
                BReport::getInstance().error()
                        << "Parameter " << parameters[paramIndex]->getName()->getValue()
                        << " in function " << m_function->getName()->getValue()
                        << " is given an undefined argument" << std::endl;
                BReport::getInstance().printError();
            } else if(parameterType != BType::TYPE_VALUE_ANY && parameterType != argumentType) {
                BReport::getInstance().error()
                        << "Function " << m_function->getName()->getValue()
                        << " expects argument " << paramIndex + 1 << " to be of type " << parameterType
                        << " but given " << argumentType << std::endl;
                BReport::getInstance().printError();
            }
        }
    } else {
        BReport::getInstance().error()
                << "Arguments number " << m_arguments.size()+1
                << " for function call " << m_lexicalToken->getValue()
                << " cannot be verified because the function is undefined" << std::endl;
        BReport::getInstance().printError();
    }
}

