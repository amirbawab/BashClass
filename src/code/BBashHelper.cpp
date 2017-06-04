#include <bashclass/BBashHelper.h>
#include <bashclass/BGenerateCode.h>
#include <bashclass/BTypes.h>
#include <bashclass/BException.h>
#include <bashclass/BGlobal.h>
#include <bashclass/BVariableCall.h>
#include <bashclass/BFunctionCall.h>
#include <iostream>

// Constants
std::string FUNCTION_THIS = "_this_";
std::string FUNCTION_RETURN = "_return_";
std::string RESULT = "_result_";

void _indent(std::shared_ptr<BScope> parent, std::stringstream& ss) {
    if(!parent) {
        throw BException("Parent scope should not be empty");
    }

    auto closestClass = parent->findClosestClass();
    while (parent != BGlobal::getInstance() && parent != closestClass) {
        ss << "\t";
        parent = parent->getParentScope();
    }
}

std::string _generateResultKey(unsigned int number) {
    return RESULT + std::to_string(number);
}

std::string _chainCallToCode(std::shared_ptr<BChainCall> chainCall) {

    // Hold the code for the chain call
    std::stringstream ss;

    // Keep track of the previous element type
    std::shared_ptr<BScope> prevTypeScope;

    // Have a unique key for each result of an element call
    std::map<std::shared_ptr<IBCallable>, std::string> returnMap;

    // Assign a unique key for each result
    static unsigned int uniqueId = 0;

    // Start processing the calls
    for (size_t i = 0; i < chainCall->size(); i++) {

        // Cast element
        auto variableCallCast = std::dynamic_pointer_cast<BVariableCall>((*chainCall)[i]);
        auto functionCallCast = std::dynamic_pointer_cast<BFunctionCall>((*chainCall)[i]);
        auto thisCallCast = std::dynamic_pointer_cast<BThisCall>((*chainCall)[i]);

        // Generate indentation
        _indent(chainCall->getParentScope(), ss);

        if (variableCallCast) {

            // Generate new key for this variable
            std::string newKey = _generateResultKey(uniqueId++);
            ss << newKey << "=";

            // Check if element is the first one in the chain
            if(i == 0) {

                // Class members have different value than other variables for the first assign statement
                if(variableCallCast->getVariable()->isClassMember()) {
                    // Get class parent of this variable
                    auto classCast = variableCallCast->getVariable()->getParentScope()->findClosestClass();
                    ss << classCast->getLabel().str() << "[" << FUNCTION_THIS << "," << variableCallCast->getVariable()->getLabel().str() << "]";
                } else {
                    ss << variableCallCast->getVariable()->getLabel().str();
                }
            } else {
                // TODO Add exception if previous call was not found in the map
                ss << prevTypeScope->getLabel().str() << "[" << returnMap[(*chainCall)[i-1]] << "," << variableCallCast->getVariable()->getLabel().str() << "]";
            }

            ss << std::endl;

            // Store the new key in the map
            returnMap[variableCallCast] = newKey;

            // Update the type of the previous scope
            prevTypeScope = variableCallCast->getVariable()->getTypeScope();

        } else if (functionCallCast) {

            // Check if function call is the first call in the chain
            if(i == 0) {
                ss << functionCallCast->getFunction()->getLabel().str();

                // TODO Put all arguments
                ss << " args...";

                if(functionCallCast->getFunction()->isClassMember()) {
                    ss << " " << FUNCTION_THIS;
                }

                if(functionCallCast->getFunction()->requiresReturn()) {
                    std::string newKey = _generateResultKey(uniqueId++);
                    returnMap[functionCallCast] = newKey;
                    ss << " " << newKey;
                }
            } else {
                ss << functionCallCast->getFunction()->getLabel().str();

                // TODO Put all arguments
                ss << " args...";

                ss << " " << returnMap[(*chainCall)[i-1]];

                if(functionCallCast->getFunction()->requiresReturn()) {
                    std::string newKey = _generateResultKey(uniqueId++);
                    returnMap[functionCallCast] = newKey;
                    ss << " " << newKey;
                }
            }

            ss << std::endl;

            // Update the type of the previous scope
            prevTypeScope = functionCallCast->getFunction()->getTypeScope();
        } else if(thisCallCast) {

            if(i > 0) {
                throw BException("A 'this' reference is supported only when it appears first at the chain");
            }

            std::string newKey = _generateResultKey(uniqueId++);
            returnMap[thisCallCast] = newKey;
            ss  << newKey << "=" << FUNCTION_THIS << std::endl;

            // Update the type of the previous scope
            prevTypeScope = thisCallCast->getReference();
        } else {
            throw BException("Cannot generate code for an unrecognized element call");
        }
    }
    return ss.str();
}

