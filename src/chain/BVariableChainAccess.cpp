#include <bashclass/BVariableChainAccess.h>
#include <bashclass/BTypes.h>
#include <iostream>
#include <bashclass/BReport.h>
#include <bashclass/BException.h>

std::string BVariableChainAccess::getTypeValueAsString() {
    if(!m_variable || !m_variable->hasKnownType()) {
        return BType::UNDEFINED;
    }
    return m_variable->getType()->getValue();
}

bool BVariableChainAccess::isKnown() {
    return m_variable != nullptr;
}

std::shared_ptr<BClass> BVariableChainAccess::getTypeScope() {
    if(!m_variable) {
        throw BException("Cannot get type scope of a function call with an unknown reference");
    }
    return m_variable->getTypeScope();
}
