#ifndef BASHCLASS_IBCHAINABLE_H
#define BASHCLASS_IBCHAINABLE_H

class IBChainable {
public:

    /**
     * Get the type of the chainable element
     * @return type value as a string
     */
    virtual std::string getTypeValueAsString()=0;

    /**
     * Check if the element is known
     * @return true if the element is recognized
     */
    virtual bool isKnown()=0;

    /**
     * Get a pointer to the class defining the type
     * of the element
     * @return pointer to a class instance
     */
    virtual std::shared_ptr<BClass> getTypeScope()=0;
};

#endif
