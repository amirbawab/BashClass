#include <bashclass/BVariable.h>
#include <bashclass/BException.h>

BVariable::BVariable() {
    m_isParam = false;
    m_type = std::make_shared<BElementType>();
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
    return std::dynamic_pointer_cast<BClass>(m_parentScope) != nullptr;
}

std::string BVariable::getDefaultValue() {
    if(!m_type) {
        throw BException("Cannot get initial value without setting the variable type");
    }

    return "0";
}