void BBashHelper::header() {
    BGenerateCode::get().writePreCode();
}

void BBashHelper::footer() {
    BGenerateCode::get().writePostCode();
}

void BBashHelper::declareClass(std::shared_ptr<BClass> classScope) {
    std::stringstream ss;
    ss << "declare -A " << classScope->getLabel().str() << "=()" << std::endl;
    BGenerateCode::get().write(ss);
}

void BBashHelper::uniqueCounter(std::shared_ptr<BClass> classScope) {
    std::stringstream ss;
    ss << "_" << classScope->getName()->getValue() << "_=0" << std::endl;
    BGenerateCode::get().write(ss);
}

void BBashHelper::createGlobalVar(std::shared_ptr<BVariable> variable, std::string defaultValue) {
    std::stringstream ss;
    ss << variable->getLabel().str() << "=" << defaultValue << std::endl;
    BGenerateCode::get().write(ss);
}

void BBashHelper::createLocalVar(std::shared_ptr<BVariable> variable, std::string defaultValue) {
    std::stringstream ss;
    _indent(variable->getParentScope(), ss);
    ss << "local " << variable->getLabel().str() << "=" << defaultValue << std::endl;
    BGenerateCode::get().write(ss);
}

void BBashHelper::bash(std::shared_ptr<ecc::LexicalToken> token) {
    std::stringstream ss;
    if(token->getName() == BType::TYPE_NAME_BASH_INLINE) {
        ss << token->getValue().substr(2,token->getValue().length()-2);
    } else if(token->getName() == BType::TYPE_NAME_BASH_SUB || token->getName() == BType::TYPE_NAME_BASH_BLOCK) {
        ss << token->getValue().substr(2, token->getValue().length()-4);
    } else {
        throw BException("Bash code can only be generated by bash lexical tokens");
    }
    ss << std::endl;
    BGenerateCode::get().write(ss);
}

void BBashHelper::createFunction(std::shared_ptr<BFunction> function) {
    std::stringstream ss;
    ss << "function " << function->getLabel().str() << "() {" << std::endl;

    // Create parameters
    int paramPos = 1;
    for(auto param : function->findAllParameters()) {
        _indent(param->getParentScope(), ss);
        ss << "local " << param->getLabel().str() << "=\"$" << paramPos++ << "\"" << std::endl;
    }

    // Generate 'this' if function is a class member
    if(function->isClassMember()) {
        _indent(function, ss);
        ss << "local " << FUNCTION_THIS << "=$" << paramPos++ << std::endl;
    }

    // Generate return reference if function requires a return statement
    if(function->requiresReturn()) {
        _indent(function, ss);
        ss << "declare -n " << FUNCTION_RETURN << "=$" << paramPos++ << std::endl;
    }

    BGenerateCode::get().write(ss);
}

void BBashHelper::closeFunction(std::shared_ptr<BFunction> function) {
    std::stringstream ss;
    _indent(function->getParentScope(), ss);
    ss << "}" << std::endl << std::endl;
    BGenerateCode::get().write(ss);
}

void BBashHelper::assignVariable(std::shared_ptr<BChainCall> chainCall) {
    std::stringstream ss;

    // Convert chain to code
    ss << _chainCallToCode(chainCall) << "=" << std::endl;
    BGenerateCode::get().write(ss);
}