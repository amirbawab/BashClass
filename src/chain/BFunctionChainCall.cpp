#include <bashclass/BFunctionChainCall.h>
#include <bashclass/BElementType.h>
#include <iostream>
#include <bashclass/BReport.h>
#include <bashclass/BVariable.h>
#include <bashclass/BException.h>

void BFunctionChainCall::verifyArguments() {

    // Check if the number of arguments is less than the number of parameters
    if(m_function) {
        auto params = m_function->findAllParameters();
        if(m_arguments.size() < params.size()) {

            // Check if the remaining params have a default value
            for(size_t paramIndex=m_arguments.size(); paramIndex < params.size(); paramIndex++) {
                if(!params[paramIndex]->getExpression()) {
                    BReport::getInstance().error()
                            << "Function call " << m_lexicalToken->getValue()
                            << " is missing arguments" << std::endl;
                    BReport::getInstance().printError();
                    return;
                }
            }
        }
    }

    // No need to check if for the number of arguments exceeding the number of parameters because
    // this is already handled by the addArgument() function
}

void BFunctionChainCall::addArgument(std::shared_ptr<IBExpression> argument) {

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

            std::shared_ptr<IBType> argumentType = argument->getType();
            std::shared_ptr<IBType> parameterType = parameters[paramIndex]->getType();
            if(!parameters[paramIndex]->getType()->hasKnownType()) {
                BReport::getInstance().error()
                        << "Cannot pass argument value to an undefined type for parameter "
                        << parameters[paramIndex]->getName()->getValue() << " in function "
                        << m_function->getName()->getValue() << std::endl;
                BReport::getInstance().printError();
            } else if(argumentType->isUndefined()) {
                BReport::getInstance().error()
                        << "Parameter " << parameters[paramIndex]->getName()->getValue()
                        << " in function " << m_function->getName()->getValue()
                        << " is given an undefined argument" << std::endl;
                BReport::getInstance().printError();
            } else if(!parameterType->isCompatible(argumentType)) {
                BReport::getInstance().error()
                        << "Function " << m_function->getName()->getValue()
                        << " expects argument " << paramIndex + 1 << " to be of type " << parameterType
                        << " but given " << argumentType << std::endl;
                BReport::getInstance().printError();
            }
        }
    } else {
        BReport::getInstance().error()
                << "Arguments number " << m_arguments.size()
                << " for function call " << m_lexicalToken->getValue()
                << " cannot be verified because the function is undefined" << std::endl;
        BReport::getInstance().printError();
    }
}

bool BFunctionChainCall::isFound() {
    return m_function != nullptr;
}

std::shared_ptr<IBType> BFunctionChainCall::getType() {
    if(!m_function) {
        throw BException("Cannot get type of a function that was not set");
    }
    return m_function->getType();
}