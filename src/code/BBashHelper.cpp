#include <bashclass/BBashHelper.h>
#include <bashclass/BGenerateCode.h>
#include <bashclass/BException.h>
#include <bashclass/BGlobal.h>
#include <bashclass/BVariableChainAccess.h>
#include <iostream>
#include <bashclass/BArithOperation.h>
#include <bashclass/BVariableAccess.h>
#include <bashclass/BThisAccess.h>
#include <bashclass/BTokenUse.h>
#include <bashclass/BArrayUse.h>

// Constants
std::string FUNCTION_THIS = "_this_";
std::string FUNCTION_RETURN = "_return_";
std::string TMP_FUNCTION_RETURN = "_tmp_return_";
std::string RESULT = "_result_";
std::string EXPRESSION = "_expression_";
std::string PROG_ARRAY = "_array_";
std::string PROG_ARRAY_COUNTER = "_array_uid_";

/**
 * Structure for the return of an expression
 */
struct ExprReturn {
    const static short DATA = 0;
    const static short VARIABLE = 1;
    ExprReturn(std::string value, short type): value(value), type(type){}
    std::string value;
    int type;
    std::string formattedValue() { return type == VARIABLE ? "${" + value + "}" : value;}
};

// Declare function (when necessary)
ExprReturn _expressionToCode(std::shared_ptr<BScope> scope, std::shared_ptr<IBExpression> expression,
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
 * @return unique expression string
 */
std::string _generateExpressionKey(unsigned int number) {
    return EXPRESSION + std::to_string(number);
}

/**
 * Generate a unique array key/variable per statement
 * @param number
 * @return unique key string
 */
std::string _generateArrayKey(unsigned int number) {
    return PROG_ARRAY + std::to_string(number);
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
 * Generate code for a variable call that is not a member
 * @param variableChainAccess
 * @param ss
 */
void _varChainAccess_nonMember(std::shared_ptr<BVariableChainAccess> variableChainAccess, std::stringstream &ss) {
    ss << variableChainAccess->getVariable()->getLabel().str();
}

/**
 * Generate code for an array access
 * @param scope
 * @param variable
 * @param variableChainAccess
 * @param ss
 * @return new key
 */
std::string _arrayToCode(std::shared_ptr<BScope> scope, std::string variable,
                         std::shared_ptr<IBChainable> chainAccess,
                         std::stringstream &ss) {
    static unsigned int uniqueId = 0;
    auto indices = chainAccess->getIndices();
    std::string tmpVariable = variable;
    for(size_t i=0; i < indices.size(); i++) {
        auto expression = _expressionToCode(scope, indices[i], ss);
        _indent(scope, ss);
        std::string newKey = _generateArrayKey(uniqueId++);
        ss << "declare -n " << newKey << "="
           << PROG_ARRAY << "[${" << tmpVariable << "}," << expression.formattedValue() << "]" << std::endl;
        tmpVariable = newKey;
    }
    return tmpVariable;
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
            ss << "declare -n " << newKey << "=";

            if(i == 0) {
                if(variableChainAccess->getVariable()->isClassMember()) {
                    _varChainAccess_member_first(variableChainAccess, ss);
                } else {
                    _varChainAccess_nonMember(variableChainAccess, ss);
                }
            } else {
                _varChainAccess_member_middle(variableChainAccess,
                                              (*chain)[i-1]->getType()->getTypeScope()->getLabel().str(),
                                              returnMap[(*chain)[i-1]], ss);
            }
            ss << std::endl;

        } else if (functionChainCall) {

            // Start processing the arguments
            std::vector<std::string> argumentsValues;
            auto arguments = functionChainCall->getArguments();
            auto params = functionChainCall->getFunction()->findAllParameters();
            for(size_t argIndex=0; argIndex < arguments.size(); argIndex++){
                auto expression = _expressionToCode(scope, arguments[argIndex], ss);
                argumentsValues.push_back(expression.formattedValue());
            }

            // Prepare the return variable if required
            if(functionChainCall->getFunction()->requiresReturn()
               || functionChainCall->getFunction()->isConstructor()) {
                std::string newKey = _generateResultKey(uniqueId++);
                returnMap[functionChainCall] = newKey;
                _indent(scope, ss);
                ss << "declare " << newKey << std::endl;
            }

            // Write the function call
            _indent(scope, ss);
            ss << functionChainCall->getFunction()->getLabel().str();

            // Write arguments
            for(size_t argIndex=0; argIndex < argumentsValues.size(); argIndex++) {
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
            ss  << "declare " << newKey << "=${" << FUNCTION_THIS << "}" << std::endl;
        } else {
            throw BException("Cannot generate code for an unrecognized element call");
        }
    }
}

/**
 * Generate code for an arithmetic operation (1)
 * @param left
 * @param op
 * @param right
 * @return arithmetic operation code
 */
std::string _arithOpForm1(std::string left, std::string op, std::string right) {
    return "$(( " + left + " " + op + " " + right + " ))";
}

/**
 * Generate code for an arithmetic operation (2)
 * @param left
 * @param op
 * @param right
 * @return arithmetic operation code
 */
std::string _arithOpForm2(std::string left, std::string op, std::string right) {
    return "$([[ \"" + left + "\" " + op + " \"" + right + "\" ]] && echo 1 || echo 0)";
}

/**
 * Convert expression to code
 * @param scope
 * @param expression
 * @param ss
 * @return ExprReturn object
 */
ExprReturn _expressionToCode(std::shared_ptr<BScope> scope, std::shared_ptr<IBExpression> expression,
                              std::stringstream &ss) {

    // Cast expression
    auto thisAccess = std::dynamic_pointer_cast<BThisAccess>(expression);
    auto variableAccess = std::dynamic_pointer_cast<BVariableAccess>(expression);
    auto functionCall = std::dynamic_pointer_cast<BFunctionCall>(expression);
    auto tokenUse = std::dynamic_pointer_cast<BTokenUse>(expression);
    auto arrayUse = std::dynamic_pointer_cast<BArrayUse>(expression);
    auto arithOperation = std::dynamic_pointer_cast<BArithOperation>(expression);

    // Assign a unique key for each expression
    static unsigned int uniqueId = 0;

    /********************
     *       THIS
     ********************/
    if(thisAccess) {
        return ExprReturn(FUNCTION_THIS, ExprReturn::VARIABLE);
    }

    /********************
     *   VARIABLE ACCESS
     ********************/
    if(variableAccess) {

        // Process chain
        std::map<std::shared_ptr<IBChainable>, std::string> returnMap;
        _chainToCode(scope, variableAccess->getChain(), 0, variableAccess->getChain()->size()-1, returnMap, ss);

        // Process array access (if any)
        std::string newKey = _arrayToCode(scope, returnMap[variableAccess->last()], variableAccess->last(), ss);

        // Get the variable key of the last element
        return ExprReturn(newKey, ExprReturn::VARIABLE);
    }

    /********************
     *   FUNCTION CALL
     ********************/
    if(functionCall) {

        // Process chain
        std::map<std::shared_ptr<IBChainable>, std::string> returnMap;
        _chainToCode(scope, functionCall->getChain(), 0, functionCall->getChain()->size()-1, returnMap, ss);

        // Process array access (if any)
        std::string newKey = _arrayToCode(scope, returnMap[functionCall->last()], functionCall->last(), ss);

        // Get the variable key of the last element
        return ExprReturn(newKey, ExprReturn::VARIABLE);
    }

    /********************
     *    TOKEN USE
     ********************/
    if(tokenUse) {

        // If the token is null
        if(tokenUse->getType()->isNull()) {
            // TODO Store in const
            return ExprReturn("0", ExprReturn::DATA);
        }

        // If the token is a true/false
        if(tokenUse->getLexicalToken()->getName() == BElementType::DATA_TYPE_NAME_BOOLEAN) {
            if(tokenUse->getLexicalToken()->getValue() == "true") {
                return ExprReturn("1", ExprReturn::DATA);
            }

            if(tokenUse->getLexicalToken()->getValue() == "false") {
                return ExprReturn("0", ExprReturn::DATA);
            }
            throw BException("Unrecognized boolean token value found when trying to generate code");
        }

        // If the token is a bash subshell
        if(tokenUse->getLexicalToken()->getName() == BElementType::DATA_TYPE_NAME_BASH_SUB) {
            return ExprReturn("$( " + tokenUse->getLexicalToken()->getValue().
                    substr(2, tokenUse->getLexicalToken()->getValue().length()-4) + " )", ExprReturn::DATA);
        }

        // If the token is a string
        if(tokenUse->getLexicalToken()->getName() == BElementType::DATA_TYPE_NAME_STRING) {
            return ExprReturn(tokenUse->getLexicalToken()->getValue()
                    .substr(1,tokenUse->getLexicalToken()->getValue().length()-2), ExprReturn::DATA);
        }

        // Write the lexical token value
        return ExprReturn(tokenUse->getLexicalToken()->getValue(), ExprReturn::DATA);
    }

    /********************
     *    ARRAY USE
     ********************/
    if(arrayUse) {
        _indent(scope, ss);
        std::string newKey = _generateExpressionKey(uniqueId++);
        ss << "declare " << newKey << "=${" << PROG_ARRAY_COUNTER << "}" << std::endl;
        _indent(scope, ss);
        ss << PROG_ARRAY_COUNTER << "=" << _arithOpForm1("${" + PROG_ARRAY_COUNTER + "}", "+", "1")
           << std::endl;
        return ExprReturn(newKey, ExprReturn::VARIABLE);
    }

    /********************
     *  ARITH OPERATION
     ********************/
    if(arithOperation) {

        // Get the type of the arithmetic operation
        std::shared_ptr<IBType> arithOperationType = arithOperation->getType();
        std::shared_ptr<IBType> leftOperandType = arithOperation->getLeftOperand()->getType();
        std::shared_ptr<IBType> rightOperandType = arithOperation->getRightOperand()->getType();
        std::string arithOperator = arithOperation->getOperator()->getName();

        // If both operand are defined
        if(arithOperation->getLeftOperand() && arithOperation->getRightOperand()) {

            // Process left and right operands
            ExprReturn leftOp = _expressionToCode(scope, arithOperation->getLeftOperand(), ss);
            ExprReturn rightOp = _expressionToCode(scope, arithOperation->getRightOperand(), ss);

            // Generate a new key to store the new expression result
            _indent(scope, ss);
            std::string newKey = _generateExpressionKey(uniqueId++);

            // +
            if(arithOperator == BArithOperation::OP_PLUS) {

                // String concatenation
                if(arithOperationType->isString()) {
                    ss << "declare " << newKey << "=\"" << leftOp.formattedValue() << rightOp.formattedValue()
                       << "\"" << std::endl;
                    return ExprReturn(newKey, ExprReturn::VARIABLE);
                }

                // Integer addition
                if(arithOperationType->isInt()) {
                    ss << "declare " << newKey << "="
                       << _arithOpForm1(leftOp.formattedValue(), arithOperation->getOperator()->getValue(),
                                        rightOp.formattedValue())
                       << std::endl;
                    return ExprReturn(newKey, ExprReturn::VARIABLE);
                }

                throw BException("Cannot generator code for the + operator with an unexpected return type");
            }

            // |, ^, &, <<, >>, -, *, /, %, **
            if(arithOperator == BArithOperation::OP_BIT_OR || arithOperator == BArithOperation::OP_BIT_XOR
               || arithOperator == BArithOperation::OP_BIT_AND || arithOperator == BArithOperation::OP_LEFT_SHIFT
               || arithOperator == BArithOperation::OP_RIGHT_SHIFT || arithOperator == BArithOperation::OP_MINUS
               || arithOperator == BArithOperation::OP_MULTIPLY || arithOperator == BArithOperation::OP_DIVIDE
               || arithOperator == BArithOperation::OP_MOD || arithOperator == BArithOperation::OP_EXPONENTIAL) {

                // Integer
                if(arithOperationType->isInt()) {
                    ss << "declare " << newKey << "="
                       << _arithOpForm1(leftOp.formattedValue(), arithOperation->getOperator()->getValue(),
                                        rightOp.formattedValue())
                       << std::endl;
                    return ExprReturn(newKey, ExprReturn::VARIABLE);
                }

                throw BException("Cannot generator code for the " + arithOperator +
                                         " operator with an unexpected return type");
            }

            // !=, ==, <, >
            if(arithOperator == BArithOperation::OP_IS_NOT_EQUAL || arithOperator == BArithOperation::OP_IS_EQUAL
               || arithOperator == BArithOperation::OP_LESS_THAN || arithOperator == BArithOperation::OP_GREATER_THAN) {

                // Boolean comparison
                if(arithOperationType->isBoolean()) {

                    // String comparison
                    if(leftOperandType->isString() || rightOperandType->isString()) {
                        ss << "declare " << newKey << "="
                           << _arithOpForm2(leftOp.formattedValue(), arithOperation->getOperator()->getValue(),
                                            rightOp.formattedValue())
                           << std::endl;
                        return ExprReturn(newKey, ExprReturn::VARIABLE);
                    }

                    // All other types comparison
                    ss << "declare " << newKey << "="
                       << _arithOpForm1(leftOp.formattedValue(), arithOperation->getOperator()->getValue(),
                                        rightOp.formattedValue())
                       << std::endl;
                    return ExprReturn(newKey, ExprReturn::VARIABLE);
                }
                throw BException("Cannot generator code for the " + arithOperator +
                                         " operator with an unexpected return type");
            }

            // ||, &&, <=, >=
            if(arithOperator == BArithOperation::OP_LOGICAL_OR || arithOperator == BArithOperation::OP_LOGICAL_AND
               || arithOperator == BArithOperation::OP_LESS_OR_EQUAL || arithOperator == BArithOperation::OP_GREATER_OR_EQUAL) {

                // Boolean comparison
                if(arithOperationType->isBoolean()) {
                    ss << "declare " << newKey << "="
                       << _arithOpForm1(leftOp.formattedValue(), arithOperation->getOperator()->getValue(),
                                        rightOp.formattedValue())
                       << std::endl;
                    return ExprReturn(newKey, ExprReturn::VARIABLE);
                }
            }

            // =
            if(arithOperator == BArithOperation::OP_ASSIGN) {

               // All types have the same code generated
                ss << "declare " << newKey << "="
                   << _arithOpForm1(leftOp.value, arithOperation->getOperator()->getValue(), rightOp.formattedValue())
                   << std::endl;
                return ExprReturn(newKey, ExprReturn::VARIABLE);
            }

            throw BException("Cannot generator code for an arithmetic operation with two operands and unknown operator");
        }

        // If only the right operand is defined
        if(arithOperation->getRightOperand()) {

            // Process right operand
            auto rightStr = _expressionToCode(scope, arithOperation->getRightOperand(), ss);

            // Generate new key to store the new expression
            _indent(scope, ss);
            std::string newKey = _generateExpressionKey(uniqueId++);

            // !
            if(arithOperator == BArithOperation::OP_NOT) {

                // Boolean toggle
                if(arithOperationType->isBoolean()) {
                    ss << "declare " << newKey << "=" << _arithOpForm1(rightStr.formattedValue(), "^", "1") << std::endl;
                    return ExprReturn(newKey, ExprReturn::VARIABLE);
                }
                throw BException("Cannot generator code for the ! operator with an unexpected return type");
            }

            // +, -
            if(arithOperator == BArithOperation::OP_PLUS || arithOperator == BArithOperation::OP_MINUS) {

                // Integer sign
                if(arithOperationType->isInt()) {
                    ss << "declare " << newKey << "="
                       << _arithOpForm1(rightStr.formattedValue(), "*", "(" + arithOperation->getOperator()->getValue() + "1)")
                       << std::endl;
                    return ExprReturn(newKey, ExprReturn::VARIABLE);
                }
                throw BException("Cannot generator code for the " + arithOperator +
                                         " operator with an unexpected return type");
            }
            throw BException("Cannot generator code for an arithmetic operation with one operand and unknown operator");
        }
        throw BException("Cannot generate code for an undefined arithmetic operation composition");
    }
    throw BException("Cannot generate code for an undefined expression type");
}

/**
 * Generate bash header
 */
void BBashHelper::header() {
    BGenerateCode::get().writePreCode();
}

/**
 * Generate bash footer
 */
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

/**
 * Create class
 * @param classScope
 */
void BBashHelper::declareClass(std::shared_ptr<BClass> classScope) {
    std::stringstream ss;
    ss << "declare -A " << classScope->getLabel().str() << "=()" << std::endl;
    ss << _generateCounter(classScope) << "=1" << std::endl;
    BGenerateCode::get().write(ss);
}

/**
 * Create single array
 */
void BBashHelper::declareArray() {
    std::stringstream ss;
    ss << "declare -A " << PROG_ARRAY << "=()" << std::endl;
    ss << PROG_ARRAY_COUNTER << "=1" << std::endl;
    BGenerateCode::get().write(ss);
}

/**
 * Create variable
 * @param variable
 */
void BBashHelper::createVar(std::shared_ptr<BVariable> variable) {
    std::stringstream ss;
    ss << std::endl;
    _indent(variable->getParentScope(),ss);
    ss << "# Create variable" << std::endl;
    _indent(variable->getParentScope(), ss);
    ss << "declare " << variable->getLabel().str() << "=" << variable->getDefaultValue() << std::endl;

    // Check if variable was initialized
    if(variable->getExpression()) {
        ss << std::endl;
        _indent(variable->getParentScope(), ss);
        ss << "# Initialize variable" << std::endl;
        _expressionToCode(variable->getParentScope(), variable->getExpression(), ss);
    }
    BGenerateCode::get().write(ss);
}

/**
 * Write bash
 * @param scope
 * @param token
 */
void BBashHelper::bash(std::shared_ptr<BScope> scope, std::shared_ptr<ecc::LexicalToken> token) {
    std::stringstream ss;
    ss << std::endl;
    _indent(scope, ss);
    ss << "# Run BASH code" << std::endl;

    _indent(scope, ss);
    if(token->getName() == BElementType::DATA_TYPE_NAME_BASH_INLINE) {
        ss << token->getValue().substr(2,token->getValue().length()-2);
    } else if(token->getName() == BElementType::DATA_TYPE_NAME_BASH_BLOCK) {
        ss << token->getValue().substr(2, token->getValue().length()-4);
    } else {
        throw BException("Bash code can only be generated by bash lexical tokens");
    }
    ss << std::endl;
    BGenerateCode::get().write(ss);
}

/**
 * Create function
 * @param function
 */
void BBashHelper::createFunction(std::shared_ptr<BFunction> function) {
    std::stringstream ss;
    ss << std::endl;
    ss << "# Create function" << std::endl;
    ss << "function " << function->getLabel().str() << "() {" << std::endl;
    int paramPos = 1;
    for(auto param : function->findAllParameters()) {

        // Check if a default value was set for that parameter
        ss << std::endl;
        if(param->getExpression()) {

            _indent(param->getParentScope(), ss);
            ss << "# Create argument with a default value" << std::endl;
            _indent(param->getParentScope(), ss);
            ss << "if (( " << paramPos-1 << " < $# )); then" << std::endl;
            _indent(param->getParentScope(), ss);
            ss << "declare " << param->getLabel().str() << "=\"${" << paramPos++ << "}\"" << std::endl;
            _indent(param->getParentScope(), ss);
            ss << "else" << std::endl;
            _indent(param->getParentScope(), ss);
            ss << "declare " << param->getLabel().str() << "=" << param->getDefaultValue() << std::endl;
            auto expression = _expressionToCode(param->getParentScope(), param->getExpression(), ss);
            _indent(param->getParentScope(), ss);
            ss << "fi" << std::endl;
        } else {
            _indent(param->getParentScope(), ss);
            ss << "# Create argument" << std::endl;
            _indent(param->getParentScope(), ss);
            ss << "declare " << param->getLabel().str() << "=\"${" << paramPos++ << "}\"" << std::endl;
        }
    }

    // Generate 'this' if function is a class member
    if(function->isClassMember()) {
        if(function->isConstructor()) {

            // Assign `this` and increment counter
            auto classScope = std::static_pointer_cast<BClass>(function->getParentScope());
            _indent(function, ss);
            ss << "declare " << FUNCTION_THIS << "=${" << _generateCounter(classScope) << "}" << std::endl;
            _indent(function, ss);
            ss << _generateCounter(classScope) << "=" << _arithOpForm1("${"+_generateCounter(classScope)+"}", "+", "1")
               << std::endl;

            // Declare variables
            ss << std::endl;
            _indent(function, ss);
            ss << "# Declaring and initialize variables" << std::endl;
            for(auto variable : classScope->findAllVariables()) {
                _indent(function, ss);
                ss << "# >> Declare" << std::endl;
                _indent(function, ss);
                ss << classScope->getLabel().str() << "[${" << FUNCTION_THIS << "},\""
                   << variable->getLabel().str() << "\"]=" << variable->getDefaultValue() << std::endl;

                // Check if initial expression was set
                if(variable->getExpression()) {
                    ss << std::endl;
                    _indent(function, ss);
                    ss << "# >> Initialize" << std::endl;
                    _expressionToCode(function, variable->getExpression(), ss);
                }
            }

        } else {
            _indent(function, ss);
            ss << "declare " << FUNCTION_THIS << "=${" << paramPos++ << "}" << std::endl;
        }
    }

    // Generate return reference if function requires a return statement
    if(function->requiresReturn() || function->isConstructor()) {
        ss << std::endl;
        _indent(function, ss);
        ss << "# Configure return statement" << std::endl;
        _indent(function, ss);
        ss << "declare -n " << FUNCTION_RETURN << "=${" << paramPos++ << "}" << std::endl;
    }

    BGenerateCode::get().write(ss);
}

/**
 * Close function
 * @param function
 */
void BBashHelper::closeFunction(std::shared_ptr<BFunction> function) {
    std::stringstream ss;

    if(function->isConstructor()) {
        ss << std::endl;
        _indent(function, ss);
        ss << "# Return statement" << std::endl;
        _indent(function, ss);
        ss << FUNCTION_RETURN << "=${" << FUNCTION_THIS << "}" << std::endl;
    }

    _indent(function->getParentScope(), ss);
    ss << "}" << std::endl;
    BGenerateCode::get().write(ss);
}

/**
 * Write expression
 * @param scope
 * @param expression
 */
void BBashHelper::writeExpression(std::shared_ptr<BScope> scope, std::shared_ptr<IBExpression> expression) {
    std::stringstream ss;
    ss << std::endl;
    _indent(scope, ss);
    ss << "# Evaluate expression" << std::endl;
    _expressionToCode(scope, expression, ss);
    BGenerateCode::get().write(ss);
}

/**
 * Write return statement
 * @param rtn
 */
void BBashHelper::writeReturn(std::shared_ptr<BReturn> rtn) {
    std::stringstream ss;

    // Add bash comment
    ss << std::endl;
    _indent(rtn->getParentScope(), ss);
    ss << "# Return statement" << std::endl;

    // Check if function has an expression, or is void
    if(rtn->getExpression()) {
        // Start processing the expression
        ExprReturn expression = _expressionToCode(rtn->getParentScope(), rtn->getExpression(), ss);

        // Write the return statement
        _indent(rtn->getParentScope(), ss);
        ss << FUNCTION_RETURN << "=" << expression.formattedValue() << std::endl;
    }

    // Return from bash function
    _indent(rtn->getParentScope(), ss);
    ss << "return" << std::endl;

    BGenerateCode::get().write(ss);
}

/**
 * Create if statement
 * @param ifStatement
 */
void BBashHelper::createIf(std::shared_ptr<BIf> ifStatement) {
    std::stringstream ss;

    // Add bash comment
    ss << std::endl;
    _indent(ifStatement->getParentScope(), ss);
    ss << "# If statement" << std::endl;

    // Start processing the expression
    ExprReturn expression = _expressionToCode(ifStatement->getParentScope(), ifStatement->getExpression(), ss);

    // Reset if statement lock
    _indent(ifStatement->getParentScope(), ss);
    ss << "declare " << _generateIfLock(ifStatement) << "=0" << std::endl;

    // Write if statement
    _indent(ifStatement->getParentScope(), ss);
    ss << "if (( " << expression.formattedValue() << " )); then" << std::endl;

    // Acquire the lock
    _indent(ifStatement->getParentScope(), ss);
    ss << _generateIfLock(ifStatement) << "=1" << std::endl;

    BGenerateCode::get().write(ss);
}

/**
 * Create elif statement
 * @param elifStatement
 */
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
    ExprReturn expression = _expressionToCode(elifStatement->getParentScope(), elifStatement->getExpression(), ss);

    // Write elif statement
    _indent(elifStatement->getParentScope(), ss);
    ss << "if (( " << expression.formattedValue() << " )); then" << std::endl;

    // Acquire lock
    _indent(elifStatement->getParentScope(), ss);
    ss << _generateIfLock(elifStatement->getParentIf()) << "=1" << std::endl;

    BGenerateCode::get().write(ss);
}

