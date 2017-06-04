#include <bashclass/BThisCall.h>
#include <bashclass/BClass.h>
#include <bashclass/BTypes.h>
#include <bashclass/BException.h>

std::string BThisCall::getTypeValueAsString() {
    if(!m_reference) {
        return BType::UNDEFINED;
    }
    return m_reference->getName()->getValue();
}

bool BThisCall::isKnown() {
    return m_reference != nullptr;
}

std::shared_ptr<BClass> BThisCall::getTypeScope() {
    if(!m_reference) {
        throw BException("Cannot get type scope of a 'this' with an unknown reference");
    }
    return m_reference;
}