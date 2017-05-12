#include <bashclass/BScope.h>
#include <bashclass/BFunction.h>
#include <bashclass/BGlobal.h>
#include <bashclass/BashClass.h>

std::shared_ptr<BScope> BScope::createClass() {
    auto classComp = std::make_shared<BClass>();
    m_scopes.push_back(classComp);
    return classComp;
}

std::shared_ptr<BScope> BScope::createFunction() {
    auto functionComp = std::make_shared<BFunction>();
    m_scopes.push_back(functionComp);
    return functionComp;
}

std::shared_ptr<BVariable> BScope::createVariable() {
    auto variableComp = std::make_shared<BVariable>();
    m_variables.push_back(variableComp);
    return variableComp;
}

std::vector<std::shared_ptr<BVariable>> BScope::findAllVariables(char* name) {
    std::vector<std::shared_ptr<BVariable>> variables;
    for(auto variable : m_variables) {
        if(!name || variable->getName() == name) {
            variables.push_back(variable);
        }
    }
    return variables;
}

std::vector<std::shared_ptr<BScope>> BScope::findAllClasses(char* name) {
    std::vector<std::shared_ptr<BScope>> classes;
    for(auto scope : m_scopes) {
        std::shared_ptr<BClass> classScope = std::dynamic_pointer_cast<BClass>(scope);
        if(classScope && (!name || classScope->getName() == name)) {
            classes.push_back(classScope);
        }
    }
    return classes;
}

std::vector<std::shared_ptr<BScope>> BScope::findAllFunctions(char *name) {
    std::vector<std::shared_ptr<BScope>> functions;
    for(auto scope : m_scopes) {
        std::shared_ptr<BFunction> functionScope = std::dynamic_pointer_cast<BFunction>(scope);
        if(functionScope && (!name || functionScope->getName() == name)) {
            functions.push_back(functionScope);
        }
    }
    return functions;
}
