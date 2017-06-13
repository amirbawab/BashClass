#include <bashclass/BGlobal.h>
#include <bashclass/BTypes.h>
#include <iostream>
#include <stack>
#include <set>
#include <bashclass/BReport.h>
#include <bashclass/BVariable.h>
#include <bashclass/BException.h>

const std::string BGlobal::MAIN_FUNCTION = "main";

std::stringstream BGlobal::getLabel() {
    return std::stringstream();
}

void BGlobal::linkTypes() {

    std::stack<std::shared_ptr<BScope>> scopeStack;
    scopeStack.push(shared_from_this());
    while(!scopeStack.empty()) {

        auto top = scopeStack.top();
        scopeStack.pop();

        // Link all variables in that scope
        for(auto variable : top->findAllVariables()) {
            if(!BType::isBuiltInType(variable->getType()->getName())) {
                // Find class scope of that type
                auto cls = findAllClasses(variable->getType()->getValue().c_str());
                if(cls.empty()) {
                    BReport::getInstance().error()
                            << "Undefined type " << variable->getType()->getValue() <<
                            " for variable " << variable->getName()->getValue() << std::endl;
                    BReport::getInstance().printError();
                } else {
                    variable->setTypeScope(cls.front());
                }
            }
        }

        // Link all functions in that scope
        for(auto function : top->findAllFunctions()) {
            if(!BType::isBuiltInType(function->getType()->getName())) {
                // Find class scope of that type
                auto cls = findAllClasses(function->getType()->getValue().c_str());
                if(cls.empty()) {
                    BReport::getInstance().error()
                            << "Undefined type " << function->getType()->getValue() <<
                            " for function " << function->getName()->getValue() << std::endl;
                    BReport::getInstance().printError();
                } else {
                    function->setTypeScope(cls.front());
                }
            }
        }

        // Push all children scopes into the stack
        for(auto scope : top->getAllScopes()) {
            scopeStack.push(scope);
        }
    }
}

void BGlobal::verifyMain() {
    auto functions = findAllFunctions(BGlobal::MAIN_FUNCTION.c_str());
    if(functions.empty()) {
        BReport::getInstance().error()
                << "A main function is required" << std::endl;
        BReport::getInstance().printError();
    } else {
        auto mainFunction = functions.front();
        if(!mainFunction->getType()) {
            throw BException("Cannot verify main function without a defined type");
        }

        // Main function must return an integer
        if(mainFunction->getType()->getName() != BType::TYPE_NAME_INT) {
            BReport::getInstance().error()
                    << "Main function must return an integer" << std::endl;
            BReport::getInstance().printError();

        }
    }
}