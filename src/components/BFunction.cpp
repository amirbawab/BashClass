#include <bashclass/BFunction.h>
#include <bashclass/BClass.h>
#include <bashclass/BTypes.h>
#include <iostream>

std::stringstream BFunction::getLabel() {
    std::stringstream stream = m_parentScope->getLabel();
    stream << "_f_" << m_name->getValue();
    return stream;
}

bool BFunction::isClassMember() {
    return m_parentScope && std::dynamic_pointer_cast<BClass>(m_parentScope);
}

bool BFunction::hasKnowType() const {
    return BType::isBuiltInType(m_type->getName()) || m_typeScope;
}

void BFunction::verifyParameters() {
    if(isClassMember()) {
        auto params = findAllParameters();
        if(params.empty() || params.front()->getTypeScope() != m_parentScope) {
            auto castClass = std::dynamic_pointer_cast<BClass>(m_parentScope);
            std::cerr << "Function " << getName()->getValue() << " in class "
                      << castClass->getName()->getValue() <<" must have the first argument of type "
                      << castClass->getName()->getValue() << std::endl;
        }
    }
}