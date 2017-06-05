#include <bashclass/BBashHelper.h>
#include <bashclass/BGenerateCode.h>
#include <bashclass/BTypes.h>
#include <bashclass/BException.h>
#include <bashclass/BGlobal.h>
#include <bashclass/BVariableChainCall.h>
#include <bashclass/BFunctionChainCall.h>
#include <iostream>
#include <bashclass/BExpressionCall.h>

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
 * Generate code for a variable call that is a member and is placed first in the chain
 * @param variableCall
 * @param ss
 */
void _varCall_member_first(std::shared_ptr<BVariableChainCall> variableCall, std::stringstream &ss) {
    ss << variableCall->getVariable()->getParentScope()->findClosestClass()->getLabel().str()
       << "[" << FUNCTION_THIS << "," << variableCall->getVariable()->getLabel().str() << "]";
}

/**
 * Generate code for a variable call that is a member and is placed in the middle of the chain
 * @param variableCall
 * @param prevTypeLabel
 * @param prevResult
 * @param ss
 */
void _varCall_member_middle(std::shared_ptr<BVariableChainCall> variableCall, std::string prevTypeLabel,
                            std::string prevResult, std::stringstream &ss) {
    ss << prevTypeLabel << "[" << prevResult << "," << variableCall->getVariable()->getLabel().str() << "]" ;
}

/**
 * Generate code for a variable call that is a member and is placed last in the chain
 * @param variableCall
 * @param prevResult
 * @param ss
 */
void _varCall_member_last(std::shared_ptr<BVariableChainCall> variableCall, std::string prevResult, std::stringstream &ss) {
    ss << variableCall->getVariable()->getParentScope()->findClosestClass()->getLabel().str()
       << "[" << prevResult << "," << variableCall->getVariable()->getLabel().str() << "]";
}

/**
 * Generate code for a variable call that is not a member
 * @param variableCall
 * @param ss
 */
void _varCall_nonMember(std::shared_ptr<BVariableChainCall> variableCall, std::stringstream &ss) {
    ss << variableCall->getVariable()->getLabel().str();
}

/**
 * Generate code for a chain call
 * @param chainCall
 * @return multi-line code representing a chain call
 */
void _chainCallToCode(std::shared_ptr<BChainCall> chainCall, std::stringstream &ss) {

    // Have a unique key for each result of an element call
    std::map<std::shared_ptr<IBSimpleCallable>, std::string> returnMap;

    // Assign a unique key for each result
    static unsigned int uniqueId = 0;

    // Start processing the calls
    for (size_t i = 0; i < chainCall->size(); i++) {

        // Cast element
        auto variableCallCast = std::dynamic_pointer_cast<BVariableChainCall>((*chainCall)[i]);
        auto functionCallCast = std::dynamic_pointer_cast<BFunctionChainCall>((*chainCall)[i]);
        auto thisCallCast = std::dynamic_pointer_cast<BThisChainCall>((*chainCall)[i]);
        auto tokenCallCast = std::dynamic_pointer_cast<BTokenCall>((*chainCall)[i]);

        // Generate indentation
        _indent(chainCall->getParentScope(), ss);

        if (variableCallCast) {
            if(i == chainCall->size()-1) {
                if(variableCallCast->getVariable()->isClassMember()) {
                    if(i == 0) {
                        _varCall_member_first(variableCallCast, ss);
                    } else {
                        _varCall_member_last(variableCallCast, returnMap[(*chainCall)[i-1]], ss);
                    }
                } else {
                    _varCall_nonMember(variableCallCast, ss);
                }
                ss << "=";

            } else if(i == 0) {
                // Generate new key for this variable
                std::string newKey = _generateResultKey(uniqueId++);
                returnMap[variableCallCast] = newKey;
                ss << newKey << "=";

                if(variableCallCast->getVariable()->isClassMember()) {
                    _varCall_member_first(variableCallCast, ss);
                } else {
                    _varCall_nonMember(variableCallCast, ss);
                }
                ss << std::endl;

            } else {
                // Generate new key for this variable
                std::string newKey = _generateResultKey(uniqueId++);
                returnMap[variableCallCast] = newKey;
                ss << newKey << "=";

                _varCall_member_middle(variableCallCast, (*chainCall)[i-1]->getTypeScope()->getLabel().str(),
                                       returnMap[(*chainCall)[i-1]], ss);
                ss << std::endl;
            }

        } else if (functionCallCast) {

            ss << functionCallCast->getFunction()->getLabel().str();

            // TODO Put all arguments
            ss << " args...";

            if(i == 0) {
                if(functionCallCast->getFunction()->isClassMember()) {
                    ss << " " << FUNCTION_THIS;
                }
            } else {
                ss << " " << returnMap[(*chainCall)[i-1]];
            }

            if(functionCallCast->getFunction()->requiresReturn()) {
                std::string newKey = _generateResultKey(uniqueId++);
                returnMap[functionCallCast] = newKey;
                ss << " " << newKey;
            }
            ss << std::endl;
        } else if(thisCallCast) {
            std::string newKey = _generateResultKey(uniqueId++);
            returnMap[thisCallCast] = newKey;
            ss  << newKey << "=" << FUNCTION_THIS << std::endl;
        } else if(tokenCallCast) {
            ss << tokenCallCast->getLexicalToken()->getValue();
        } else {
            throw BException("Cannot generate code for an unrecognized element call");
        }
    }
}

