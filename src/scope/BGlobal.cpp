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
        }
    }

    for(auto function : findAllFunctions()) {
        function->getType()->linkType();
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
    }
}