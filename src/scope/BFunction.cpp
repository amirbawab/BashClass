#include <bashclass/BFunction.h>
#include <iostream>
#include <bashclass/BException.h>
#include <bashclass/BReport.h>
#include <bashclass/BVariable.h>
#include <stack>
#include <bashclass/BReturn.h>

BFunction::BFunction() {
    m_isConstructor = false;
    m_type = std::make_shared<BElementType>();
}

std::stringstream BFunction::getLabel() {
    std::stringstream stream = m_parentScope->getLabel();
    stream << "_f_" << m_name->getValue();
    return stream;
}

bool BFunction::isClassMember() {
    if(!m_parentScope) {
        throw BException("Cannot check if a function is a class member if it does not have a parent scope");
    }
    return std::dynamic_pointer_cast<BClass>(m_parentScope) != nullptr;
}

bool BFunction::requiresReturn() {
    return !m_type->isVoid() && !isConstructor();
}

void BFunction::verifyReturns() {
    if(requiresReturn() && !hasReturn()) {
        BReport::getInstance().error()
                << "Missing return statement in function " << m_name->getValue() << std::endl;
        BReport::getInstance().printError();
    }

    // No need to check for the case of return not required
    // since it is already handled when it is set into a scope
}

void BFunction::verifyParameters() {
    auto params = findAllParameters();

    // If a parameter has an default expression,
    // then all the following ones must also have a default expression
    bool hasExpression = false;
    for(size_t i=0; i < params.size(); i++) {
        if(hasExpression && !params[i]->getExpression()) {
            BReport::getInstance().error()
                    << "Parameter " << i+1 << " in function " << m_name->getValue()
                    << " is missing a default expression" << std::endl;
            BReport::getInstance().printError();
        }
        hasExpression |= params[i]->getExpression() != nullptr;
    }
}

std::shared_ptr<BFunction> BFunction::findClosestFunction() {
    return std::static_pointer_cast<BFunction>(shared_from_this());
}

bool BFunction::isConstructor() {
    return m_isConstructor;
}

std::vector<std::shared_ptr<BVariable>> BFunction::findAllParameters(const char *name) {
    std::vector<std::shared_ptr<BVariable>> parameters;
    for(auto variable : m_variables) {
        if(variable.second->isParam() && (!name ||
                                          (variable.second->getName() && variable.second->getName()->getValue() == name))) {
            parameters.push_back(variable.second);
        }
    }
    return parameters;
}

bool BFunction::hasSameSignature(std::shared_ptr<BFunction> function) {

    // Name must be identical
    if(function->getName()->getValue() != getName()->getValue()) {
        return false;
    }

    // Check if both functions hve the same return type
    if(!function->getType()->isSame(getType())) {
        return false;
    }

    // Check if the arguments number, order and type match
    auto thisParam = findAllParameters();
    auto funcParam = function->findAllParameters();
    if(thisParam.size() != funcParam.size()) {
        return false;
    }

    for(size_t i=0; i < thisParam.size(); i++) {
        if(!thisParam[i]->getType()->isSame(funcParam[i]->getType())) {
            return false;
        }
    }
    return true;
}

void BFunction::verifyOverride() {
    if(isClassMember()) {
        auto parentClass = std::static_pointer_cast<BClass>(m_parentScope)->getExtends();
        if(parentClass) {
            auto parGetFunc = parentClass->findAllFunctionsExtended(getName()->getValue().c_str());
            if(!parGetFunc.empty()) {
                auto overFunction = parGetFunc.back();
                if(!hasSameSignature(overFunction)) {
                    BReport::getInstance().error()
                            << "Function " << getName()->getValue() << " at line "
                            << getName()->getLine()
                            <<" tries to override a parent function with a different signature" << std::endl;
                    BReport::getInstance().printError();
                }
            }
        }
    }
}

void BFunction::verifyNoSuperConstructor() {

    // If function is a constructor and class extends another one,
    // then a super constructor call is required
    if(isConstructor()) {
        auto parentClass = std::static_pointer_cast<BClass>(m_parentScope);
        if(parentClass->getExtends()) {
            BReport::getInstance().error()
                    << "Constructor at line "
                    << getName()->getLine()
                    <<" is missing a super constructor" << std::endl;
            BReport::getInstance().printError();
        }
    }
}