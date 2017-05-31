#include <bashclass/BVariableCall.h>
#include <bashclass/BTypes.h>

std::string BVariableCall::getTypeValueAsString() {
    if(!m_variable || !m_variable->hasKnownType()) {
        return BType::UNDEFINED;
    }
    return m_variable->getType()->getValue();
}
