#include <bashclass/BGlobal.h>

std::shared_ptr<BClass> BGlobal::createClass() {
    auto classComp = std::make_shared<BClass>();
    m_classes.push_back(classComp);
    return classComp;
}

