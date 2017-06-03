#include <bashclass/BScope.h>
#include <bashclass/BFunction.h>
#include <bashclass/BGlobal.h>
#include <bashclass/BClass.h>
#include <bashclass/BWhile.h>
#include <bashclass/BIf.h>
#include <bashclass/BashClass.h>
#include <iostream>
#include <sstream>
#include <bashclass/BException.h>
#include <bashclass/BReport.h>
#include <bashclass/BReturn.h>

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

std::shared_ptr<BScope> BScope::getScopeByReferenceKey(unsigned int referenceKey) {
    if(m_scopes.find(referenceKey) != m_scopes.end()) {
        return m_scopes[referenceKey];
    }
    throw BException("Requesting scope with an unrecognized reference key");
}

std::shared_ptr<BVariable> BScope::getVariableByReferenceKey(unsigned int referenceKey) {
    if(m_variables.find(referenceKey) != m_variables.end()) {
        return m_variables[referenceKey];
    }
    throw BException("Requesting variable with an unrecognized reference key");
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
    while(currentScope) {
        auto variables = currentScope->findAllVariables(name.c_str());
        if(!variables.empty()) {
            // If multiple variable definition were found (semantic error), also select the front
            return variables.front();
        }
        currentScope = currentScope->getParentScope();
    }
    return nullptr;
}

std::shared_ptr<BFunction> BScope::findClosestFunction() {
    auto currentScope = shared_from_this();
    while(currentScope) {
        auto functionCast = std::dynamic_pointer_cast<BFunction>(currentScope);
        if(functionCast) {
            return functionCast;
        }
        currentScope = currentScope->getParentScope();
    }
    return nullptr;
}

std::shared_ptr<BClass> BScope::findClosestClass() {
    auto currentScope = shared_from_this();
    while(currentScope) {
        auto classCast = std::dynamic_pointer_cast<BClass>(currentScope);
        if(classCast) {
            return classCast;
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

void BScope::registerClass(unsigned int referenceKey, std::shared_ptr<BScope> classScope) {

    // Cast class
    auto classScopeCast = std::dynamic_pointer_cast<BClass>(classScope);

    // Class name is required
    if(!classScopeCast->getName()) {
        throw BException("Cannot register a class without specifying its name first");
    }

    // Check if class was added previously
    auto getClass = findAllClasses(classScopeCast->getName()->getValue().c_str());
    if(!getClass.empty()) {
        BReport::getInstance().error()
                << "Class " << classScopeCast->getName()->getValue() << " at line "
                << classScopeCast->getName()->getLine() << " was defined previously" << std::endl;
        BReport::getInstance().printError();
    }

    // Register class in this scope
    registerScope(referenceKey, classScope);
}

void BScope::registerFunction(unsigned int referenceKey, std::shared_ptr<BScope> functionScope) {

    // Cast function
    auto functionScopeCast = std::dynamic_pointer_cast<BFunction>(functionScope);

    // Function name is required
    if(!functionScopeCast->getName()) {
        throw BException("Cannot register a function without specifying its name first");
    }

    // Check if function was added previously
    auto getFunc = findAllFunctions(functionScopeCast->getName()->getValue().c_str());
    if(!getFunc.empty()) {
        BReport::getInstance().error()
                << "Function " << functionScopeCast->getName()->getValue() << " at line "
                << functionScopeCast->getName()->getLine()
                <<" was defined previously in the same scope" << std::endl;
        BReport::getInstance().printError();
    }

    // Register function in this scope
    registerScope(referenceKey, functionScope);
}

void BScope::registerScope(unsigned int referenceKey, std::shared_ptr<BScope> scope) {
    m_scopes[referenceKey] = scope;
    scope->setParentScope(shared_from_this());
}

void BScope::registerVariable(unsigned int referenceKey, std::shared_ptr<BVariable> variable) {

    // Variable name is required
    if(!variable->getName()) {
        throw BException("Cannot register a variable without specifying its name");
    }

    // Check if variable was added previously
    auto getVar = findAllVariables(variable->getName()->getValue().c_str());
    if(!getVar.empty()) {
        BReport::getInstance().error()
                << (variable->isParam() ? "Parameter " : "Variable ")
                << variable->getName()->getValue() << " at line " << variable->getName()->getLine()
                << " was defined previously in the same scope" << std::endl;
        BReport::getInstance().printError();
    }

    // Register variable in this scope
    m_variables[referenceKey] = variable;
    variable->setParentScope(shared_from_this());
}

void BScope::setReturn(std::shared_ptr<BReturn> ret) {

    // If several return statement are found in the same scope,
    // then only the first one is registered
    if(m_return) {
        BReport::getInstance().error()
                << "Multiple return statements are set within the same scope in function "
                << m_return->getFunction()->getName()->getValue() << std::endl;
        BReport::getInstance().printError();
    } else {
        // Find function to which this return belongs to
        ret->setFunction(findClosestFunction());

        // Verify return statement
        ret->verifyReturn();

        // Set return statement in this scope
        ret->setParentScope(shared_from_this());
        m_return = ret;
    }
}

void BScope::registerChainCall(unsigned int referenceKey, std::shared_ptr<BChainCall> chainCall) {
    m_chainCalls[referenceKey] = chainCall;
    chainCall->setParentScope(shared_from_this());
}

std::shared_ptr<BChainCall> BScope::getChainCallByReferenceKey(unsigned int referenceKey) {
    if(m_chainCalls.find(referenceKey) == m_chainCalls.end()) {
        throw BException("Requesting chain call with an unrecognized reference key");
    }
    return m_chainCalls[referenceKey];
}
