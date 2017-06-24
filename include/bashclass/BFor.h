#ifndef BASHCLASS_BFOR_H
#define BASHCLASS_BFOR_H

#include <bashclass/BScope.h>

class BFor : public BScope {
private:

    // Hold expression to execute initially
    std::shared_ptr<IBExpression> m_preCond;

    // Hold condition of the for loop
    std::shared_ptr<IBExpression> m_cond;

    // Hold expression to execute at the end of each iteration
    std::shared_ptr<IBExpression> m_postCond;

public:

    /**
     * Get for label
     * @return label
     */
    std::stringstream getLabel();

    /**
     * Find closest function by asking the parent scope
     * @return function scope or nullptr if not found
     */
    std::shared_ptr<BFunction> findClosestFunction();

    /**
     * Find closest class by asking the parent scope
     * @return class scope or nullptr if not found
     */
    std::shared_ptr<BClass> findClosestClass();

    /**
     * Get pre condition
     * @return pre condition
     */
    std::shared_ptr<IBExpression> getPreCondition() { return m_preCond;}

    /**
     * Set condition
     * @param expression
     */
    void setPreCondition(std::shared_ptr<IBExpression> expression) { m_preCond = expression;}

    /**
     * Get condition
     * @return condition
     */
    std::shared_ptr<IBExpression> getCondition() { return m_cond;}

    /**
     * Set condition
     * @param expression
     */
    void setCondition(std::shared_ptr<IBExpression> expression);

    /**
     * Get post condition
     * @return post condition
     */
    std::shared_ptr<IBExpression> getPostCondition() { return m_postCond;}

    /**
     * Set post condition
     * @param expression
     */
    void setPostCondition( std::shared_ptr<IBExpression> expression) { m_postCond = expression;}
};

#endif