#ifndef BASHCLASS_BIF_H
#define BASHCLASS_BIF_H

#include <bashclass/BScope.h>

class BIf : public BScope {
private:
    // Condition/Expression for this if statement
    std::shared_ptr<IBCallable> m_expression;
public:
    /**
     * Get if label
     */
    std::stringstream getLabel();

    /**
     * Get expression/condition
     * @return expression
     */
    std::shared_ptr<IBCallable> getExpression() {return m_expression;}

    /**
     * Set expression/condition
     * @param expression
     */
    void setExpression(std::shared_ptr<IBCallable> expression);
};

#endif
