#include <bashclass/BScope.h>
#include <bashclass/BFunction.h>
#include <bashclass/BGlobal.h>
#include <bashclass/BClass.h>
#include <bashclass/BWhile.h>
#include <bashclass/BIf.h>
#include <bashclass/BashClass.h>
#include <iostream>
#include <sstream>

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
    throw std::runtime_error("Requesting scope with an unrecognized token key");
}

std::shared_ptr<BVariable> BScope::getVariableByToken(std::shared_ptr<ecc::LexicalToken> lexicalToken) {
    if(m_variables.find(lexicalToken->getUID()) != m_variables.end()) {
        return m_variables[lexicalToken->getUID()];
    }
    throw std::runtime_error("Requesting variable with an unrecognized token key");
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
            structure << "class: " << castClass->getName()->getValue() << ". Label: "
            << castClass->getLabel().str() << std::endl;
        } else if(castFunction) {
            structure << "function: " << castFunction->getName()->getValue()
            << " : " << castFunction->getType()->getValue() << ". Label: " << castFunction->getLabel().str()
            << std::endl;
        } else if(castGlobal) {
            structure << "global" << std::endl;
        } else if(castWhile) {
            structure << "while. Label: " << castWhile->getLabel().str() << std::endl;
        } else if(castIf) {
            structure << "if. Label: " << castIf->getLabel().str() << std::endl;
        }

        // Get all variables
        for(auto variable : top->findAllVariables()) {
            structure << "variable" << (variable->isParam() ? "[P]" : "") << ": " << variable->getName()->getValue()
            << " : " << variable->getType()->getValue() << ". Label: " << variable->getLabel().str() << std::endl;
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

void BScope::registerClass(std::shared_ptr<ecc::LexicalToken> token, std::shared_ptr<BScope> classScope) {

    // Cast class
    auto classScopeCast = std::dynamic_pointer_cast<BClass>(classScope);

    // Class name is required
    if(!classScopeCast->getName()) {
        throw std::runtime_error("Cannot register a class without specifying its name first");
    }

    // Check if class was added previously
    auto getClass = findAllClasses(classScopeCast->getName()->getValue().c_str());
    if(!getClass.empty()) {
        std::cerr << "Class " << classScopeCast->getName()->getValue() << " at line "
                  << classScopeCast->getName()->getLine() << " was defined previously" << std::endl;
    }

    // Register class in this scope
    registerScope(token, classScope);
}

void BScope::registerFunction(std::shared_ptr<ecc::LexicalToken> token, std::shared_ptr<BScope> functionScope) {

    // Cast function
    auto functionScopeCast = std::dynamic_pointer_cast<BFunction>(functionScope);

    // Function name is required
    if(!functionScopeCast->getName()) {
        throw std::runtime_error("Cannot register a function without specifying its name first");
    }

    // Check if function was added previously
    auto getFunc = findAllFunctions(functionScopeCast->getName()->getValue().c_str());
    if(!getFunc.empty()) {
        std::cerr << "Function " << functionScopeCast->getName()->getValue() << " at line "
                  << functionScopeCast->getName()->getLine()
                  <<" was defined previously in the same scope" << std::endl;
    }

    // Register function in this scope
    registerScope(token, functionScope);
}

void BScope::registerScope(std::shared_ptr<ecc::LexicalToken> token, std::shared_ptr<BScope> scope) {
    m_scopes[token->getUID()] = scope;
    scope->setParentScope(shared_from_this());
}

void BScope::registerVariable(std::shared_ptr<ecc::LexicalToken> token, std::shared_ptr<BVariable> variable) {

    // Variable name is required
    if(!variable->getName()) {
        throw std::runtime_error("Cannot register a variable without specifying its name");
    }

    // Check if variable was added previously
    auto getVar = findAllVariables(variable->getName()->getValue().c_str());
    if(!getVar.empty()) {
        std::cerr << (variable->isParam() ? "Parameter " : "Variable ")
                  << variable->getName()->getValue() << " at line " << variable->getName()->getLine()
                  << " was defined previously in the same scope" << std::endl;
    }

    // Register variable in this scope
    m_variables[token->getUID()] = variable;
    variable->setParentScope(shared_from_this());
}

void BScope::registerChainCall(std::shared_ptr<ecc::LexicalToken> token,
                                  std::shared_ptr<BChainCall> chainCall) {
    m_chainCalls[token->getUID()] = chainCall;
}

std::shared_ptr<BChainCall> BScope::getChainCallByToken(std::shared_ptr<ecc::LexicalToken> token) {
    if(m_chainCalls.find(token->getUID()) == m_chainCalls.end()) {
        throw std::runtime_error("Requesting chain call with an unrecognized token key");
    }
    return m_chainCalls[token->getUID()];
}