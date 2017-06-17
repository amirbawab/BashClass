#include <bashclass/BBashHelper.h>
#include <bashclass/BGenerateCode.h>
#include <bashclass/BTypes.h>
#include <bashclass/BException.h>
#include <bashclass/BGlobal.h>
#include <bashclass/BVariableChainAccess.h>
#include <bashclass/BFunctionChainCall.h>
#include <iostream>
#include <bashclass/BArithOperation.h>
#include <bashclass/BVariableAccess.h>
#include <bashclass/BThisAccess.h>
#include <bashclass/BTokenUse.h>

// Constants
std::string FUNCTION_THIS = "_this_";
std::string FUNCTION_RETURN = "_return_";
std::string TMP_FUNCTION_RETURN = "_tmp_return_";
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
 * Generate a unique counter per class
 * @param classScope
 * @return class counter
 */
std::string _generateCounter(std::shared_ptr<BClass> classScope) {
    return classScope->getLabel().str() + "_uid_";
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
 * Generate a lock for an if statement
 * @param ifStatement
 * @return lock string
 */
std::string _generateIfLock(std::shared_ptr<BIf> ifStatement) {
    return ifStatement->getLabel().str() + "_lock";
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
            ss << "local " << newKey << "=${";

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

            // Prepare the return variable if required
            if(functionChainCall->getFunction()->requiresReturn()
               || functionChainCall->getFunction()->isConstructor()) {
                std::string newKey = _generateResultKey(uniqueId++);
                returnMap[functionChainCall] = newKey;
                _indent(scope, ss);
                ss << "local " << newKey << std::endl;
            }

            // Write the function call
            _indent(scope, ss);
            ss << functionChainCall->getFunction()->getLabel().str();

            // Write arguments
            for(size_t argIndex=0; argIndex < arguments.size(); argIndex++) {
                ss << " \"" << argumentsValues[argIndex] << "\"";
            }

            // Write the reference from previous or this when applicable
            if(!functionChainCall->getFunction()->isConstructor()) {
                if (i == 0) {
                    if (functionChainCall->getFunction()->isClassMember()) {
                        ss << " ${" << FUNCTION_THIS << "}";
                    }
                } else {
                    ss << " ${" << returnMap[(*chain)[i - 1]] << "}";
                }
            }

            // Write the return part
            if(functionChainCall->getFunction()->requiresReturn()
               || functionChainCall->getFunction()->isConstructor()) {
                ss << " " << TMP_FUNCTION_RETURN << std::endl;
                _indent(scope, ss);
                ss << returnMap[functionChainCall] << "=${" << TMP_FUNCTION_RETURN << "}";
            }

            ss << std::endl;

        } else if(thisChainAccess) {
            _indent(scope, ss);
            std::string newKey = _generateResultKey(uniqueId++);
            returnMap[thisChainAccess] = newKey;
            ss  << "local " << newKey << "=${" << FUNCTION_THIS << "}" << std::endl;
        } else {
            throw BException("Cannot generate code for an unrecognized element call");
        }
    }
}

std::string _arithOpForm1(std::string left, std::string op, std::string right) {
    return "$(( " + left + " " + op + " " + right + " ))";
}

