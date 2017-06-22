#ifndef BASHCLASS_BTHISACCESS_H
#define BASHCLASS_BTHISACCESS_H

#include <bashclass/IBExpression.h>
#include <bashclass/BThisChainAccess.h>
#include <bashclass/BChain.h>

class BThisAccess : public IBExpression {
private:

    // Hold chain which will contain only a this
    std::shared_ptr<BChain> m_chain;
public:

    /**
     * Set this chain access
     * @param chain
     */
    void setChain(std::shared_ptr<BChain> chain);

    /**
     * Get this chain access
     * @return thisChainAccess
     */
    std::shared_ptr<BThisChainAccess> getThisChainAccess();

    /**
     * Get type
     * @return type
     */
    std::shared_ptr<IBType> getType();
};

#endif
