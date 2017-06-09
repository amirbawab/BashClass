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
#include <bashclass/BVariableAccess.h>
#include <bashclass/BThisAccess.h>
#include <bashclass/BTokenUse.h>

// Constants
std::string FUNCTION_THIS = "_this_";
std::string FUNCTION_RETURN = "_return_";
std::string RESULT = "_result_";
std::string EXPRESSION = "_expression_";

// Delcare function (when necessary)
std::string _expressionToCode(std::shared_ptr<BScope> scope, std::shared_ptr<IBExpression> expression,
                              std::stringstream &ss);
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
 * Generate a unique expression key/variable per expression
 * @param number
 * @return unqieu expression string
 */
std::string _generateExpressionKey(unsigned int number) {
    return EXPRESSION + std::to_string(number);
}

/**
 * Generate code for a variable chain access that is a member and is placed first in the chain
 * @param variableChainAccess
 * @param ss
 */
void _varChainAccess_member_first(std::shared_ptr<BVariableChainAccess> variableChainAccess, std::stringstream &ss) {
    ss << variableChainAccess->getVariable()->getParentScope()->findClosestClass()->getLabel().str()
       << "[${" << FUNCTION_THIS << "},\"" << variableChainAccess->getVariable()->getLabel().str() << "\"]";
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
    ss << prevTypeLabel << "[${" << prevResult << "},\"" << variableChainAccess->getVariable()->getLabel().str() << "\"]" ;
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
       << "[${" << prevResult << "},\"" << variableChainAccess->getVariable()->getLabel().str() << "\"]";
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
                  std::map<std::shared_ptr<IBChainable>, std::string> &returnMap, std::stringstream &ss) {

    // Assign a unique key for each result
    static unsigned int uniqueId = 0;

    // Start processing the calls
    for (size_t i = from; i <= to; i++) {

        // Cast element
        auto variableChainAccess = std::dynamic_pointer_cast<BVariableChainAccess>((*chain)[i]);
        auto functionChainCall = std::dynamic_pointer_cast<BFunctionChainCall>((*chain)[i]);
        auto thisChainAccess = std::dynamic_pointer_cast<BThisChainAccess>((*chain)[i]);

        if (variableChainAccess) {

            // Add indentation
            _indent(scope, ss);

            // Generate new key for this variable
            std::string newKey = _generateResultKey(uniqueId++);
            returnMap[variableChainAccess] = newKey;
            ss << newKey << "=${";

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
            ss << "}" << std::endl;

        } else if (functionChainCall) {

            // Start processing the arguments
            std::vector<std::string> argumentsValues;
            auto arguments = functionChainCall->getArguments();
            for(size_t argIndex=0; argIndex < arguments.size(); argIndex++){
                argumentsValues.push_back(_expressionToCode(scope, arguments[argIndex], ss));
            }

            // Write the function call
            _indent(scope, ss);
            ss << functionChainCall->getFunction()->getLabel().str();

            // Write arguments
            for(size_t argIndex=0; argIndex < arguments.size(); argIndex++) {
                ss << " " << argumentsValues[argIndex];
            }

            // Write the reference from previous or this when applicable
            if(i == 0) {
                if(functionChainCall->getFunction()->isClassMember()) {
                    ss << " " << FUNCTION_THIS;
                }
            } else {
                ss << " " << returnMap[(*chain)[i-1]];
            }

            // Write the return variable if required
            if(functionChainCall->getFunction()->requiresReturn()) {
                std::string newKey = _generateResultKey(uniqueId++);
                returnMap[functionChainCall] = newKey;
                ss << " " << newKey;
            }
            ss << std::endl;
        } else if(thisChainAccess) {
            _indent(scope, ss);
            std::string newKey = _generateResultKey(uniqueId++);
            returnMap[thisChainAccess] = newKey;
            ss  << newKey << "=${" << FUNCTION_THIS << "}" << std::endl;
        } else {
            throw BException("Cannot generate code for an unrecognized element call");
        }
    }
}

std::string _expressionToCode(std::shared_ptr<BScope> scope, std::shared_ptr<IBExpression> expression,
                              std::stringstream &ss) {
    auto thisAccess = std::dynamic_pointer_cast<BThisAccess>(expression);
    auto variableAccess = std::dynamic_pointer_cast<BVariableAccess>(expression);
    auto functionCall = std::dynamic_pointer_cast<BFunctionCall>(expression);
    auto tokenUse = std::dynamic_pointer_cast<BTokenUse>(expression);
    auto arithOperation = std::dynamic_pointer_cast<BArithOperation>(expression);

    // Assign a unique key for each expression
    static unsigned int uniqueId = 0;

    if(thisAccess) {
        return "${" + FUNCTION_THIS + "}";
    }

    if(variableAccess) {

        // Process chain
        std::map<std::shared_ptr<IBChainable>, std::string> returnMap;
        _chainToCode(scope, variableAccess->getChain(), 0, variableAccess->getChain()->size()-1, returnMap, ss);

        // Get the variable key of the last element
        return "${" + returnMap[variableAccess->last()] + "}";
    }

    if(functionCall) {

        // Process chain
        std::map<std::shared_ptr<IBChainable>, std::string> returnMap;
        _chainToCode(scope, functionCall->getChain(), 0, functionCall->getChain()->size()-1, returnMap, ss);

        // Get the variable key of the last element
        return "${" + returnMap[functionCall->last()] + "}";
    }

    if(tokenUse) {
        return tokenUse->getLexicalToken()->getValue();
    }

    auto leftStr = _expressionToCode(scope, arithOperation->getLeftOperand(), ss);
    auto rightStr = _expressionToCode(scope, arithOperation->getRightOperand(), ss);
    _indent(scope, ss);
    std::string newKey = _generateExpressionKey(uniqueId++);
    ss << newKey << "=$((" << leftStr + " " + arithOperation->getOperator()->getValue() + " " + rightStr << "))"
       << std::endl;
    return "${" + newKey + "}";
}