std::string _arithOpForm2(std::string left, std::string op, std::string right) {
    return "$([[ \"" + left + "\" " + op + " \"" + right + "\" ]] && echo 1 || echo 0)";
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

        // If the token is null
        if(tokenUse->getLexicalToken()->getName() == BType::NULL_VALUE) {
            return "0";
        }

        // If the token is a true/false
        if(tokenUse->getLexicalToken()->getName() == BType::DATA_TYPE_NAME_BOOLEAN) {
            if(tokenUse->getLexicalToken()->getValue() == "true") {
                return "1";
            } else if(tokenUse->getLexicalToken()->getValue() == "false") {
                return "0";
            } else {
                throw BException("Unrecognized boolean token value found when trying to generate code");
            }
        }

        // If the token is a bash subshell
        if(tokenUse->getLexicalToken()->getName() == BType::DATA_TYPE_NAME_BASH_SUB) {
            return "$( " + tokenUse->getLexicalToken()->getValue().
                    substr(2, tokenUse->getLexicalToken()->getValue().length()-4) + " )";
        }

        // If the token is a string
        if(tokenUse->getLexicalToken()->getName() == BType::DATA_TYPE_NAME_STRING) {
            return tokenUse->getLexicalToken()->getValue()
                    .substr(1,tokenUse->getLexicalToken()->getValue().length()-2);
        }

        // Write the lexical token value
        return tokenUse->getLexicalToken()->getValue();
    }

    if(arithOperation) {
        // Get the type of the arithmetic operation
        std::string arithOperationType = arithOperation->getTypeValueAsString();
        std::string leftOperandType = arithOperation->getLeftOperand()->getTypeValueAsString();
        std::string rightOperandType = arithOperation->getRightOperand()->getTypeValueAsString();

        // If both operand are defined
        if(arithOperation->getLeftOperand() && arithOperation->getRightOperand()) {
            auto leftStr = _expressionToCode(scope, arithOperation->getLeftOperand(), ss);
            auto rightStr = _expressionToCode(scope, arithOperation->getRightOperand(), ss);

            _indent(scope, ss);
            std::string newKey = _generateExpressionKey(uniqueId++);

            if(arithOperationType == BType::TYPE_VALUE_STRING) {

                // String concatenation
                ss << "local " << newKey << "=\"" << leftStr << rightStr << "\"" << std::endl;

            } else if(arithOperationType == BType::TYPE_VALUE_BOOLEAN) {

                if(arithOperation->getOperator()->getName() == BArithOperation::BOOL_IS_NOT_EQUAL
                   || arithOperation->getOperator()->getName() == BArithOperation::BOOL_IS_EQUAL
                   || arithOperation->getOperator()->getName() == BArithOperation::BOOL_LESS_THAN
                   || arithOperation->getOperator()->getName() == BArithOperation::BOOL_GREATER_THAN) {

                    if(leftOperandType == BType::TYPE_VALUE_STRING || rightOperandType == BType::TYPE_VALUE_STRING) {
                        ss << "local " << newKey << "="
                           << _arithOpForm2(leftStr, arithOperation->getOperator()->getValue(), rightStr)
                           << std::endl;
                    } else {
                        ss << "local " << newKey << "="
                           << _arithOpForm1(leftStr, arithOperation->getOperator()->getValue(), rightStr)
                           << std::endl;
                    }
                } else {
                    ss << "local " << newKey << "=" << _arithOpForm1(leftStr, arithOperation->getOperator()->getValue(), rightStr)
                       << std::endl;
                }

            } else if(arithOperationType == BType::TYPE_VALUE_INT) {

                ss << "local " << newKey << "=" << _arithOpForm1(leftStr, arithOperation->getOperator()->getValue(), rightStr)
                   << std::endl;
            } else {
                throw BException("Cannot generate code for an unknown arithmetic operation type with 2 operand");
            }

            return "${" + newKey + "}";
        }

        if(arithOperation->getRightOperand()) {

            auto rightStr = _expressionToCode(scope, arithOperation->getRightOperand(), ss);
            _indent(scope, ss);
            std::string newKey = _generateExpressionKey(uniqueId++);

            if(arithOperationType == BType::TYPE_VALUE_BOOLEAN) {
                ss << "local " << newKey << "=" << _arithOpForm1(rightStr, "^", "1") << std::endl;
            } else if (arithOperationType == BType::TYPE_VALUE_INT) {
                ss << "local " << newKey << "="
                   << _arithOpForm1(rightStr, "*", "(" + arithOperation->getOperator()->getValue() + "1)")
                   << std::endl;
            }else {
                throw BException("Cannot generate code for an unknown arithmetic operation type with right operand");
            }
            return "${" + newKey + "}";
        }
        throw BException("Cannot generate code for an undefined arithmetic operation composition");
    }
    throw BException("Cannot generate code for an undefined expression type");
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
    ss << mainFunction->getLabel().str();

    // Generate arguments
    for(size_t argIndex=0; argIndex < mainFunction->findAllParameters().size(); argIndex++) {
        ss << " " << "\"${" << argIndex+1 << "}\"";
    }

    // Generate exit
    const char* MAIN_FUNCTION_RETURN = "_main_return_";
    ss << " " << MAIN_FUNCTION_RETURN << std::endl;
    ss << "exit ${" << MAIN_FUNCTION_RETURN << "}";

    ss << std::endl << std::endl;

    // Write footer
    BGenerateCode::get().write(ss);
    BGenerateCode::get().writePostCode();
}

