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
#include <bashclass/BVariableAssign.h>
#include <bashclass/BFunctionCall.h>

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

std::vector<std::shared_ptr<BClass>> BScope::findAllClasses(const char* name) {
    std::vector<std::shared_ptr<BClass>> classes;
    for(auto scope : m_scopes) {
        std::shared_ptr<BClass> classScope = std::dynamic_pointer_cast<BClass>(scope.second);
        if(classScope) {
            if(!classScope->getName()) {
                throw BException("Finding classes requires a class name to be defined");
            }

            if(!name || (classScope->getName()->getValue() == name)) {
                classes.push_back(classScope);
            }
        }
    }
    return classes;
}

std::vector<std::shared_ptr<BFunction>> BScope::findAllFunctions(const char *name) {
    std::vector<std::shared_ptr<BFunction>> functions;
    for(auto scope : m_scopes) {
        std::shared_ptr<BFunction> functionScope = std::dynamic_pointer_cast<BFunction>(scope.second);
        if(functionScope) {
            if(!functionScope->getName()) {
                throw BException("Finding functions requires a function name to be defined");
            }

            if (!name || (functionScope->getName()->getValue() == name)) {
                functions.push_back(functionScope);
            }
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

std::stringstream BScope::getStructure() {
    std::stringstream structure;
    std::stack<std::shared_ptr<BScope>> structureStack;
    structureStack.push(shared_from_this());
    while(!structureStack.empty()) {

        // Get the top of the stack
        std::shared_ptr<BScope> top = structureStack.top();
        structureStack.pop();

        structure << "Scope: " << top->getLabel().str() << std::endl;
        for(auto variable : top->findAllVariables()) {
            structure << "Variable: " << variable->getLabel().str() << std::endl;
        }

        // Push all children scopes
        for(auto scope : top->m_scopes) {
            structureStack.push(scope.second);
        }
    }
    return structure;
}

void BScope::registerClass(unsigned int referenceKey, std::shared_ptr<BClass> classScope) {

    // Class name is required
    if(!classScope->getName()) {
        throw BException("Cannot register a class without specifying its name first");
    }

    // Check if class was added previously
    auto getClass = findAllClasses(classScope->getName()->getValue().c_str());
    if(!getClass.empty()) {
        BReport::getInstance().error()
                << "Class " << classScope->getName()->getValue() << " at line "
                << classScope->getName()->getLine() << " was defined previously" << std::endl;
        BReport::getInstance().printError();
    }

    // Register class in this scope
    registerScope(referenceKey, classScope);
}

void BScope::registerFunction(unsigned int referenceKey, std::shared_ptr<BFunction> functionScope) {

    // Function name is required
    if(!functionScope->getName()) {
        throw BException("Cannot register a function without specifying its name first");
    }

    // Check if function was added previously
    auto getFunc = findAllFunctions(functionScope->getName()->getValue().c_str());
    if(!getFunc.empty()) {
        BReport::getInstance().error()
                << "Function " << functionScope->getName()->getValue() << " at line "
                << functionScope->getName()->getLine()
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
        auto function = findClosestFunction();
        BReport::getInstance().error()
                << "Multiple return statements are set within the same scope in function "
                << function << std::endl;
        BReport::getInstance().printError();
    } else {

        // Set return statement in this scope
        ret->setParentScope(shared_from_this());
        m_return = ret;

        // Verify return statement
        ret->verifyReturn();
    }
}

void BScope::registerVariableAssign(unsigned int referenceKey, std::shared_ptr<BVariableAssign> variableAssign) {
    m_variablesAssignments[referenceKey] = variableAssign;
    variableAssign->setParentScope(shared_from_this());
}

std::shared_ptr<BVariableAssign> BScope::getVariableAssignByReferenceKey(unsigned int referenceKey) {
    if(m_variablesAssignments.find(referenceKey) == m_variablesAssignments.end()) {
        throw BException("Requesting a variable assignment with an unrecognized reference key");
    }
    return m_variablesAssignments[referenceKey];
}

void BScope::registerFunctionCall(unsigned int referenceKey, std::shared_ptr<BFunctionCall> functionCall) {
    m_functionCalls[referenceKey] = functionCall;
    functionCall->setParentScope(shared_from_this());
}

std::shared_ptr<BFunctionCall> BScope::getFunctionCallByReferenceKey(unsigned int referenceKey) {
    if(m_functionCalls.find(referenceKey) == m_functionCalls.end()) {
        throw BException("Requesting a variable assignment with an unrecognized reference key");
    }
    return m_functionCalls[referenceKey];
}

bool BScope::hasReturn() {

    // Check if a return statement has been set in this scope
    if(m_return) {
        return true;
    }

    // TODO Check children if/elseif/else scopes if all of them have a return statement
    return false;
}