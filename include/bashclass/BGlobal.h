#ifndef BASHCLASS_BGLOBAL_H
#define BASHCLASS_BGLOBAL_H

#include <vector>
#include <memory>
#include <bashclass/BClass.h>
#include <bashclass/BFunction.h>
#include <bashclass/BScope.h>

class BGlobal : public BScope {
private:
    std::vector<std::shared_ptr<BClass>> m_classes;
    std::vector<std::shared_ptr<BFunction>> m_functions;
public:
    std::shared_ptr<BClass> createClass();
    std::shared_ptr<BFunction> createFunction();
};

#endif
