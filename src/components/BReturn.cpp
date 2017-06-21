#include <bashclass/BReturn.h>
#include <bashclass/BException.h>
#include <bashclass/BFunction.h>
#include <bashclass/BReport.h>
#include <bashclass/BElementType.h>
#include <iostream>

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
            std::string functionType = function->getType()->getValue();
            std::string expressionType = m_expression->getTypeValueAsString();
            if(!function->hasKnowType()) {
                BReport::getInstance().error()
                        << "Cannot return expression in function " << function->getName()->getValue()
                        << " of undefined type" << std::endl;
                BReport::getInstance().printError();
            } else if(BElementType::isUndefined(expressionType)) {
                BReport::getInstance().error()
                        << "Function " << function->getName()->getValue()
                        << " has return statement but of undefined type" << std::endl;
                BReport::getInstance().printError();
            } else if(functionType != BElementType::TYPE_VALUE_ANY
                      && (BElementType::isBuiltInType(function->getType()->getName())
                          || expressionType != BElementType::NULL_VALUE)
                      && functionType != expressionType) {
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
