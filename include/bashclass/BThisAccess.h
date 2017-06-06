#ifndef BASHCLASS_BTHISACCESS_H
#define BASHCLASS_BTHISACCESS_H

#include <bashclass/IBExpression.h>
#include <bashclass/BThisChainAccess.h>

class BThisAccess : public IBExpression {
private:

    // Hold this reference access
    std::shared_ptr<BThisChainAccess> m_thisChainAccess;
public:

    /**
     * Get the type value of the this reference
     * @return type value as a string
     */
    std::string getTypeValueAsString();

    /**
     * Set this chain access
     * @param thisChainAccess
     */
    void setThisChainAccess(std::shared_ptr<BThisChainAccess> thisChainAccess) {m_thisChainAccess = thisChainAccess;}

    /**
     * Get this chain access
     * @return thisChainAccess
     */
    std::shared_ptr<BThisChainAccess> getThisChainAccess() {return m_thisChainAccess;}
};

#endif
