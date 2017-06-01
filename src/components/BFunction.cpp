#include <bashclass/BFunction.h>
#include <bashclass/BClass.h>
#include <bashclass/BTypes.h>
#include <iostream>
#include <bashclass/BException.h>
#include <bashclass/BReport.h>

std::stringstream BFunction::getLabel() {
    std::stringstream stream = m_parentScope->getLabel();
    stream << "_f_" << m_name->getValue();
    return stream;
}

bool BFunction::isClassMember() {
    return m_parentScope && std::dynamic_pointer_cast<BClass>(m_parentScope);
}

bool BFunction::hasKnowType() const {
    return BType::isBuiltInType(m_type->getName()) || m_typeScope;
}

void BFunction::verifyParameters() {
    if(isClassMember()) {
        auto params = findAllParameters();
        if(params.empty() || params.front()->getTypeScope() != m_parentScope) {
            auto castClass = std::dynamic_pointer_cast<BClass>(m_parentScope);
            BReport::getInstance().error()
                    << "Function " << getName()->getValue() << " in class "
                    << castClass->getName()->getValue() <<" must have the first argument of type "
                    << castClass->getName()->getValue() << std::endl;
            BReport::getInstance().printError();
        }
    }
}

bool BFunction::requiresReturn() {
    return m_type->getName() != BType::TYPE_NAME_VOID;
}

void BFunction::registerReturn(unsigned int referenceKey, std::shared_ptr<BReturn> ret) {

    if(!ret->getExpression()) {
        throw BException("Cannot register a return statement without an expression");
    }

    // If function is of type void, then a return statement is not expected
    if(requiresReturn()) {
        std::string functionType = m_type->getValue();
        std::string expressionType = ret->getExpression()->getTypeValueAsString();
        if(!hasKnowType()) {
            BReport::getInstance().error()
                    << "Cannot return expression in function " << m_name->getValue() << " of undefined type"
                    << std::endl;
            BReport::getInstance().printError();
        } else if(expressionType == BType::UNDEFINED) {
            BReport::getInstance().error()
                    << "Function " << m_name->getValue() << " has return statement but of undefined type" << std::endl;
            BReport::getInstance().printError();
        } else if(functionType != BType::TYPE_VALUE_ANY && functionType != expressionType) {
            BReport::getInstance().error()
                    << "Function " << m_name->getValue() << " is of type " << functionType
                    << " but return expression is of type " << expressionType << std::endl;
            BReport::getInstance().printError();
        }
    } else {
        BReport::getInstance().error()
                << "Function " << m_name->getValue()
                << " does not expect to return an expression" << std::endl;
        BReport::getInstance().printError();
    }

    // Register return expression in this function
    m_returns[referenceKey] = ret;
}

std::shared_ptr<BReturn> BFunction::getReturnByReferenceKey(unsigned int referenceKey) {
    if(m_returns.find(referenceKey) != m_returns.end()) {
        return m_returns[referenceKey];
    }
    throw BException("Requesting return statement with an unrecognized token key");
}

bool BFunction::hasReturn() {
    return !m_returns.empty();
}

void BFunction::verifyReturns() {
    if(requiresReturn() && !hasReturn()) {
        BReport::getInstance().error()
                << "Function " << m_name->getValue() << " is missing a return statement" << std::endl;
        BReport::getInstance().printError();
    }
    // No need to check for !requiresReturn() && hasReturn() because is handled by the register function
}