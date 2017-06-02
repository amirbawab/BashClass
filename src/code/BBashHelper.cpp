#include <bashclass/BBashHelper.h>
#include <bashclass/BGenerateCode.h>

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

void BBashHelper::createGlobalVar(std::shared_ptr<BVariable> variable) {
    std::stringstream ss;
    ss << variable->getLabel().str() << "=" << std::endl;
    BGenerateCode::get().write(ss);
}

void BBashHelper::createLocalVar(std::shared_ptr<BVariable> variable) {
    std::stringstream ss;
    ss << "local " << variable->getLabel().str() << "=" << std::endl;
    BGenerateCode::get().write(ss);
}