#include <bashclass/BSuperChainAccess.h>
#include <bashclass/BTypeFactory.h>

bool BSuperChainAccess::isFound() {
    return m_srcReference && m_srcReference->getExtends();
}

std::shared_ptr<IBType> BSuperChainAccess::getType() {
    if(!isFound()) {
        return BTypeFactory::createUndefinedExpressionType();
    }
    return BTypeFactory::createClassExpressionType(m_srcReference->getExtends());
}
