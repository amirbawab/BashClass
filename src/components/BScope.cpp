#include <bashclass/BScope.h>
#include <bashclass/BFunction.h>
#include <bashclass/BGlobal.h>
#include <bashclass/BashClass.h>
#include <sstream>

std::shared_ptr<BScope> BScope::createClass(std::shared_ptr<ecc::LexicalToken> lexicalToken) {
    auto classComp = std::make_shared<BClass>();
    classComp->setParentScope(shared_from_this());
    classComp->setLexicalToken(lexicalToken);
    m_scopes[lexicalToken->getUID()]=classComp;
    return classComp;
}

std::shared_ptr<BScope> BScope::createFunction(std::shared_ptr<ecc::LexicalToken> lexicalToken) {
    auto functionComp = std::make_shared<BFunction>();
    functionComp->setParentScope(shared_from_this());
    functionComp->setLexicalToken(lexicalToken);
    m_scopes[lexicalToken->getUID()]=functionComp;
    return functionComp;
}

std::shared_ptr<BVariable> BScope::createVariable(std::shared_ptr<ecc::LexicalToken> lexicalToken) {
    auto variableComp = std::make_shared<BVariable>();
    variableComp->setParentScope(shared_from_this());
    variableComp->setLexicalToken(lexicalToken);
    m_variables[lexicalToken->getUID()]=variableComp;
    return variableComp;
}

std::vector<std::shared_ptr<BVariable>> BScope::findAllVariables(const char* name) {
    std::vector<std::shared_ptr<BVariable>> variables;
    for(auto variable : m_variables) {
        if(!name || variable.second->getName() == name) {
            variables.push_back(variable.second);
        }
    }
    return variables;
}

std::vector<std::shared_ptr<BVariable>> BScope::findAllParameters(char *name) {
    std::vector<std::shared_ptr<BVariable>> parameters;
    for(auto variable : m_variables) {
        if(variable.second->isParam() && (!name || variable.second->getName() == name)) {
            parameters.push_back(variable.second);
        }
    }
    return parameters;
}

std::vector<std::shared_ptr<BScope>> BScope::findAllClasses(const char* name) {
    std::vector<std::shared_ptr<BScope>> classes;
    for(auto scope : m_scopes) {
        std::shared_ptr<BClass> classScope = std::dynamic_pointer_cast<BClass>(scope.second);
        if(classScope && (!name || classScope->getName() == name)) {
            classes.push_back(classScope);
        }
    }
    return classes;
}

std::vector<std::shared_ptr<BScope>> BScope::findAllFunctions(const char *name) {
    std::vector<std::shared_ptr<BScope>> functions;
    for(auto scope : m_scopes) {
        std::shared_ptr<BFunction> functionScope = std::dynamic_pointer_cast<BFunction>(scope.second);
        if(functionScope && (!name || functionScope->getName() == name)) {
            functions.push_back(functionScope);
        }
    }
    return functions;
}

std::shared_ptr<BScope> BScope::getScope(std::shared_ptr<ecc::LexicalToken> lexicalToken) {
    if(m_scopes.find(lexicalToken->getUID()) != m_scopes.end()) {
        return m_scopes[lexicalToken->getUID()];
    }
    return nullptr;
}

std::shared_ptr<BVariable> BScope::getVariable(std::shared_ptr<ecc::LexicalToken> lexicalToken) {
    if(m_variables.find(lexicalToken->getUID()) != m_variables.end()) {
        return m_variables[lexicalToken->getUID()];
    }
    return nullptr;
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
            structure << "class: " << castClass->getName() << ". Label: " << castClass->getLabel().str() << std::endl;
        } else if(castFunction) {
            structure << "function: " << castFunction->getName() << " : " << castFunction->getType() <<
            ". Label: " << castFunction->getLabel().str() << std::endl;
        } else if(castGlobal) {
            structure << "global" << std::endl;
        }

        // Get all variables
        for(auto variable : top->findAllVariables()) {
            structure << "variable" << (variable->isParam() ? "[P]" : "") << ": " << variable->getName()
            << " : " << variable->getType() << ". Label: " << variable->getLabel().str() << std::endl;
        }

        if(castGlobal || castClass || castFunction) {
            // Push all the children scopes
            for(auto scope : top->m_scopes) {
                structureStack.push(scope.second);
            }
        }
    }
    return structure;
}