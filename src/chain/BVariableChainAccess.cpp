#include <bashclass/BVariableChainAccess.h>
#include <bashclass/BException.h>
#include <bashclass/BTypeFactory.h>

bool BVariableChainAccess::isFound() {
    return m_variable != nullptr;
}

std::shared_ptr<IBType> BVariableChainAccess::getType() {
    if(!m_variable || !m_variable->getType()->hasKnownType()) {
        return BTypeFactory::createUndefinedExpressionType();
    }
    return m_variable->getType();
}