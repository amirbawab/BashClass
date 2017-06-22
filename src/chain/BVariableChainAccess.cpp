#include <bashclass/BVariableChainAccess.h>
#include <bashclass/BException.h>

bool BVariableChainAccess::isFound() {
    return m_variable != nullptr;
}

std::shared_ptr<IBType> BVariableChainAccess::getType() {
    if(!m_variable) {
        throw BException("Cannot get type of a variable that was not set");
    }
    return m_variable->getType();
}