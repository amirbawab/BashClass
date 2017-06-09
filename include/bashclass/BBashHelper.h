#ifndef BASHCLASS_BBASHHELPER_H
#define BASHCLASS_BBASHHELPER_H

#include <memory>
#include <bashclass/BClass.h>
#include <bashclass/BVariable.h>
#include <bashclass/BFunction.h>
#include <bashclass/BChain.h>
#include <bashclass/BVariableAssign.h>
#include <bashclass/BFunctionCall.h>
#include <bashclass/BReturn.h>

class BBashHelper {
public:

    /**
     * Generate header bash code
     */
    static void header();

    /**
     * Generate footer bash code
     */
    static void footer();

    /**
     * Generate an associative array
     * @param classScope
     */
    static void declareClass(std::shared_ptr<BClass> classScope);

    /**
     * Generate a unique counter for a class
     * @param classScope
     */
    static void uniqueCounter(std::shared_ptr<BClass> classScope);

    /**
     * Generate a global variable in bash
     * @param variable
     * @param defaultValue
     */
    static void createGlobalVar(std::shared_ptr<BVariable> variable, std::string defaultValue="");

    /**
     * Genera a local variable in bash
     * @param variable
     * @param defaultValue
     */
    static void createLocalVar(std::shared_ptr<BVariable> variable, std::string defaultValue="");

    /**
     * Generate a function in bash
     * @param function
     */
    static void createFunction(std::shared_ptr<BFunction> function);

    /**
     * Close a function in bash
     * @param function
     */
    static void closeFunction(std::shared_ptr<BFunction> function);

    /**
     * Write pure bash
     * @param lexicalToken
     */
    static void bash(std::shared_ptr<ecc::LexicalToken> lexicalToken);

    /**
     * Generate a variable assign
     * @param variableAssign
     */
    static void assignVariable(std::shared_ptr<BVariableAssign> variableAssign);

    /**
     * Generate call for a function execution
     * @param functionCall
     */
    static void functionExec(std::shared_ptr<BFunctionCall> functionCall);

    /**
     * Generate code for a return statement
     * @param rtn
     */
    static void writeReturn(std::shared_ptr<BReturn> rtn);
};

#endif