void BBashHelper::header() {
    BGenerateCode::get().writePreCode();
}

void BBashHelper::footer() {

    std::stringstream ss;

    // Add bash comment
    ss << std::endl << "# Run main function" << std::endl;

    // Find main function
    auto functions = BGlobal::getInstance()->findAllFunctions(BGlobal::MAIN_FUNCTION.c_str());

    if(functions.size() != 1) {
        throw BException("Cannot determine which main function to run");
    }

    // Get main function
    std::shared_ptr<BFunction> mainFunction = functions.front();

    // Generate code for the main
    ss << BGlobal::MAIN_FUNCTION;

    // Generate arguments
    for(size_t argIndex=0; argIndex < mainFunction->findAllParameters().size(); argIndex++) {
        ss << " " << "\"$" << argIndex+1 << "\"";
    }

    ss << std::endl << std::endl;

    // Write footer
    BGenerateCode::get().write(ss);
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

    ss << variable->getLabel().str() << "=\"TODO\"" << defaultValue << std::endl;
    BGenerateCode::get().write(ss);
}

void BBashHelper::createLocalVar(std::shared_ptr<BVariable> variable, std::string defaultValue) {
    std::stringstream ss;
    ss << std::endl;
    _indent(variable->getParentScope(),ss);
    ss << "# Create local variable" << std::endl;

    _indent(variable->getParentScope(), ss);
    ss << "local " << variable->getLabel().str() << "=\"TODO\"" << defaultValue << std::endl;
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

    std::shared_ptr<BVariableAccess> variableAccess = variableAssign->getVariableAccess();
    std::shared_ptr<BChain> variableAccessChain = variableAccess->getChain();

    // Store return values in unique keys in a map
    std::map<std::shared_ptr<IBChainable>, std::string> returnMap;

    // Add bash comments
    ss << std::endl;
    _indent(variableAssign->getParentScope(), ss);
    ss << "# Assign variable" << std::endl;

    // Start processing the expression
    std::string expression = _expressionToCode(variableAssign->getParentScope(), variableAssign->getExpression(), ss);

    // Convert chain to code
    if(variableAccessChain->size() >= 2) {
        _chainToCode(variableAssign->getParentScope(), variableAccessChain, 0, variableAccessChain->size()-2,
                     returnMap, ss);
    }

    // Add the last assignment statement
    _indent(variableAssign->getParentScope(), ss);
    if(variableAccess->last()->getVariable()->isClassMember()) {
        if(variableAccessChain->size() == 1) {
            _varChainAccess_member_first(variableAccess->last(), ss);
        } else {
            _varChainAccess_member_last(variableAccess->last(),
                                        returnMap[(*variableAccessChain)[variableAccessChain->size()-2]], ss);
        }
    } else {
        _varChainAccess_nonMember(variableAccess->last(), ss);
    }
    ss << "=" << expression << std::endl;
    BGenerateCode::get().write(ss);
}

void BBashHelper::functionExec(std::shared_ptr<BFunctionCall> functionCall) {
    std::stringstream ss;

    // Store return values in unique keys in a map
    std::map<std::shared_ptr<IBChainable>, std::string> returnMap;

    // Convert chain to code
    ss << std::endl;
    _indent(functionCall->getParentScope(), ss);
    ss << "# Execute a function" << std::endl;
    _chainToCode(functionCall->last()->getFunction(), functionCall->getChain(), 0, functionCall->getChain()->size()-1,
                 returnMap, ss);
    ss << std::endl;
    BGenerateCode::get().write(ss);
}

void BBashHelper::writeReturn(std::shared_ptr<BReturn> rtn) {
    std::stringstream ss;

    // Add bash comment
    ss << std::endl;
    _indent(rtn->getParentScope(), ss);
    ss << "# Return statement" << std::endl;

    // Start processing the expression
    std::string expression = _expressionToCode(rtn->getParentScope(), rtn->getExpression(), ss);

    // Write the return statement
    _indent(rtn->getParentScope(), ss);
    ss << FUNCTION_RETURN << "=" << expression;

    ss << std::endl;
    BGenerateCode::get().write(ss);
}