void BBashHelper::declareClass(std::shared_ptr<BClass> classScope) {
    std::stringstream ss;
    ss << "declare -A " << classScope->getLabel().str() << "=()" << std::endl;
    ss << _generateCounter(classScope) << "=1" << std::endl;
    BGenerateCode::get().write(ss);
}

void BBashHelper::createGlobalVar(std::shared_ptr<BVariable> variable) {
    std::stringstream ss;
    ss << std::endl;
    ss << "# Create global variable" << std::endl;

    ss << variable->getLabel().str() << "=" << variable->getDefaultValue() << std::endl;
    BGenerateCode::get().write(ss);
}

void BBashHelper::createLocalVar(std::shared_ptr<BVariable> variable) {
    std::stringstream ss;
    ss << std::endl;
    _indent(variable->getParentScope(),ss);
    ss << "# Create local variable" << std::endl;

    _indent(variable->getParentScope(), ss);
    ss << "local " << variable->getLabel().str() << "=" << variable->getDefaultValue() << std::endl;
    BGenerateCode::get().write(ss);
}

void BBashHelper::bash(std::shared_ptr<BScope> scope, std::shared_ptr<ecc::LexicalToken> token) {
    std::stringstream ss;
    ss << std::endl;
    _indent(scope, ss);
    ss << "# Run BASH code" << std::endl;

    _indent(scope, ss);
    if(token->getName() == BType::DATA_TYPE_NAME_BASH_INLINE) {
        ss << token->getValue().substr(2,token->getValue().length()-2);
    } else if(token->getName() == BType::DATA_TYPE_NAME_BASH_BLOCK) {
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
        ss << "local " << param->getLabel().str() << "=\"${" << paramPos++ << "}\"" << std::endl;
    }

    // Generate 'this' if function is a class member
    if(function->isClassMember()) {
        if(function->isConstructor()) {

            // Assign `this` and increment counter
            auto classScope = std::static_pointer_cast<BClass>(function->getParentScope());
            _indent(function, ss);
            ss << "local " << FUNCTION_THIS << "=${" << _generateCounter(classScope) << "}" << std::endl;
            _indent(function, ss);
            ss << _generateCounter(classScope) << "=" << _arithOpForm1("${"+_generateCounter(classScope)+"}", "+", "1")
               << std::endl;

            // Initialize members
            for(auto variable : classScope->findAllVariables()) {
                _indent(function, ss);
                ss << classScope->getLabel().str() << "[${" << FUNCTION_THIS << "},\""
                   << variable->getLabel().str() << "\"]=" << variable->getDefaultValue() << std::endl;
            }

        } else {
            _indent(function, ss);
            ss << "local " << FUNCTION_THIS << "=${" << paramPos++ << "}" << std::endl;
        }
    }

    // Generate return reference if function requires a return statement
    if(function->requiresReturn() || function->isConstructor()) {
        _indent(function, ss);
        ss << "declare -n " << FUNCTION_RETURN << "=${" << paramPos++ << "}" << std::endl;
    }

    BGenerateCode::get().write(ss);
}

void BBashHelper::closeFunction(std::shared_ptr<BFunction> function) {
    std::stringstream ss;

    if(function->isConstructor()) {
        _indent(function, ss);
        ss << FUNCTION_RETURN << "=${" << FUNCTION_THIS << "}" << std::endl;
    }

    _indent(function->getParentScope(), ss);
    ss << "}" << std::endl;
    BGenerateCode::get().write(ss);
}

void BBashHelper::writeExpression(std::shared_ptr<BScope> scope, std::shared_ptr<IBExpression> expression) {
    std::stringstream ss;
    ss << std::endl;
    _indent(scope, ss);
    ss << "# Evaluate expression" << std::endl;
    _expressionToCode(scope, expression, ss);
    BGenerateCode::get().write(ss);
}

void BBashHelper::writeReturn(std::shared_ptr<BReturn> rtn) {
    std::stringstream ss;

    // Add bash comment
    ss << std::endl;
    _indent(rtn->getParentScope(), ss);
    ss << "# Return statement" << std::endl;

    // Check if function has an expression, or is void
    if(rtn->getExpression()) {
        // Start processing the expression
        std::string expression = _expressionToCode(rtn->getParentScope(), rtn->getExpression(), ss);

        // Write the return statement
        _indent(rtn->getParentScope(), ss);
        ss << FUNCTION_RETURN << "=" << expression << std::endl;
    }

    // Return from bash function
    _indent(rtn->getParentScope(), ss);
    ss << "return" << std::endl;

    BGenerateCode::get().write(ss);
}

