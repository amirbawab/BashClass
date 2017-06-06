#ifndef BASHCLASS_BVARIABLEASSIGN_H
#define BASHCLASS_BVARIABLEASSIGN_H

#include <bashclass/IBExpression.h>
#include <bashclass/BVariableAccess.h>

class BVariableAssign {
private:

    // Hold the variable that is being assigned
    std::shared_ptr<BVariableAccess> m_variableAccess;

    // Hold the expression value of the variable
    std::shared_ptr<IBExpression> m_expression;
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
     * Set variable access
     * @param variableAccess
     */
    void setVariableAccess(std::shared_ptr<BVariableAccess> variableAccess) { m_variableAccess = variableAccess;}

    /**
     * Get variable access
     * @return variable access
     */
    std::shared_ptr<BVariableAccess> getVariableAccess() {return m_variableAccess;}
};

#endif
