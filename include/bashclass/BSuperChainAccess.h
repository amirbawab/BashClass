#ifndef BASHCLASS_BSUPERCHAINACCESS_H
#define BASHCLASS_BSUPERCHAINACCESS_H

#include <bashclass/IBChainable.h>

class BSuperChainAccess : public IBChainable {
private:
    // The source class from where the super is call
    std::shared_ptr<BClass> m_srcReference;
public:

    /**
    * Get reference class
    * @return reference
    */
    std::shared_ptr<BClass> getSrcReference() {return m_srcReference;}

    /**
     * Set reference class
     * @param reference
     */
    void setSrcReference(std::shared_ptr<BClass> reference) {m_srcReference = reference;}

    /**
     * Check if reference is found
     * @return true if found
     */
    bool isFound();

    /**
     * Get this reference type
     * @return type
     */
    std::shared_ptr<IBType> getType();
};

#endif
