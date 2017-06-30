#include <bashclass/BChain.h>
#include <bashclass/BVariableChainAccess.h>
#include <bashclass/BFunctionChainCall.h>
#include <iostream>
#include <bashclass/BException.h>
#include <bashclass/BReport.h>
#include <bashclass/BGlobal.h>
#include <bashclass/BSuperConstructorChainCall.h>
#include <bashclass/BConstructorChainCall.h>

void BChain::addVariable(std::shared_ptr<BScope> scope, std::shared_ptr<ecc::LexicalToken> token) {

    // Prepare variable call
    std::shared_ptr<BVariableChainAccess> variableCall = std::make_shared<BVariableChainAccess>();
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
        if(!lastElement->isFound()) {
            BReport::getInstance().error()
                    << "Cannot access variable member " << token->getValue()
                    << " of undefined at line " << token->getLine() << " and column "
                    << token->getColumn() << std::endl;
            BReport::getInstance().printError();
        } else {

            // Get the type of the previous element
            std::shared_ptr<IBType> lastType = lastElement->getType();

            // Check the type of the previous element
            if(lastType->isIdentifier()) {

                // If type was found
                if(lastType->getTypeScope()) {

                    // Search for the new variable in class and its parent classes
                    std::shared_ptr<BClass> tmpClass = lastType->getTypeScope();
                    std::vector<std::shared_ptr<BVariable>> variables;
                    while(variables.empty() && tmpClass) {
                        variables = tmpClass->findAllVariables(token->getValue().c_str());
                        tmpClass = tmpClass->getExtends();
                    }

                    // Check if variable was found
                    if(!variables.empty()) {
                        variableCall->setVariable(variables.front());
                    } else {
                        BReport::getInstance().error()
                                << "Class " << lastType->getTypeScope()->getName()->getValue()
                                << " does not have a variable member "
                                << token->getValue() << " at line "
                                << token->getLine() << " and column "
                                << token->getColumn() << std::endl;
                        BReport::getInstance().printError();
                    }
                } else {
                    BReport::getInstance().error()
                            << "Cannot find variable member "
                            << token->getValue() << " at line "
                            << token->getLine() << " and column "
                            << token->getColumn() << " because the previous element has an undefined type"
                            << std::endl;
                    BReport::getInstance().printError();
                }
            } else if(lastType->isBuiltInType()) {
                BReport::getInstance().error()
                        << "Variable member " << token->getValue()
                        << " at line " << token->getLine()
                        << " and column " << token->getColumn()
                        << " does not exist. Previous element type does not have any known members." << std::endl;
                BReport::getInstance().printError();
            } else {
                throw BException("Cannot verify new variable type because previous element is of an unrecognized type");
            }
        }
    }

    // Add variable to the chain
    m_callables.push_back(variableCall);
}

