#include <bashclass/BFunctionCall.h>
#include <bashclass/BTypes.h>
#include <iostream>

std::string BFunctionCall::getTypeValueAsString() {
    if(!m_function || !m_function->hasKnowType()) {
        return BType::UNDEFINED;
    }
    return m_function->getType()->getValue();
}

void BFunctionCall::verifyArguments() {

    if(!m_function) {
        std::cerr << "Arguments for function call " << m_lexicalToken->getValue() << " cannot be verified" << std::endl;
        return;
    }

    // Get function parameters
    auto parameters = m_function->findAllParameters();

    // Arguments number must match function parameters
    if(parameters.size() != m_arguments.size()) {
        std::cerr << "Function " << m_function->getName()->getValue()
                  << " expects " << parameters.size() << " arguments but given "
                  << m_arguments.size() << " instead" << std::endl;
    } else {

        // Arguments type must match function parameters
        for(size_t i = 0; i < parameters.size(); i++) {
            std::cout << m_arguments[i]->getTypeValueAsString() << std::endl;
            if(!parameters[i]->hasKnownType()) {
                std::cerr << "Cannot pass argument value to an undefined type for parameter "
                          << parameters[i]->getName()->getValue() << " in function "
                          << m_function->getName()->getValue() << std::endl;
            } else if(m_arguments[i]->getTypeValueAsString() == BType::UNDEFINED) {
                std::cerr << "Parameter " << parameters[i]->getName()->getValue()
                          << " in function " << m_function->getName()->getValue()
                          << " is given an undefined argument" << std::endl;
            } else {
                std::string argumentType = m_arguments[i]->getTypeValueAsString();
                std::string parameterType = parameters[i]->getType()->getValue();
                if(parameterType != BType::TYPE_VALUE_ANY && parameterType != argumentType) {
                    std::cerr << "Function " << m_function->getName()->getValue()
                              << " expects argument " << i + 1 << " to be of type " << parameterType
                              << " but given " << argumentType << std::endl;
                }
            }
        }
    }
}
