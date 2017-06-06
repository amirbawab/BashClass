#include <bashclass/BThisAccess.h>
#include <bashclass/BException.h>

std::string BThisAccess::getTypeValueAsString() {
    if(!m_thisChainAccess) {
        throw BException("Cannot get type value of the 'this' access' without setting the 'this' chain access");
    }
    return m_thisChainAccess->getTypeValueAsString();
}