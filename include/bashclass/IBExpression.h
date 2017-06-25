#ifndef BASHCLASS_IBEXPRESSION_H
#define BASHCLASS_IBEXPRESSION_H

#include <bashclass/IBType.h>

class IBExpression {
public:

    /**
     * Get the type of the expression element
     * @return type value
     */
    virtual std::shared_ptr<IBType> getType()=0;

    /**
     * Cast type
     * @param type
     */
    virtual void castType(std::shared_ptr<BElementType> type)=0;
};

#endif
