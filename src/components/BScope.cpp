#include <bashclass/BScope.h>

std::shared_ptr<BVariable> BScope::createVariable() {
    auto variableComp = std::make_shared<BVariable>();
    m_variables.push_back(variableComp);
    return variableComp;
}
