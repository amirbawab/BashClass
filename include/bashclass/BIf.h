#ifndef BASHCLASS_BIF_H
#define BASHCLASS_BIF_H

#include <bashclass/BScope.h>
#include <bashclass/IBCompositeCallable.h>

class BIf : public BScope {
private:
    // Condition/Expression for this if statement
    std::shared_ptr<IBCompositeCallable> m_expression;
public:
    /**
     * Get if label
     */
    std::stringstream getLabel();

    /**
     * Get expression/condition
     * @return expression
     */
    std::shared_ptr<IBCompositeCallable> getExpression() {return m_expression;}

    /**
     * Set expression/condition
     * @param expression
     */
    void setExpression(std::shared_ptr<IBCompositeCallable> expression);

    /**
     * Find closest function by asking the parent scope
     * @return function scope or nullptr if not found
     */
    std::shared_ptr<BFunction> findClosestFunction() {return m_parentScope->findClosestFunction(); }

    /**
     * Find closest class by asking the parent scope
     * @return class scope or nullptr if not found
     */
    std::shared_ptr<BClass> findClosestClass() { return m_parentScope->findClosestClass(); }
};

#endif
