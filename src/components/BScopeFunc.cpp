#include <bashclass/BScopeFunc.h>

std::shared_ptr<BFunction> BScopeFunc::createFunction() {
    auto functionComp = std::make_shared<BFunction>();
    m_functions.push_back(functionComp);
    return functionComp;
}
