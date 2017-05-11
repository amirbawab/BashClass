#include <bashclass/BGlobal.h>

std::shared_ptr<BClass> BGlobal::createClass() {
    auto classComp = std::make_shared<BClass>();
    m_classes.push_back(classComp);
    return classComp;
}

std::shared_ptr<BFunction> BGlobal::createFunction() {
    auto functionComp = std::make_shared<BFunction>();
    m_functions.push_back(functionComp);
    return functionComp;
}

std::shared_ptr<BVariable> BGlobal::createVariable() {
    auto variableComp = std::make_shared<BVariable>();
    m_variables.push_back(variableComp);
    return variableComp;
}
