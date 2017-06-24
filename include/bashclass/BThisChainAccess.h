#ifndef BASHCLASS_BTHISCHAINACCESS_H
#define BASHCLASS_BTHISCHAINACCESS_H

#include <bashclass/IBChainable.h>
#include <bashclass/BClass.h>
#include <easycc/LexicalToken.h>
#include <memory>

class BThisChainAccess : public IBChainable {
private:

    // The class that this reference points to
    std::shared_ptr<BClass> m_reference;
public:

    /**
    * Get reference class
    * @return reference
    */
    std::shared_ptr<BClass> getReference() {return m_reference;}

    /**
     * Set reference class
     * @param reference
     */
    void setReference(std::shared_ptr<BClass> reference) {m_reference = reference;}

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
