#include <bashclass/BGlobal.h>

std::shared_ptr<BClass> BGlobal::createClass() {
    auto classComp = std::make_shared<BClass>();
    m_classes.push_back(classComp);
    return classComp;
}

std::vector<std::shared_ptr<BClass>> BGlobal::findAllClasses(std::string name) {
    std::vector<std::shared_ptr<BClass>> classes;
    for(auto cls : m_classes) {
        if(cls->getName() == name) {
            classes.push_back(cls);
        }
    }
    return classes;
}

