#include <bashclass/BFunctionCall.h>
#include <bashclass/BException.h>

std::shared_ptr<BFunctionChainCall> BFunctionCall::last() {
    if(!m_chain) {
        throw BException("Cannot get the last function chain call element before setting the chain");
    }
    return std::static_pointer_cast<BFunctionChainCall>(m_chain->last());
}

std::shared_ptr<IBType> BFunctionCall::getType() {
    return last()->getType();
}

void BFunctionCall::castType(std::shared_ptr<BElementType> type) {
    last()->getType()->cast(type);
}