#include <bashclass/BScope.h>
#include <bashclass/BFunction.h>
#include <bashclass/BGlobal.h>
#include <bashclass/BClass.h>
#include <bashclass/BWhile.h>
#include <bashclass/BIf.h>
#include <bashclass/BashClass.h>
#include <sstream>

std::shared_ptr<BScope> BScope::createClass(std::shared_ptr<ecc::LexicalToken> lexicalToken) {
    auto classComp = std::make_shared<BClass>();
    classComp->setParentScope(shared_from_this());
    classComp->setReferenceToken(lexicalToken);
    m_scopes[lexicalToken->getUID()]=classComp;
    return classComp;
}

std::shared_ptr<BScope> BScope::createFunction(std::shared_ptr<ecc::LexicalToken> lexicalToken) {
    auto functionComp = std::make_shared<BFunction>();
    functionComp->setParentScope(shared_from_this());
    functionComp->setReferenceToken(lexicalToken);
    m_scopes[lexicalToken->getUID()]=functionComp;
    return functionComp;
}

std::shared_ptr<BScope> BScope::createWhile(std::shared_ptr<ecc::LexicalToken> lexicalToken) {
    auto whileComp = std::make_shared<BWhile>();
    whileComp->setParentScope(shared_from_this());
    whileComp->setReferenceToken(lexicalToken);
    m_scopes[lexicalToken->getUID()]=whileComp;
    return whileComp;
}

std::shared_ptr<BScope> BScope::createIf(std::shared_ptr<ecc::LexicalToken> lexicalToken) {
    auto ifComp = std::make_shared<BIf>();
    ifComp->setParentScope(shared_from_this());
    ifComp->setReferenceToken(lexicalToken);
    m_scopes[lexicalToken->getUID()]=ifComp;
    return ifComp;
}

std::shared_ptr<BVariable> BScope::createVariable(std::shared_ptr<ecc::LexicalToken> lexicalToken) {
    auto variableComp = std::make_shared<BVariable>();
    variableComp->setParentScope(shared_from_this());
    variableComp->setReferenceToken(lexicalToken);
    m_variables[lexicalToken->getUID()]=variableComp;
    return variableComp;
}

std::vector<std::shared_ptr<BVariable>> BScope::findAllVariables(const char* name) {
    std::vector<std::shared_ptr<BVariable>> variables;
    for(auto variable : m_variables) {
        if(!name || (variable.second->getName() && variable.second->getName()->getValue() == name)) {
            variables.push_back(variable.second);
        }
    }
    return variables;
}

std::vector<std::shared_ptr<BVariable>> BScope::findAllParameters(const char *name) {
    std::vector<std::shared_ptr<BVariable>> parameters;
    for(auto variable : m_variables) {
        if(variable.second->isParam() && (!name ||
                (variable.second->getName() && variable.second->getName()->getValue() == name))) {
            parameters.push_back(variable.second);
        }
    }
    return parameters;
}

std::vector<std::shared_ptr<BScope>> BScope::findAllClasses(const char* name) {
    std::vector<std::shared_ptr<BScope>> classes;
    for(auto scope : m_scopes) {
        std::shared_ptr<BClass> classScope = std::dynamic_pointer_cast<BClass>(scope.second);
        if(classScope && (!name ||
                (classScope->getName() && classScope->getName()->getValue() == name))) {
            classes.push_back(classScope);
        }
    }
    return classes;
}

std::vector<std::shared_ptr<BScope>> BScope::findAllFunctions(const char *name) {
    std::vector<std::shared_ptr<BScope>> functions;
    for(auto scope : m_scopes) {
        std::shared_ptr<BFunction> functionScope = std::dynamic_pointer_cast<BFunction>(scope.second);
        if(functionScope && (!name ||
                (functionScope->getName() && functionScope->getName()->getValue() == name))) {
            functions.push_back(functionScope);
        }
    }
    return functions;
}

std::shared_ptr<BScope> BScope::getScopeByToken(std::shared_ptr<ecc::LexicalToken> lexicalToken) {
    if(m_scopes.find(lexicalToken->getUID()) != m_scopes.end()) {
        return m_scopes[lexicalToken->getUID()];
    }
    return nullptr;
}

std::shared_ptr<BVariable> BScope::getVariableByToken(std::shared_ptr<ecc::LexicalToken> lexicalToken) {
    if(m_variables.find(lexicalToken->getUID()) != m_variables.end()) {
        return m_variables[lexicalToken->getUID()];
    }
    return nullptr;
}

std::vector<std::shared_ptr<BScope>> BScope::getAllScopes() {
    std::vector<std::shared_ptr<BScope>> scopes;
    for(auto pair : m_scopes) {
        scopes.push_back(pair.second);
    }
    return scopes;
}

std::shared_ptr<BVariable> BScope::findClosestVariable(std::string name) {
    auto currentScope = shared_from_this();
    while(currentScope != nullptr) {
        auto variables = currentScope->findAllVariables(name.c_str());
        if(!variables.empty()) {
            // If multiple variable definition were found (semantic error), also select the front
            return variables.front();
        }
        currentScope = currentScope->getParentScope();
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
        auto castWhile = std::dynamic_pointer_cast<BWhile>(top);
        auto castIf = std::dynamic_pointer_cast<BIf>(top);

        // Get corresponding information
        if(castClass) {
            structure << "class: " << castClass->getName() << ". Label: " << castClass->getLabel().str() << std::endl;
        } else if(castFunction) {
            structure << "function: " << castFunction->getName() << " : " << castFunction->getType() <<
            ". Label: " << castFunction->getLabel().str() << std::endl;
        } else if(castGlobal) {
            structure << "global" << std::endl;
        } else if(castWhile) {
            structure << "while. Label: " << castWhile->getLabel().str() << std::endl;
        } else if(castIf) {
            structure << "if. Label: " << castIf->getLabel().str() << std::endl;
        }

        // Get all variables
        for(auto variable : top->findAllVariables()) {
            structure << "variable" << (variable->isParam() ? "[P]" : "") << ": " << variable->getName()
            << " : " << variable->getType() << ". Label: " << variable->getLabel().str() << std::endl;
        }

        if(castGlobal || castClass || castFunction || castWhile || castIf) {
            // Push all the children scopes
            for(auto scope : top->m_scopes) {
                structureStack.push(scope.second);
            }
        }
    }
    return structure;
}