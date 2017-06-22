#include <bashclass/BVariableAccess.h>
#include <bashclass/BException.h>

std::shared_ptr<BVariableChainAccess> BVariableAccess::last() {
    if(!m_chain) {
        throw BException("Cannot get the last variable chain access element before setting the chain");
    }
    return std::static_pointer_cast<BVariableChainAccess>(m_chain->last());
}


std::shared_ptr<IBType> BVariableAccess::getType() {
    return last()->getType();
}