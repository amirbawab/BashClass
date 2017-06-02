#ifndef BASHCLASS_BBASHHELPER_H
#define BASHCLASS_BBASHHELPER_H

#include <memory>
#include <bashclass/BClass.h>
#include <bashclass/BVariable.h>

class BBashHelper {
public:
    static void header();
    static void footer();
    static void declareClass(std::shared_ptr<BClass> classScope);
    static void uniqueCounter(std::shared_ptr<BClass> classScope);
    static void createGlobalVar(std::shared_ptr<BVariable> variable);
    static void createLocalVar(std::shared_ptr<BVariable> variable);
};

#endif