void _expressionToCode(std::shared_ptr<BExpressionCall> expression, std::stringstream &ss) {
    auto leftOperandExpression = std::dynamic_pointer_cast<BExpressionCall>(expression->getLeftOperand());
    auto leftOperandChain = std::dynamic_pointer_cast<BChainCall>(expression->getLeftOperand());
    auto rightOperandExpression = std::dynamic_pointer_cast<BExpressionCall>(expression->getRightOperand());
    auto rightOperandChain = std::dynamic_pointer_cast<BChainCall>(expression->getRightOperand());

    if(leftOperandExpression) {
        _expressionToCode(leftOperandExpression, ss);
    } else if(leftOperandChain) {
        _chainCallToCode(leftOperandChain, ss);
    } else {
        throw BException("Left operand is not recognized");
    }

    ss << " " << expression->getOperator()->getValue() << " ";

    if(rightOperandExpression) {
        _expressionToCode(rightOperandExpression, ss);
    } else if(rightOperandChain) {
        _chainCallToCode(rightOperandChain, ss);
    } else {
        throw BException("Right operand is not recognized");
    }
    ss << std::endl;
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

void BBashHelper::assignVariable(std::shared_ptr<BChainCall> chainCall) {
    std::stringstream ss;

    // TOD Add expression
    auto variableCall = std::static_pointer_cast<BVariableChainCall>(chainCall->last());
    auto expressionComposite = std::dynamic_pointer_cast<BExpressionCall>(variableCall->getExpression());
    auto chainComposite = std::dynamic_pointer_cast<BChainCall>(variableCall->getExpression());

    // Start converting the expression
    ss << std::endl;
    _indent(chainCall->getParentScope(), ss);
    ss << "# Evaluating expression" << std::endl;
    // TODO Create a function/Adjust current one to convert chain into code for inner calls
    if(chainComposite) {
        _chainCallToCode(chainComposite, ss);
    } else if(expressionComposite) {
        _expressionToCode(expressionComposite, ss);
    } else {
        throw BException("Unrecognized value of a variable call");
    }

    // Convert chain to code
    ss << std::endl;
    _indent(chainCall->getParentScope(), ss);
    ss << "# Assign variable" << std::endl;
    _chainCallToCode(chainCall, ss);

    ss << std::endl;
    BGenerateCode::get().write(ss);
}

void BBashHelper::functionExec(std::shared_ptr<BChainCall> chainCall) {
    std::stringstream ss;

    // Convert chain to code
    ss << std::endl;
    _indent(chainCall->getParentScope(), ss);
    ss << "# Execute a function" << std::endl;
    _chainCallToCode(chainCall, ss);
    ss << std::endl;

    BGenerateCode::get().write(ss);

}