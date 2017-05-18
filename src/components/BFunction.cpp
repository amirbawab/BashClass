#include <bashclass/BFunction.h>
#include <bashclass/BClass.h>
#include <bashclass/BTypes.h>

std::stringstream BFunction::getLabel() {
    std::stringstream stream = m_parentScope->getLabel();
    stream << "_f_" << m_name->getValue();
    return stream;
}

bool BFunction::isClassMember() {
    return m_parentScope && std::dynamic_pointer_cast<BClass>(m_parentScope);
}

std::string BFunction::getTypeValue() const {
    if(!m_knownType) {
        return BType::UNDEFINED;
    }
    return m_type->getValue();
}
