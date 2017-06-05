#include <bashclass/BChainCall.h>
#include <bashclass/BVariableChainCall.h>
#include <bashclass/BFunctionCall.h>
#include <bashclass/BClass.h>
#include <iostream>
#include <bashclass/BException.h>
#include <bashclass/BReport.h>
#include <bashclass/BGlobal.h>

std::string BChainCall::getTypeValueAsString() {
    return last()->getTypeValueAsString();
}

void BChainCall::addVariable(std::shared_ptr<BScope> scope, std::shared_ptr<ecc::LexicalToken> token) {

    // Prepare variable call
    std::shared_ptr<BVariableChainCall> variableCall = std::make_shared<BVariableChainCall>();
    variableCall->setLexicalToken(token);

    // Check if the variable is the first callable item to be added
    if(m_callables.empty()) {
        auto variable = scope->findClosestVariable(token->getValue());
        if(variable) {
            variableCall->setVariable(variable);
        } else {
            BReport::getInstance().error()
                    << "Undefined variable " << token->getValue()
                    << " at line " << token->getLine() << " and column "
                    << token->getColumn() << std::endl;
            BReport::getInstance().printError();
        }
    } else {

        // Cast previous element
        auto lastElement = last();

        // Check if previous element is recognized
        if(!lastElement->isKnown()) {
            BReport::getInstance().error()
                    << "Cannot access variable member " << token->getValue()
                    << " of undefined at line " << token->getLine() << " and column "
                    << token->getColumn() << std::endl;
            BReport::getInstance().printError();
        } else {

            // Get the type of the previous element
            auto typeScope = lastElement->getTypeScope();

            // Check if the type is defined
            if(typeScope) {
                auto variables = typeScope->findAllVariables(token->getValue().c_str());
                if(!variables.empty()) {
                    variableCall->setVariable(variables.front());
                } else {
                    BReport::getInstance().error()
                            << "Class " << typeScope->getName()->getValue()
                            << " does not have a variable member "
                            << token->getValue() << " at line "
                            << token->getLine() << " and column "
                            << token->getColumn() << std::endl;
                    BReport::getInstance().printError();
                }
            } else {
                BReport::getInstance().error()
                        << "Variable member " << token->getValue()
                        << " at line " << token->getLine()
                        << " and column " << token->getColumn()
                        << " does not exist. Previous element type does not have any known members." << std::endl;
                BReport::getInstance().printError();
            }
        }
    }

    // Add variable to the chain
    m_callables.push_back(variableCall);
}

void BChainCall::addFunction(std::shared_ptr<BScope> scope, std::shared_ptr<ecc::LexicalToken> token) {

    // Prepare function call
    auto functionCall = std::make_shared<BFunctionCall>();
    functionCall->setLexicalToken(token);

    // Check if the function is the first callable item to be added
    if(m_callables.empty()) {

        // If function call is inside a class, then start searching for a function member of that class
        auto classScope = scope->findClosestClass();
        if(classScope) {
            auto functions = classScope->findAllFunctions(token->getValue().c_str());
            if(!functions.empty()) {
                functionCall->setFunction(functions.front());
            }
        }

        // If not a class member, then search in the global scope
        if(!functionCall->getFunction()) {
            auto functions = BGlobal::getInstance()->findAllFunctions(token->getValue().c_str());
            if(!functions.empty()) {
                functionCall->setFunction(functions.front());
            } else {
                BReport::getInstance().error()
                        << "Undefined function " << token->getValue()
                        << " at line " << token->getLine() << " and column "
                        << token->getColumn() << std::endl;
                BReport::getInstance().printError();
            }

        }
    } else {

        // Cast previous element
        auto lastElement = last();

        // Check if the previous element is recognized
        if(!lastElement->isKnown()) {
            BReport::getInstance().error()
                    << "Cannot access function member " << token->getValue()
                    << " of undefined at line " << token->getLine() << " and column "
                    << token->getColumn() << std::endl;
            BReport::getInstance().printError();
        } else {

            // Get the type of the previous element
            auto typeScope = lastElement->getTypeScope();

            // Check if the type is defined
            if(typeScope) {
                auto functions = typeScope->findAllFunctions(token->getValue().c_str());
                if(!functions.empty()) {
                    functionCall->setFunction(functions.front());
                } else {
                    BReport::getInstance().error()
                            << "Class " << typeScope->getName()->getValue() << " does not have a function member "
                            << token->getValue() << " at line "
                            << token->getLine() << " and column "
                            << token->getColumn() << std::endl;
                    BReport::getInstance().printError();
                }
            } else {
                BReport::getInstance().error()
                        << "Function member " << token->getValue()
                        << " at line " << token->getLine()
                        << " and column " << token->getColumn()
                        << " does not exist. Previous element type does not have any known members." << std::endl;
                BReport::getInstance().printError();
            }
        }
    }

    // Add function to the chain
    m_callables.push_back(functionCall);
}

void BChainCall::addToken(std::shared_ptr<BTokenCall> token) {
    m_callables.push_back(token);
}

void BChainCall::addThis(std::shared_ptr<BScope> scope, std::shared_ptr<BThisChainCall> thisReference) {

    // Callable chain must be empty
    if(!m_callables.empty()) {
        throw BException("A 'this' reference must always be at the beginning of the chain");
    }

    // Find the parent class of the this reference
    auto classScope = scope->findClosestClass();

    // Check if a parent class was found
    if(classScope) {
        thisReference->setReference(classScope);
    } else {
        BReport::getInstance().error()
                << "Undefined reference for 'this' at line " << thisReference->getLexicalToken()->getLine()
                << " and column " << thisReference->getLexicalToken()->getColumn() << std::endl;
        BReport::getInstance().printError();
    }

    // Add this reference to the chain anyway
    m_callables.push_back(thisReference);
}

std::shared_ptr<IBSimpleCallable> BChainCall::operator[](int index) {
    if(index < 0 || index >= m_callables.size()) {
        throw BException("Accessing an index out of bound in a chain call");
    }
    return m_callables[index];
}
