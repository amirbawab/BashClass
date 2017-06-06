#ifndef BASHCLASS_BVARIABLEACCESS_H
#define BASHCLASS_BVARIABLEACCESS_H

#include <bashclass/IBExpression.h>
#include <bashclass/BChain.h>
#include <bashclass/BVariableChainAccess.h>

class BVariableAccess : public IBExpression {
private:

    // Hold chain for the variable access
    std::shared_ptr<BChain> m_chain;
public:

    /**
     * Get the type value of the last element
     * in the chain which is a variable access
     * @return type as string
     */
    std::string getTypeValueAsString();

    /**
     * Get the last element in the chain
     * @return variable chain access
     */
    std::shared_ptr<BVariableChainAccess> last();
};

#endif
