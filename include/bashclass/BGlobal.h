#ifndef BASHCLASS_BGLOBAL_H
#define BASHCLASS_BGLOBAL_H

#include <vector>
#include <memory>
#include <bashclass/BClass.h>
#include <bashclass/BFunction.h>
#include <bashclass/BScopeFunc.h>

class BGlobal : public BScopeFunc {
private:
    std::vector<std::shared_ptr<BClass>> m_classes;
public:
    std::shared_ptr<BClass> createClass();
    std::vector<std::shared_ptr<BClass>> findAllClasses(std::string name);
};

#endif
