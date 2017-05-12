#include <bashclass/BScope.h>

std::shared_ptr<BVariable> BScope::createVariable() {
    auto variableComp = std::make_shared<BVariable>();
    m_variables.push_back(variableComp);
    return variableComp;
}

std::vector<std::shared_ptr<BVariable>> BScope::findAllVariables(std::string name) {
    std::vector<std::shared_ptr<BVariable>> variables;
    for(auto variable : m_variables) {
        if(variable->getName() == name) {
            variables.push_back(variable);
        }
    }
    return variables;
}