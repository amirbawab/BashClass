#include <bashclass/BVariable.h>
#include <bashclass/BTypes.h>
#include <bashclass/BClass.h>
#include <bashclass/BException.h>

BVariable::BVariable() {
    m_isParam = false;
}

std::stringstream BVariable::getLabel() {
    std::stringstream stream = m_parentScope->getLabel();
    stream << (m_isParam ? "_p_" : "_v_") << m_name->getValue();
    return stream;
}

bool BVariable::isClassMember() {
    if(!m_parentScope) {
        throw BException("Cannot check if a variable is a class member if it does not have a parent scope");
    }
    return m_parentScope->findClosestClass() != nullptr;
}

bool BVariable::hasKnownType() const {
    return BType::isBuiltInType(m_type->getName()) || m_typeScope;
}