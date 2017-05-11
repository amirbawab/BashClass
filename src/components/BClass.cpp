#include <bashclass/BClass.h>

std::shared_ptr<BFunction> BClass::createFunction() {
    auto functionComp = std::make_shared<BFunction>();
    m_functions.push_back(functionComp);
    return functionComp;
}
