#include <bashclass/BVariable.h>
#include <bashclass/BTypes.h>
#include <bashclass/BClass.h>

BVariable::BVariable() {
    m_isParam = false;
}

std::stringstream BVariable::getLabel() {
    std::stringstream stream = m_parentScope->getLabel();
    stream << "_v_" << m_name->getValue();
    return stream;
}

std::string BVariable::getTypeValue() const {
    if(!m_knownType) {
        return BType::UNDEFINED;
    }
    return m_type->getValue();
}

bool BVariable::isClassMember() {
    return m_parentScope && std::dynamic_pointer_cast<BClass>(m_parentScope);
}