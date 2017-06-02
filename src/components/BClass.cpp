#include <bashclass/BClass.h>
#include <bashclass/BGenerateCode.h>

std::stringstream BClass::getLabel() {
    std::stringstream stream = m_parentScope->getLabel();
    stream << "_c_" << m_name->getValue();
    return stream;
}
