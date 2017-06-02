#include <bashclass/BClass.h>
#include <bashclass/BGenerateCode.h>

std::stringstream BClass::getLabel() {
    std::stringstream stream = m_parentScope->getLabel();
    stream << "_c_" << m_name->getValue();
    return stream;
}

void BClass::bashifyHeader() {
    std::stringstream ss;
    ss << "declare -A " << getLabel().str() << "=()" << std::endl;
    BGenerateCode::get().write(ss);
}