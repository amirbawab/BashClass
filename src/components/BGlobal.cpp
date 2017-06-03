#include <bashclass/BGlobal.h>
#include <bashclass/BClass.h>
#include <bashclass/BTypes.h>
#include <iostream>
#include <stack>
#include <set>
#include <bashclass/BReport.h>

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

void BGlobal::detectCircularReference() {

    for(auto cls : findAllClasses()) {
        std::stack<std::shared_ptr<BScope>> scopeStack;
        scopeStack.push(cls);
        std::set<std::shared_ptr<BScope>> visited;
        while(!scopeStack.empty()) {

            // Get top class scope
            auto top = scopeStack.top();
            scopeStack.pop();

            // Mark class as visited
            visited.insert(top);

            // Evaluate all variables in the class scope
            for(auto variable : top->findAllVariables()) {
                if(variable->getTypeScope()) {
                    if(visited.find(variable->getTypeScope()) == visited.end()) {
                        scopeStack.push(variable->getTypeScope());
                    } else {
                        BReport::getInstance().error()
                                << "Circular reference detected from class " << cls->getName()->getValue() << std::endl;
                        BReport::getInstance().printError();
                    }
                }
            }
        }
    }
}