/**
 * Create else statement
 * @param elseStatement
 */
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

/**
 * Close if statement
 * @param ifStatement
 */
void BBashHelper::closeIf(std::shared_ptr<BIf> ifStatement) {
    std::stringstream ss;
    _indent(ifStatement->getParentScope(), ss);
    ss << "fi" << std::endl;
    BGenerateCode::get().write(ss);
}

/**
 * Close elif statement
 * @param elifStatement
 */
void BBashHelper::closeElif(std::shared_ptr<BElif> elifStatement) {
    std::stringstream ss;
    _indent(elifStatement->getParentScope(), ss);
    ss << "fi" << std::endl;
    _indent(elifStatement->getParentScope(), ss);
    ss << "fi" << std::endl;
    BGenerateCode::get().write(ss);
}

/**
 * Close else statement
 * @param elseStatement
 */
void BBashHelper::closeElse(std::shared_ptr<BElse> elseStatement) {
    std::stringstream ss;
    _indent(elseStatement->getParentScope(), ss);
    ss << "fi" << std::endl;
    BGenerateCode::get().write(ss);
}

/**
 * Create while statement
 * @param whileStatement
 */
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
    ExprReturn expression = _expressionToCode(whileStatement->getParentScope(), whileStatement->getExpression(), ss);

    // Create if statement
    _indent(whileStatement->getParentScope(), ss);
    ss << "! (( " << expression.formattedValue() << " )) && break" << std::endl;

    BGenerateCode::get().write(ss);
}

/**
 * Close while statement
 * @param whileStatement
 */
void BBashHelper::closeWhile(std::shared_ptr<BWhile> whileStatement) {
    std::stringstream ss;
    _indent(whileStatement->getParentScope(), ss);
    ss << "done" << std::endl;
    BGenerateCode::get().write(ss);
}