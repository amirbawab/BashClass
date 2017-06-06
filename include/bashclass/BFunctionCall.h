#ifndef BASHCLASS_BFUNCTIONCALL_H
#define BASHCLASS_BFUNCTIONCALL_H

#include <bashclass/IBExpression.h>
#include <bashclass/BChain.h>
#include <bashclass/BFunctionChainCall.h>

class BFunctionCall : public IBExpression {
private:

    // Hold chain for the function call
    std::shared_ptr<BChain> m_chain;
public:

    /**
     * Get the type value of the last element
     * in the chain which is a function call
     * @return type as string
     */
    std::string getTypeValueAsString();

    /**
     * Get the last element in the chain
     * @return function call
     */
    std::shared_ptr<BFunctionChainCall> last();

};

#endif
