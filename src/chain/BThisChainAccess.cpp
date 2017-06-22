#include <bashclass/BThisChainAccess.h>
#include <bashclass/BTypeFactory.h>
#include <bashclass/BException.h>

bool BThisChainAccess::isFound() {
    return m_reference != nullptr;
}

std::shared_ptr<IBType> BThisChainAccess::getType() {
    if(!m_reference) {
        return BTypeFactory::createUndefinedExpressionType();
    }
    return BTypeFactory::createClassExpressionType(m_reference);
}
