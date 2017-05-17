#include <bashclass/BVariable.h>

BVariable::BVariable() {
    m_isParam = false;
}

std::stringstream BVariable::getLabel() {
    std::stringstream stream = m_parentScope->getLabel();
    stream << "_v_" << m_name->getValue();
    return stream;
}