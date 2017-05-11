#include <bashclass/BClass.h>

std::shared_ptr<BVariable> BClass::createVariable() {
    auto variableComp = std::make_shared<BVariable>();
    m_variables.push_back(variableComp);
    return variableComp;
}

std::shared_ptr<BFunction> BClass::createFunction() {
    auto functionComp = std::make_shared<BFunction>();
    m_functions.push_back(functionComp);
    return functionComp;
}
