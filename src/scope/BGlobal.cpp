#include <bashclass/BGlobal.h>
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

    for(auto cls : findAllClasses()) {

        for(auto variable : cls->findAllVariables()) {
            variable->getType()->linkType();
        }

        for(auto function : cls->findAllFunctions()) {
            function->getType()->linkType();

            for(auto variable : function->findAllParameters()) {
                variable->getType()->linkType();
            }
        }
    }

    for(auto function : findAllFunctions()) {
        function->getType()->linkType();

        for(auto variable : function->findAllParameters()) {
            variable->getType()->linkType();
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
        if(!mainFunction->getType()->isInt()) {
            BReport::getInstance().error()
                    << "Main function must return an integer" << std::endl;
            BReport::getInstance().printError();
        }

        // Check params: int, char[][]
        auto params = mainFunction->findAllParameters();
        if(params.size() != 0 && params.size() != 2) {
            BReport::getInstance().error()
                    << "Main function parameters must either be empty or (int, char[][])" << std::endl;
            BReport::getInstance().printError();
        } else if(params.size() == 2) {

            // int
            if(!params[0]->getType()->isInt() || params[0]->getType()->getDimension() != 0) {
                BReport::getInstance().error()
                        << "First parameter in main function must be of type int" << std::endl;
                BReport::getInstance().printError();
            }

            // char[][]
            if(!params[1]->getType()->isChar() || params[1]->getType()->getDimension() != 2) {
                BReport::getInstance().error()
                        << "Second parameter in main function must be of type char[][]" << std::endl;
                BReport::getInstance().printError();
            }

            // Params should not have default expression
            if(params[0]->getExpression() || params[1]->getExpression()) {
                BReport::getInstance().error()
                        << "Main function parameters cannot have a default expression" << std::endl;
                BReport::getInstance().printError();
            }
        }
    }
}