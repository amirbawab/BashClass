#ifndef BASHCLASS_BWHILE_H
#define BASHCLASS_BWHILE_H

#include <bashclass/BScope.h>
#include <bashclass/IBCallable.h>

class BWhile : public BScope {
private:

    // Condition/Expression for this if statement
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
