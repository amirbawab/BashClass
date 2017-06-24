#include <bashclass/BVariableChainAccess.h>
#include <bashclass/BException.h>
#include <bashclass/BTypeFactory.h>
#include <bashclass/BReport.h>

bool BVariableChainAccess::isFound() {
    return m_variable != nullptr;
}

std::shared_ptr<IBType> BVariableChainAccess::getType() {
    if(!m_variable || !m_variable->getType()->hasKnownType() || m_type->getDimension() < 0) {
        return BTypeFactory::createUndefinedExpressionType();
    }
    return m_type;
}

void BVariableChainAccess::setVariable(std::shared_ptr<BVariable> variable) {
    m_variable = variable;
    m_type = variable->getType()->cloneToExpressionType();
}

