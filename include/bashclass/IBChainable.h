#ifndef BASHCLASS_IBCHAINABLE_H
#define BASHCLASS_IBCHAINABLE_H

class IBChainable {
public:

    /**
     * Get the type of the chainable element
     * @return type value as a string
     */
    virtual std::string getTypeValueAsString()=0;
};

#endif
