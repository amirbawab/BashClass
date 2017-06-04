#include <bashclass/BClass.h>
#include <bashclass/BGenerateCode.h>
#include <bashclass/BException.h>

std::stringstream BClass::getLabel() {
    std::stringstream stream = m_parentScope->getLabel();
    stream << "_c_" << m_name->getValue();
    return stream;
}

std::shared_ptr<BClass> BClass::findClosestClass() {
    return std::static_pointer_cast<BClass>(shared_from_this());
}

std::shared_ptr<BFunction> BClass::findClosestFunction() {
    if(!m_parentScope) {
        throw BException("Cannot find closest function for a class with an undefined parent scope");
    }
    return m_parentScope->findClosestFunction();
}
