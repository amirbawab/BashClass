#ifndef BASHCLASS_BWHILE_H
#define BASHCLASS_BWHILE_H

#include <bashclass/BScope.h>
#include <bashclass/BExpression.h>

class BWhile : public BScope {
private:
    std::shared_ptr<BExpression> m_condition;
public:
    /**
     * Get while label
     */
    std::stringstream getLabel();

    /**
     * Set while condition
     * @param condition
     */
    void setCondition(std::shared_ptr<BExpression> condition) {m_condition = condition;}

    /**
     * Get while condition
     * @return condition
     */
    std::shared_ptr<BExpression> getCondition() {return m_condition;}
};

#endif
