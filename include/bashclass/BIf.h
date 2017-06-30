#ifndef BASHCLASS_BIF_H
#define BASHCLASS_BIF_H

#include <bashclass/BScope.h>
#include <bashclass/IBExpression.h>

class BElif;
class BElse;
class BIf : public BScope {
private:
    // Condition/Expression for this if statement
    std::shared_ptr<IBExpression> m_expression;

    // Elif scopes in order
    std::vector<std::shared_ptr<BElif>> m_elifScopes;

    // Else scope for this if statement
    std::shared_ptr<BElse> m_elseScope;
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
     * Add elif scope
     * @param elifScope
     */
    void addElif(std::shared_ptr<BElif> elifScope);

    /**
     * Get elif scopes in the order of insertion
     * @return elif scopes
     */
    const std::vector<std::shared_ptr<BElif>>& getElifScopes() const { return m_elifScopes; }

    /**
     * Set the else scope
     * @param elseScope
     */
    void setElse(std::shared_ptr<BElse> elseScope);

    /**
     * Get the else scope
     * @return else scope
     */
    std::shared_ptr<BElse> getElse() {return m_elseScope;}
};

#endif
