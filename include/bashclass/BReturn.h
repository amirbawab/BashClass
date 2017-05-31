#ifndef BASHCLASS_BRETURN_H
#define BASHCLASS_BRETURN_H

#include <memory>
#include <bashclass/IBCallable.h>

class BReturn {
private:
    std::shared_ptr<IBCallable> m_expression;
public:

    /**
     * Set return statement expression
     * @param return statement expression
     */
    void setExpression(std::shared_ptr<IBCallable> expression) {m_expression = expression;}

    /**
     * Get return statement expression
     * @return return statement expression
     */
    std::shared_ptr<IBCallable> getExpression(){return m_expression;}
};

#endif
