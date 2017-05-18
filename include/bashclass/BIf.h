#ifndef BASHCLASS_BIF_H
#define BASHCLASS_BIF_H

#include <bashclass/BScope.h>
#include <bashclass/BExpression.h>

class BIf : public BScope {
private:
    std::shared_ptr<BExpression> m_condition;
public:
    /**
     * Get if label
     */
    std::stringstream getLabel();

    /**
     * Set if condition
     * @param condition
     */
    void setCondition(std::shared_ptr<BExpression> condition) {m_condition = condition;}

    /**
     * Get if condition
     * @return condition
     */
    std::shared_ptr<BExpression> getCondition() {return m_condition;}
};

#endif
