#ifndef BASHCLASS_BWHILE_H
#define BASHCLASS_BWHILE_H

#include <bashclass/BScope.h>

class BWhile : public BScope {
private:
    std::shared_ptr<IBCallable> m_expression;
public:
    /**
     * Get while label
     */
    std::stringstream getLabel();

    /**
     * Get expression/condition
     * @return expression
     */
    std::shared_ptr<IBCallable> getExpression(){return m_expression;}

    /**
     * Set expression/condition
     * @param expression
     */
    void setExpression(std::shared_ptr<IBCallable> expression);
};

#endif
