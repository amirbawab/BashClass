#ifndef BASHCLASS_BRETURN_H
#define BASHCLASS_BRETURN_H

#include <memory>
#include <bashclass/IBCompositeCallable.h>
#include <bashclass/BFunction.h>

class BReturn {
private:

    // Expression assigned to this return statement
    std::shared_ptr<IBCompositeCallable> m_expression;

    // Set the parent scope of this return statement
    std::shared_ptr<BScope> m_parentScope;
public:

    /**
     * Set return statement expression
     * @param return statement expression
     */
    void setExpression(std::shared_ptr<IBCompositeCallable> expression) {m_expression = expression;}

    /**
     * Get return statement expression
     * @return return statement expression
     */
    std::shared_ptr<IBCompositeCallable> getExpression(){return m_expression;}

    /**
     * Verify return
     */
    void verifyReturn();

    /**
     * Set parent scope
     * @param parentScope
     */
    void setParentScope(std::shared_ptr<BScope> parentScope) {m_parentScope = parentScope;}

    /**
     * Get parent scope
     * @return parent scope
     */
    std::shared_ptr<BScope> getParentScope() {return m_parentScope;}
};

#endif
