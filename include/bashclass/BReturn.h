#ifndef BASHCLASS_BRETURN_H
#define BASHCLASS_BRETURN_H

#include <memory>
#include <bashclass/IBCallable.h>
#include <bashclass/BFunction.h>

class BReturn {
private:

    // Expression assigned to this return statement
    std::shared_ptr<IBCallable> m_expression;

    // Set the parent scope of this return statement
    std::shared_ptr<BScope> m_parentScope;

    // Set the function to which this return statement belongs to.
    // If the return statement is not in a scope inside the function (e.g. if, while, et...),
    // then the m_function and m_parentScope will be both pointing to the function holding
    // this return statement
    std::shared_ptr<BFunction> m_function;
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

    /**
     * Set the function to which the return statement belongs
     * @param function
     */
    void setFunction(std::shared_ptr<BFunction> function) {m_function = function;}

    /**
     * Get the function to which the return statement belongs
     * @return
     */
    std::shared_ptr<BFunction> getFunction() {return m_function;}
};

#endif
