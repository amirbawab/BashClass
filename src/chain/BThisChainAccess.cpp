#include <bashclass/BThisChainAccess.h>
#include <bashclass/BElementType.h>
#include <bashclass/BException.h>

std::string BThisChainAccess::getTypeValueAsString() {
    if(!m_reference) {
        return BElementType::UNDEFINED;
    }
    return m_reference->getName()->getValue();
}

bool BThisChainAccess::isKnown() {
    return m_reference != nullptr;
}

std::shared_ptr<BClass> BThisChainAccess::getTypeScope() {
    if(!m_reference) {
        throw BException("Cannot get type scope of a 'this' with an unknown reference");
    }
    return m_reference;
}