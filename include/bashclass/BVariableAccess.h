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
     * Get the type value as a string of the last element
     * in the chain which is a variable chain access
     * @return type as string
     */
    std::string getTypeValueAsString();

    /**
     * Get the last element in the chain which is of type variable chain access
     * @return variable chain access
     */
    std::shared_ptr<BVariableChainAccess> last();

    /**
     * Get chain
     * @return chain
     */
    std::shared_ptr<BChain> getChain() {return m_chain;}

    /**
     * Set chain
     * @param chain
     */
    void setChain(std::shared_ptr<BChain> chain) { m_chain = chain;}
};

#endif
