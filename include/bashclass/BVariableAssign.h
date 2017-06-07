#ifndef BASHCLASS_BVARIABLEASSIGN_H
#define BASHCLASS_BVARIABLEASSIGN_H

#include <bashclass/IBExpression.h>
#include <bashclass/BVariableAccess.h>

class BVariableAssign {
private:

    // Hold the variable access that is being assigned
    std::shared_ptr<BVariableAccess> m_variableAccess;

    // Hold the expression value of the variable
    std::shared_ptr<IBExpression> m_expression;

    // Hold the parent scope of this statement
    std::shared_ptr<BScope> m_parentScope;
public:

    /**
     * Set variable expression value
     * @param expression
     */
    void setExpression(std::shared_ptr<IBExpression> expression);

    /**
     * Get expression value
     * @return expression value
     */
    std::shared_ptr<IBExpression> getExpression() { return m_expression;}

    /**
     * Get parent scope
     * @return parent scope
     */
    std::shared_ptr<BScope> getParentScope() {return m_parentScope;}

    /**
     * Set parent scope
     * @param parentScope
     */
    void setParentScope(std::shared_ptr<BScope> parentScope) {m_parentScope = parentScope;}

    /**
     * Get variable access
     * @return variableAccess
     */
    std::shared_ptr<BVariableAccess> getVariableAccess() {return m_variableAccess;}

    /**
     * Set variable access
     * @param variableAccess
     */
    void setVariableAccess(std::shared_ptr<BVariableAccess> variableAccess) {m_variableAccess = variableAccess;}
};

#endif
