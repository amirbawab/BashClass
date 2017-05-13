#include <bashclass/BScope.h>
#include <bashclass/BFunction.h>
#include <bashclass/BGlobal.h>
#include <bashclass/BashClass.h>
#include <sstream>
#include <stack>

std::shared_ptr<BScope> BScope::createClass() {
    auto classComp = std::make_shared<BClass>();
    classComp->setParentScope(shared_from_this());
    m_scopes.push_back(classComp);
    return classComp;
}

std::shared_ptr<BScope> BScope::createFunction() {
    auto functionComp = std::make_shared<BFunction>();
    functionComp->setParentScope(shared_from_this());
    m_scopes.push_back(functionComp);
    return functionComp;
}

std::shared_ptr<BVariable> BScope::createVariable() {
    auto variableComp = std::make_shared<BVariable>();
    variableComp->setParentScope(shared_from_this());
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

std::stringstream BScope::getStructure() {
    std::stringstream structure;
    std::stack<std::shared_ptr<BScope>> structureStack;
    structureStack.push(shared_from_this());
    while(!structureStack.empty()) {

        // Get the top of the stack
        std::shared_ptr<BScope> top = structureStack.top();
        structureStack.pop();

        // Cast the top pointer
        auto castGlobal = std::dynamic_pointer_cast<BGlobal>(top);
        auto castClass = std::dynamic_pointer_cast<BClass>(top);
        auto castFunction = std::dynamic_pointer_cast<BFunction>(top);

        // Get corresponding information
        if(castClass) {
            structure << "class: " << castClass->getName() << std::endl;
        } else if(castFunction) {
            structure << "function: " << castFunction->getName() << " : " << castFunction->getType() << std::endl;
        } else if(castGlobal) {
            structure << "global" << std::endl;
        }

        // Get all variables
        for(auto variable : top->findAllVariables()) {
            structure << "variable: " << variable->getName() << " : " << variable->getType() << std::endl;
        }

        if(castGlobal || castClass || castFunction) {
            // Push all the children scopes
            for(auto scope : top->m_scopes) {
                structureStack.push(scope);
            }
        }
    }
    return structure;
}