void BBashHelper::createIf(std::shared_ptr<BIf> ifStatement) {
    std::stringstream ss;

    // Add bash comment
    ss << std::endl;
    _indent(ifStatement->getParentScope(), ss);
    ss << "# If statement" << std::endl;

    // Start processing the expression
    std::string expression = _expressionToCode(ifStatement->getParentScope(), ifStatement->getExpression(), ss);

    // Reset if statement lock
    _indent(ifStatement->getParentScope(), ss);
    ss << "local " << _generateIfLock(ifStatement) << "=0" << std::endl;

    // Write if statement
    _indent(ifStatement->getParentScope(), ss);
    ss << "if (( " << expression << " )); then" << std::endl;

    // Acquire the lock
    _indent(ifStatement->getParentScope(), ss);
    ss << _generateIfLock(ifStatement) << "=1" << std::endl;

    BGenerateCode::get().write(ss);
}

void BBashHelper::createElif(std::shared_ptr<BElif> elifStatement) {
    std::stringstream ss;

    // Add bash comment
    ss << std::endl;
    _indent(elifStatement->getParentScope(), ss);
    ss << "# Elif statement" << std::endl;

    // Check if lock acquired already
    _indent(elifStatement->getParentScope(), ss);
    ss << "if ! (( ${" << _generateIfLock(elifStatement->getParentIf()) << "} )); then" << std::endl;

    // Start processing the expression
    std::string expression = _expressionToCode(elifStatement->getParentScope(), elifStatement->getExpression(), ss);

    // Write elif statement
    _indent(elifStatement->getParentScope(), ss);
    ss << "if (( " << expression << " )); then" << std::endl;

    // Acquire lock
    _indent(elifStatement->getParentScope(), ss);
    ss << _generateIfLock(elifStatement->getParentIf()) << "=1" << std::endl;

    BGenerateCode::get().write(ss);
}

void BBashHelper::createElse(std::shared_ptr<BElse> elseStatement) {
    std::stringstream ss;

    // Add bash comment
    ss << std::endl;
    _indent(elseStatement->getParentScope(), ss);
    ss << "# Else statement" << std::endl;

    // Check if lock acquired already
    _indent(elseStatement->getParentScope(), ss);
    ss << "if ! (( ${" << _generateIfLock(elseStatement->getParentIf()) << "} )); then" << std::endl;

    BGenerateCode::get().write(ss);
}

void BBashHelper::closeIf(std::shared_ptr<BIf> ifStatement) {
    std::stringstream ss;
    _indent(ifStatement->getParentScope(), ss);
    ss << "fi" << std::endl;
    BGenerateCode::get().write(ss);
}

void BBashHelper::closeElif(std::shared_ptr<BElif> elifStatement) {
    std::stringstream ss;
    _indent(elifStatement->getParentScope(), ss);
    ss << "fi" << std::endl;
    _indent(elifStatement->getParentScope(), ss);
    ss << "fi" << std::endl;
    BGenerateCode::get().write(ss);
}

void BBashHelper::closeElse(std::shared_ptr<BElse> elseStatement) {
    std::stringstream ss;
    _indent(elseStatement->getParentScope(), ss);
    ss << "fi" << std::endl;
    BGenerateCode::get().write(ss);
}

void BBashHelper::createWhile(std::shared_ptr<BWhile> whileStatement) {
    std::stringstream ss;

    // Add bash comment
    ss << std::endl;
    _indent(whileStatement->getParentScope(), ss);
    ss << "# While statement" << std::endl;

    // Write while statement
    _indent(whileStatement->getParentScope(), ss);
    ss << "while true; do" << std::endl;

    // Start processing the expression
    std::string expression = _expressionToCode(whileStatement->getParentScope(), whileStatement->getExpression(), ss);

    // Create if statement
    _indent(whileStatement->getParentScope(), ss);
    ss << "! (( " << expression << " )) && break" << std::endl;

    BGenerateCode::get().write(ss);
}

void BBashHelper::closeWhile(std::shared_ptr<BWhile> whileStatement) {
    std::stringstream ss;
    _indent(whileStatement->getParentScope(), ss);
    ss << "done" << std::endl;
    BGenerateCode::get().write(ss);
}