#ifndef BASHCLASS_BWHILE_H
#define BASHCLASS_BWHILE_H

#include <bashclass/BScope.h>
#include <bashclass/IBExpression.h>

class BWhile : public BScope {
private:

    // Condition/Expression for this while statement
    std::shared_ptr<IBExpression> m_expression;
public:
    /**
     * Get while label
     */
    std::stringstream getLabel();

    /**
     * Get expression/condition
     * @return expression
     */
    std::shared_ptr<IBExpression> getExpression(){return m_expression;}

    /**
     * Set expression/condition
     * @param expression
     */
    void setExpression(std::shared_ptr<IBExpression> expression);

    /**
     * Find closest class by asking the parent scope
     * @return class scope or nullptr if not found
     */
    std::shared_ptr<BClass> findClosestClass();
};

#endif