void BChain::addFunction(std::shared_ptr<BScope> scope, std::shared_ptr<ecc::LexicalToken> token) {

    // Prepare function call
    auto functionCall = std::make_shared<BFunctionChainCall>();
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
        if(!lastElement->isFound()) {
            BReport::getInstance().error()
                    << "Cannot access function member " << token->getValue()
                    << " of undefined at line " << token->getLine() << " and column "
                    << token->getColumn() << std::endl;
            BReport::getInstance().printError();
        } else {

            // Get the type of the previous element
            auto lastType = lastElement->getType();

            // Check the type of the previous element
            if(lastType->isIdentifier()) {

                // Check if type was found
                if(lastType->getTypeScope()) {
                    auto functions = lastType->getTypeScope()->findAllFunctions(token->getValue().c_str());
                    if(!functions.empty()) {
                        functionCall->setFunction(functions.front());
                    } else {
                        BReport::getInstance().error()
                                << "Class " << lastType->getTypeScope()->getName()->getValue()
                                << " does not have a function member "
                                << token->getValue() << " at line "
                                << token->getLine() << " and column "
                                << token->getColumn() << std::endl;
                        BReport::getInstance().printError();
                    }
                } else {
                    BReport::getInstance().error()
                            << "Cannot find function member "
                            << token->getValue() << " at line "
                            << token->getLine() << " and column "
                            << token->getColumn() << " because the previous element has an undefined type"
                            << std::endl;
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

void BChain::addConstructor(std::shared_ptr<BScope> scope, std::shared_ptr<ecc::LexicalToken> token) {

    // Prepare function call
    auto functionCall = std::make_shared<BConstructorChainCall>();
    functionCall->setLexicalToken(token);

    if(!m_callables.empty()) {
        throw BException("A constructor call must be at the beginning of the chain");
    }

    // Search for the class and constructor
    auto classes = BGlobal::getInstance()->findAllClasses(token->getValue().c_str());
    if(classes.empty()) {
        BReport::getInstance().error()
                << "Undefined class " << token->getValue() << " at line " << token->getLine()
                << " and column " << token->getColumn() << std::endl;
        BReport::getInstance().printError();
    } else {

        // Search for the constructor
        auto functions = classes.front()->findAllConstructors();

        // Set function if found
        if(functions.empty()) {
            BReport::getInstance().error()
                    << "Call to an undefined constructor for class " << token->getValue() << " at line " << token->getLine()
                    << " and column " << token->getColumn() << std::endl;
            BReport::getInstance().printError();
        } else {
            functionCall->setFunction(functions.front());
        }
    }

    // Add function to the chain
    m_callables.push_back(functionCall);
}

void BChain::addSuperConstructor(std::shared_ptr<BScope> scope, std::shared_ptr<ecc::LexicalToken> token) {

    // Prepare function call
    auto functionCall = std::make_shared<BSuperConstructorChainCall>();
    functionCall->setLexicalToken(token);

    if(!m_callables.empty()) {
        throw BException("A super constructor call must be at the beginning of the chain");
    }

    // Search for the class and constructor
    auto classScope = scope->findClosestClass();
    if(!classScope) {
        BReport::getInstance().error()
                << "Cannot call super constructor outside a constructor function at line " << token->getLine()
                << " and column " << token->getColumn() << std::endl;
        BReport::getInstance().printError();
    } else {

        // Check if super constructor is defined
        if(!classScope->getExtends()) {
            BReport::getInstance().error()
                    << "Cannot call super constructor at line " << token->getLine()
                    << " and column " << token->getColumn() << " because class " << classScope->getName()->getValue()
                    << " does not extend from another class" << std::endl;
            BReport::getInstance().printError();
        } else {

            // Search for the constructor
            auto functions = classScope->getExtends()->findAllConstructors();

            // Set function if found
            if(functions.empty()) {
                BReport::getInstance().error()
                        << "Call to an undefined constructor for class "
                        << classScope->getExtends()->getName()->getValue() << " at line " << token->getLine()
                        << " and column " << token->getColumn() << std::endl;
                BReport::getInstance().printError();
            } else {
                functionCall->setFunction(functions.front());
            }
        }
    }

    // Add function to the chain
    m_callables.push_back(functionCall);
}

void BChain::addThis(std::shared_ptr<BScope> scope, std::shared_ptr<BThisChainAccess> thisReference) {

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

void BChain::addSuper(std::shared_ptr<BScope> scope, std::shared_ptr<BSuperChainAccess> superReference) {

    // Callable chain must be empty
    if(!m_callables.empty()) {
        throw BException("A 'super' reference must always be at the beginning of the chain");
    }

    // Find the parent class of the this reference
    auto classScope = scope->findClosestClass();

    // Check if a parent class was found
    if(classScope) {

        // Set the source reference
        superReference->setSrcReference(classScope);

        // Check if class extends from another
        if(!classScope->getExtends()) {
            BReport::getInstance().error()
                    << "Cannot call 'super' at line " << superReference->getLexicalToken()->getLine()
                    << " and column " << superReference->getLexicalToken()->getColumn() << " because class "
                    << classScope->getName()->getValue() << " does not extend from another class" << std::endl;
            BReport::getInstance().printError();
        }
    } else {
        BReport::getInstance().error()
                << "Undefined reference for 'super' at line " << superReference->getLexicalToken()->getLine()
                << " and column " << superReference->getLexicalToken()->getColumn() << std::endl;
        BReport::getInstance().printError();
    }

    // Add this reference to the chain anyway
    m_callables.push_back(superReference);
}

std::shared_ptr<IBChainable> BChain::operator[](int index) {
    if(index < 0 || index >= m_callables.size()) {
        throw BException("Accessing an index out of bound in a chain call");
    }
    return m_callables[index];
}

std::shared_ptr<IBChainable> BChain::last() {
    if(m_callables.empty()) {
        throw BException("Accessing last element of an empty chain");
    }
    return m_callables.back();
}
