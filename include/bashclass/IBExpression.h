#ifndef BASHCLASS_IBEXPRESSION_H
#define BASHCLASS_IBEXPRESSION_H

#include <string>

class IBExpression {
public:

    /**
     * Get the type of the callable element
     * @return type value as a string
     */
    virtual std::string getTypeValueAsString()=0;
};

#endif
