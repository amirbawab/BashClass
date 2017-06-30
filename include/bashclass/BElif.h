#ifndef BASHCLASS_BELIF_H
#define BASHCLASS_BELIF_H

#include <bashclass/BScope.h>
#include <bashclass/IBExpression.h>
#include <bashclass/BIf.h>

class BElif : public BScope {
private:
    // Condition/Expression for this elif statement
    std::shared_ptr<IBExpression> m_expression;

    // Parent if statement
    std::shared_ptr<BIf> m_parentIf;
public:
    /**
     * Get if label
     */
    std::stringstream getLabel();

    /**
     * Get expression/condition
     * @return expression
     */
    std::shared_ptr<IBExpression> getExpression() {return m_expression;}

    /**
     * Set expression/condition
     * @param expression
     */
    void setExpression(std::shared_ptr<IBExpression> expression);

    /**
     * Set parent if
     * @param parentIf
     */
    void setParentIf(std::shared_ptr<BIf> parentIf) {m_parentIf = parentIf;}

    /**
     * Get parent if
     * @return parent if
     */
    std::shared_ptr<BIf> getParentIf() {return m_parentIf;}
};

#endif
