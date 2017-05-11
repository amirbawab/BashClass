#include <bashclass/BFunction.h>

std::shared_ptr<BVariable> BFunction::createVariable() {
    auto variableComp = std::make_shared<BVariable>();
    m_variables.push_back(variableComp);
    return variableComp;
}
