#include <bashclass/BFunction.h>
#include <bashclass/BClass.h>
#include <bashclass/BTypes.h>
#include <iostream>
#include <bashclass/BException.h>
#include <bashclass/BReport.h>
#include <stack>

std::stringstream BFunction::getLabel() {
    std::stringstream stream = m_parentScope->getLabel();
    stream << "_f_" << m_name->getValue();
    return stream;
}

bool BFunction::isClassMember() {
    if(!m_parentScope) {
        throw BException("Cannot check if a function is a class member if it does not have a parent scope");
    }
    return m_parentScope->findClosestClass() != nullptr;
}

bool BFunction::hasKnowType() const {
    return BType::isBuiltInType(m_type->getName()) || m_typeScope;
}

bool BFunction::requiresReturn() {
    return m_type->getName() != BType::TYPE_NAME_VOID
           && m_type->getName() != BType::TYPE_NAME_CONSTRUCTOR;
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

std::shared_ptr<BFunction> BFunction::findClosestFunction() {
    return std::static_pointer_cast<BFunction>(shared_from_this());
}

std::shared_ptr<BClass> BFunction::findClosestClass() {
    if(!m_parentScope) {
        throw BException("Cannot find closest class for a function with an undefined parent scope");
    }
    return m_parentScope->findClosestClass();
}