#ifndef BASHCLASS_BIF_H
#define BASHCLASS_BIF_H

#include <bashclass/BScope.h>

class BIf : public BScope {
private:
    std::shared_ptr<IBCallable> m_condition;
public:
    /**
     * Get if label
     */
    std::stringstream getLabel();

    /**
     * Get condition
     * @return condition
     */
    std::shared_ptr<IBCallable> getCondition() {return m_condition;}

    /**
     * Set condition
     * @param condition
     */
    void setCondition(std::shared_ptr<IBCallable> condition) {m_condition = condition;}
};

#endif
