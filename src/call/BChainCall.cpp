#include <bashclass/BChainCall.h>
#include <bashclass/BVariableCall.h>
#include <bashclass/BFunctionCall.h>
#include <bashclass/BClass.h>
#include <iostream>
#include <bashclass/BException.h>

std::string BChainCall::getTypeValueAsString() {
    return last()->getTypeValueAsString();
}

/**
 * Get the type of the previous item in the chain
 * @param prevVariableCall
 * @param prevFunctionCall
 * @return pointer to the type scope
 */
std::shared_ptr<BScope> getPrevItemTypeScope(std::shared_ptr<BVariableCall> prevVariableCall,
                                             std::shared_ptr<BFunctionCall> prevFunctionCall) {
    if(prevVariableCall) {
        return prevVariableCall->getVariable()->getTypeScope();
    } else if(prevFunctionCall) {
        return prevFunctionCall->getFunction()->getTypeScope();
    } else {
        throw BException("Undefined call on an unknown instance");
    }
}

void BChainCall::addVariable(std::shared_ptr<BScope> scope, std::shared_ptr<ecc::LexicalToken> token) {

    // Prepare variable call
    std::shared_ptr<BVariableCall> variableCall = std::make_shared<BVariableCall>();
    variableCall->setLexicalToken(token);

    // Check if the variable is the first callable item to be added
    if(m_callables.empty()) {
        auto variable = scope->findClosestVariable(token->getValue());
        if(variable) {
            // Check if the variable is a class member
            if(variable->isClassMember()) {
                std::cerr << "Use the first parameter of the function to refer to the variable "
                          << variable->getName()->getValue() << " at line " << token->getLine() << " and column "
                          << token->getColumn() << std::endl;
            }

            // Set variable anyway since it's clear what the user wants
            variableCall->setVariable(variable);
        } else {
            std::cerr << "Undefined variable " << token->getValue()
                      << " at line " << token->getLine() << " and column "
                      << token->getColumn() << std::endl;
        }
    } else {
        auto prevVariableCall = std::dynamic_pointer_cast<BVariableCall>(last());
        auto prevFunctionCall = std::dynamic_pointer_cast<BFunctionCall>(last());
        if((prevVariableCall && !prevVariableCall->getVariable())
           || (prevFunctionCall && !prevFunctionCall->getFunction())) {
            std::cerr << "Cannot access variable member " << token->getValue()
                      << " of undefined at line " << token->getLine() << " and column "
                      << token->getColumn() << std::endl;
        } else {
            std::shared_ptr<BScope> typeScope = getPrevItemTypeScope(prevVariableCall, prevFunctionCall);
            if(typeScope) {
                auto variables = typeScope->findAllVariables(token->getValue().c_str());
                if(!variables.empty()) {
                    variableCall->setVariable(variables.front());
                } else {
                    auto castClass = std::dynamic_pointer_cast<BClass>(typeScope);
                    std::cerr << "Class " << castClass->getName()->getValue() << " does not have a variable member "
                              << token->getValue() << " at line "
                              << token->getLine() << " and column "
                              << token->getColumn() << std::endl;
                }
            } else {
                std::cerr << "Variable member " << token->getValue()
                          << " at line " << token->getLine()
                          << " and column " << token->getColumn()
                          << " does not exist. Previous element type does not have any known members." << std::endl;
            }
        }
    }

    // Add variable to the chain
    m_callables.push_back(variableCall);
}

void BChainCall::addFunction(std::shared_ptr<BScope> globalScope, std::shared_ptr<ecc::LexicalToken> token) {

    // Prepare function call
    auto functionCall = std::make_shared<BFunctionCall>();
    functionCall->setLexicalToken(token);

    // Check if the function is the first callable item to be added
    if(m_callables.empty()) {
        auto functions = globalScope->findAllFunctions(token->getValue().c_str());
        if(!functions.empty()) {
            functionCall->setFunction(std::dynamic_pointer_cast<BFunction>(functions.front()));
        } else {
            std::cerr << "Undefined function " << token->getValue()
                      << " at line " << token->getLine() << " and column "
                      << token->getColumn() << std::endl;

        }
    } else {
        auto prevVariableCall = std::dynamic_pointer_cast<BVariableCall>(last());
        auto prevFunctionCall = std::dynamic_pointer_cast<BFunctionCall>(last());
        if((prevVariableCall && !prevVariableCall->getVariable())
           || (prevFunctionCall && !prevFunctionCall->getFunction())) {
            std::cerr << "Cannot access function member " << token->getValue()
                      << " of undefined at line " << token->getLine() << " and column "
                      << token->getColumn() << std::endl;
        } else {
            std::shared_ptr<BScope> typeScope = getPrevItemTypeScope(prevVariableCall, prevFunctionCall);
            if(typeScope) {
                auto functions = typeScope->findAllFunctions(token->getValue().c_str());
                if(!functions.empty()) {
                    auto function = std::dynamic_pointer_cast<BFunction>(functions.front());
                    functionCall->setFunction(function);
                } else {
                    auto castClass = std::dynamic_pointer_cast<BClass>(typeScope);
                    std::cerr << "Class " << castClass->getName()->getValue() << " does not have a function member "
                              << token->getValue() << " at line "
                              << token->getLine() << " and column "
                              << token->getColumn() << std::endl;
                }
            } else {
                std::cerr << "Function member " << token->getValue()
                          << " at line " << token->getLine()
                          << " and column " << token->getColumn()
                          << " does not exist. Previous element type does not have any known members." << std::endl;
            }
        }
    }

    // Add function to the chain
    m_callables.push_back(functionCall);
}

void BChainCall::addToken(std::shared_ptr<BTokenCall> token) {
    m_callables.push_back(token);
}