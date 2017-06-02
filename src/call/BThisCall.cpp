#include <bashclass/BThisCall.h>
#include <bashclass/BClass.h>
#include <bashclass/BTypes.h>

std::string BThisCall::getTypeValueAsString() {
    if(!m_reference) {
        return BType::UNDEFINED;
    }
    return m_reference->getName()->getValue();
}