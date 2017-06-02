#include <bashclass/BVariable.h>
#include <bashclass/BTypes.h>
#include <bashclass/BClass.h>

BVariable::BVariable() {
    m_isParam = false;
}

std::stringstream BVariable::getLabel() {
    std::stringstream stream = m_parentScope->getLabel();
    stream << (m_isParam ? "_p_" : "_v_") << m_name->getValue();
    return stream;
}

bool BVariable::isClassMember() {
    return m_parentScope && std::dynamic_pointer_cast<BClass>(m_parentScope);
}

bool BVariable::hasKnownType() const {
    return BType::isBuiltInType(m_type->getName()) || m_typeScope;
}