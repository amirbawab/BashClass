#include <bashclass/BThisChainCall.h>
#include <bashclass/BTypes.h>
#include <bashclass/BException.h>

std::string BThisChainCall::getTypeValueAsString() {
    if(!m_reference) {
        return BType::UNDEFINED;
    }
    return m_reference->getName()->getValue();
}

bool BThisChainCall::isKnown() {
    return m_reference != nullptr;
}

std::shared_ptr<BClass> BThisChainCall::getTypeScope() {
    if(!m_reference) {
        throw BException("Cannot get type scope of a 'this' with an unknown reference");
    }
    return m_reference;
}