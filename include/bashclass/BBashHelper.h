#ifndef BASHCLASS_BBASHHELPER_H
#define BASHCLASS_BBASHHELPER_H

#include <memory>
#include <bashclass/BClass.h>
#include <bashclass/BVariable.h>
#include <bashclass/BFunction.h>
#include <bashclass/BChainCall.h>

class BBashHelper {
public:
    static void header();
    static void footer();
    static void declareClass(std::shared_ptr<BClass> classScope);
    static void uniqueCounter(std::shared_ptr<BClass> classScope);
    static void createGlobalVar(std::shared_ptr<BVariable> variable, std::string defaultValue="");
    static void createLocalVar(std::shared_ptr<BVariable> variable, std::string defaultValue="");
    static void createFunction(std::shared_ptr<BFunction> function);
    static void closeFunction(std::shared_ptr<BFunction> function);
    static void bash(std::shared_ptr<ecc::LexicalToken> lexicalToken);
    static void assignVariable(std::shared_ptr<BChainCall> chainCall);
};

#endif
