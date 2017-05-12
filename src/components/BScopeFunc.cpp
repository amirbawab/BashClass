#include <bashclass/BScopeFunc.h>

std::shared_ptr<BFunction> BScopeFunc::createFunction() {
    auto functionComp = std::make_shared<BFunction>();
    m_functions.push_back(functionComp);
    return functionComp;
}

std::vector<std::shared_ptr<BFunction>> BScopeFunc::findAllFunctions(std::string name) {
    std::vector<std::shared_ptr<BFunction>> functions;
    for(auto function : m_functions) {
        if(function->getName() == name) {
            functions.push_back(function);
        }
    }
    return functions;
}