#ifndef BASHCLASS_BCHAINCALL_H
#define BASHCLASS_BCHAINCALL_H

#include <bashclass/IBCallable.h>
#include <memory>
#include <vector>

class BChainCall : public IBCallable {
private:
    std::vector<std::shared_ptr<IBCallable>> m_callables;
};

#endif
