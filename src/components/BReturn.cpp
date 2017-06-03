#include <bashclass/BReturn.h>
#include <bashclass/BException.h>
#include <bashclass/BFunction.h>
#include <bashclass/BReport.h>
#include <bashclass/BTypes.h>

void BReturn::verifyReturn() {

    if(!m_expression) {
        throw BException("Cannot verify a return statement without an expression");
    }

    if(!m_function) {
        throw BException("Cannot verify a return statement without a function");
    }

    // If function is of type void, then a return statement is not expected
    if(m_function->requiresReturn()) {
        std::string functionType = m_function->getType()->getValue();
        std::string expressionType = m_expression->getTypeValueAsString();
        if(!m_function->hasKnowType()) {
            BReport::getInstance().error()
                    << "Cannot return expression in function " << m_function->getName()->getValue()
                    << " of undefined type" << std::endl;
            BReport::getInstance().printError();
        } else if(expressionType == BType::UNDEFINED) {
            BReport::getInstance().error()
                    << "Function " << m_function->getName()->getValue()
                    << " has return statement but of undefined type" << std::endl;
            BReport::getInstance().printError();
        } else if(functionType != BType::TYPE_VALUE_ANY && functionType != expressionType) {
            BReport::getInstance().error()
                    << "Function " << m_function->getName()->getValue() << " is of type " << functionType
                    << " but return expression is of type " << expressionType << std::endl;
            BReport::getInstance().printError();
        }
    } else {
        BReport::getInstance().error()
                << "Function " << m_function->getName()->getValue()
                << " does not expect to return an expression" << std::endl;
        BReport::getInstance().printError();
    }
}
