#include <bashclass/BThisChainAccess.h>
#include <bashclass/BTypeFactory.h>
#include <bashclass/BException.h>

bool BThisChainAccess::isFound() {
    return m_reference != nullptr;
}

std::shared_ptr<IBType> BThisChainAccess::getType() {
    if(!m_reference) {
        throw BException("Cannot get type of an undefined reference");
    }
    return BTypeFactory::createClassExpressionType(m_reference);
}
