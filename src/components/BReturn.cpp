#include <bashclass/BReturn.h>
#include <bashclass/BException.h>
#include <bashclass/BReport.h>

void BReturn::verifyReturn() {

    if(!m_parentScope) {
        throw BException("Cannot verify a return statement without a parent scope");
    }

    // Get ancestor function
    auto function = m_parentScope->findClosestFunction();
    if(!function) {
        throw BException("Cannot verify a return statement without a function");
    }

    // If function is of type void, then a return statement is not expected
    if(function->requiresReturn()) {

        // A function that requires a return statement
        // must only have return statements with expressions
        if(!m_expression) {
            BReport::getInstance().error()
                    << "Function " << function->getName()->getValue() << " expects a return statement with expression"
                    << std::endl;
            BReport::getInstance().printError();
        } else {

            std::shared_ptr<IBType> functionType = function->getType();
            std::shared_ptr<IBType> expressionType = m_expression->getType();

            if(!function->getType()->hasKnownType()) {
                BReport::getInstance().error()
                        << "Cannot return expression in function " << function->getName()->getValue()
                        << " of undefined type" << std::endl;
                BReport::getInstance().printError();
            } else if(expressionType->isUndefined()) {
                BReport::getInstance().error()
                        << "Function " << function->getName()->getValue()
                        << " has return statement but of undefined type" << std::endl;
                BReport::getInstance().printError();
            } else if(!functionType->isCompatible(expressionType)) {
                BReport::getInstance().error()
                        << "Function " << function->getName()->getValue() << " is of type " << functionType
                        << " but return expression is of type " << expressionType << std::endl;
                BReport::getInstance().printError();
            }
        }
    } else if (m_expression) {
        BReport::getInstance().error()
                << "Function " << function->getName()->getValue()
                << " does not expect to return an expression" << std::endl;
        BReport::getInstance().printError();
    }
}
