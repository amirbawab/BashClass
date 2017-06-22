#ifndef BASHCLASS_IBCHAINABLE_H
#define BASHCLASS_IBCHAINABLE_H

#include <memory>
#include <string>
#include <bashclass/BClass.h>
#include <bashclass/IBType.h>

class IBChainable {
public:

    /**
     * Check if the element is known
     * @return true if the element is recognized
     */
    virtual bool isFound()=0;

    /**
     * Get the type of the chainable element
     * @return type
     */
    virtual std::shared_ptr<IBType> getType()=0;
};

#endif
