#include <bashclass/BThisAccess.h>
#include <bashclass/BException.h>

std::string BThisAccess::getTypeValueAsString() {
    return getThisChainAccess()->getTypeValueAsString();
}

void BThisAccess::setChain(std::shared_ptr<BChain> chain) {
    if(chain->size() != 1) {
        throw BException("A 'this' access must contain exactly one 'this' element");
    }
    m_chain = chain;
}

std::shared_ptr<BThisChainAccess> BThisAccess::getThisChainAccess() {
    if(m_chain->empty()) {
        throw BException("Requesting 'this' reference from an empty 'this' access chain");
    }
    return std::static_pointer_cast<BThisChainAccess>(m_chain->last());
}