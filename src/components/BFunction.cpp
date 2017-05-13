#include <bashclass/BFunction.h>

std::vector<std::shared_ptr<BVariable>> BFunction::findAllParameters(char *name) {
    std::vector<std::shared_ptr<BVariable>> parameters;
    for(auto variable : m_variables) {
        if(variable->isParam() && (!name || variable->getName() == name)) {
            parameters.push_back(variable);
        }
    }
    return parameters;
}

