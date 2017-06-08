#include <bashclass/BBashHelper.h>
#include <bashclass/BGenerateCode.h>
#include <bashclass/BTypes.h>
#include <bashclass/BException.h>
#include <bashclass/BGlobal.h>
#include <bashclass/BVariableChainAccess.h>
#include <bashclass/BFunctionChainCall.h>
#include <iostream>
#include <bashclass/BArithOperation.h>
#include <bashclass/BVariableAssign.h>

// Constants
std::string FUNCTION_THIS = "_this_";
std::string FUNCTION_RETURN = "_return_";
std::string RESULT = "_result_";

/**
 * Indent generated code
 * @param parent
 * @param ss
 */
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

/**
 * Generate a unique result key/variable per element call
 * @param number
 * @return unique result string
 */
std::string _generateResultKey(unsigned int number) {
    return RESULT + std::to_string(number);
}

/**
 * Generate code for a variable chain access that is a member and is placed first in the chain
 * @param variableChainAccess
 * @param ss
 */
void _varChainAccess_member_first(std::shared_ptr<BVariableChainAccess> variableChainAccess, std::stringstream &ss) {
    ss << variableChainAccess->getVariable()->getParentScope()->findClosestClass()->getLabel().str()
       << "[" << FUNCTION_THIS << "," << variableChainAccess->getVariable()->getLabel().str() << "]";
}

/**
 * Generate code for a variable chain access that is a member and is placed in the middle of the chain
 * @param variableChainAccess
 * @param prevTypeLabel
 * @param prevResult
 * @param ss
 */
void _varChainAccess_member_middle(std::shared_ptr<BVariableChainAccess> variableChainAccess, std::string prevTypeLabel,
                            std::string prevResult, std::stringstream &ss) {
    ss << prevTypeLabel << "[" << prevResult << "," << variableChainAccess->getVariable()->getLabel().str() << "]" ;
}

/**
 * Generate code for a variable chain access that is a member and is placed last in the chain
 * @param variableChainAccess
 * @param prevResult
 * @param ss
 */
void _varChainAccess_member_last(std::shared_ptr<BVariableChainAccess> variableChainAccess,
                          std::string prevResult, std::stringstream &ss) {
    ss << variableChainAccess->getVariable()->getParentScope()->findClosestClass()->getLabel().str()
       << "[" << prevResult << "," << variableChainAccess->getVariable()->getLabel().str() << "]";
}

/**
 * Generate code for a variable call that is not a member
 * @param variableChainAccess
 * @param ss
 */
void _varChainAccess_nonMember(std::shared_ptr<BVariableChainAccess> variableChainAccess, std::stringstream &ss) {
    ss << variableChainAccess->getVariable()->getLabel().str();
}

/**
 * Generate code for a chain call
 * @param chainCall
 * @return multi-line code representing a chain call
 */
void _chainToCode(std::shared_ptr<BScope> scope, std::shared_ptr<BChain> chain, size_t from, size_t to,
                  std::stringstream &ss) {

    // Have a unique key for each result of an element call
    std::map<std::shared_ptr<IBChainable>, std::string> returnMap;

    // Assign a unique key for each result
    static unsigned int uniqueId = 0;

    // Start processing the calls
    for (size_t i = from; i <= to; i++) {

        // Cast element
        auto variableChainAccess = std::dynamic_pointer_cast<BVariableChainAccess>((*chain)[i]);
        auto functionChainCall = std::dynamic_pointer_cast<BFunctionChainCall>((*chain)[i]);
        auto thisChainAccess = std::dynamic_pointer_cast<BThisChainAccess>((*chain)[i]);

        // Add indentation
        _indent(scope, ss);

        if (variableChainAccess) {

            // Generate new key for this variable
            std::string newKey = _generateResultKey(uniqueId++);
            returnMap[variableChainAccess] = newKey;
            ss << newKey << "=";

            if(i == 0) {
                if(variableChainAccess->getVariable()->isClassMember()) {
                    _varChainAccess_member_first(variableChainAccess, ss);
                } else {
                    _varChainAccess_nonMember(variableChainAccess, ss);
                }
            } else {
                _varChainAccess_member_middle(variableChainAccess, (*chain)[i-1]->getTypeScope()->getLabel().str(),
                                       returnMap[(*chain)[i-1]], ss);
            }
            ss << std::endl;

        } else if (functionChainCall) {

            ss << functionChainCall->getFunction()->getLabel().str();

            // TODO Put all arguments
            ss << " args...";

            if(i == 0) {
                if(functionChainCall->getFunction()->isClassMember()) {
                    ss << " " << FUNCTION_THIS;
                }
            } else {
                ss << " " << returnMap[(*chain)[i-1]];
            }

            if(functionChainCall->getFunction()->requiresReturn()) {
                std::string newKey = _generateResultKey(uniqueId++);
                returnMap[functionChainCall] = newKey;
                ss << " " << newKey;
            }
            ss << std::endl;
        } else if(thisChainAccess) {
            std::string newKey = _generateResultKey(uniqueId++);
            returnMap[thisChainAccess] = newKey;
            ss  << newKey << "=" << FUNCTION_THIS << std::endl;
        } else {
            throw BException("Cannot generate code for an unrecognized element call");
        }
    }
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
    ss << std::endl;
    ss << "# Create global variable" << std::endl;

    ss << variable->getLabel().str() << "=" << defaultValue << std::endl;
    BGenerateCode::get().write(ss);
}

void BBashHelper::createLocalVar(std::shared_ptr<BVariable> variable, std::string defaultValue) {
    std::stringstream ss;
    ss << std::endl;
    _indent(variable->getParentScope(),ss);
    ss << "# Create local variable" << std::endl;

    _indent(variable->getParentScope(), ss);
    ss << "local " << variable->getLabel().str() << "=" << defaultValue << std::endl;
    BGenerateCode::get().write(ss);
}

void BBashHelper::bash(std::shared_ptr<ecc::LexicalToken> token) {
    std::stringstream ss;
    ss << std::endl;
    ss << "# Run BASH code" << std::endl;

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
    ss << std::endl;
    ss << "# Create function" << std::endl;

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
    ss << "}" << std::endl;
    BGenerateCode::get().write(ss);
}

void BBashHelper::assignVariable(std::shared_ptr<BVariableAssign> variableAssign) {
    std::stringstream ss;

    // TODO Add expression
    auto variableChainAccess = variableAssign->getVariableAccess()->last();
    auto expression = variableAssign->getExpression();

    // Start converting the expression
    ss << std::endl;
    _indent(variableAssign->getParentScope(), ss);
    ss << "# Evaluating expression" << std::endl;

    // Convert chain to code
    ss << std::endl;
    _indent(variableAssign->getParentScope(), ss);
    ss << "# Assign variable" << std::endl;
    _chainToCode(variableAssign->getParentScope(), variableAssign->getVariableAccess()->getChain(), 0,
                     variableAssign->getVariableAccess()->getChain()->size()-2, ss);
    ss << std::endl;
    BGenerateCode::get().write(ss);
}

void BBashHelper::functionExec(std::shared_ptr<BFunctionCall> functionCall) {
    std::stringstream ss;

    // Convert chain to code
    ss << std::endl;
    _indent(functionCall->getParentScope(), ss);
    ss << "# Execute a function" << std::endl;
    _chainToCode(functionCall->last()->getFunction(), functionCall->getChain(), 0, functionCall->getChain()->size()-1, ss);
    ss << std::endl;

    BGenerateCode::get().write(ss);

}