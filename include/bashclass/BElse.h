#ifndef BASHCLASS_BELSE_H
#define BASHCLASS_BELSE_H

#include <bashclass/BScope.h>
#include <bashclass/IBExpression.h>
#include <bashclass/BIf.h>

class BElse : public BScope {
private:
    // Parent if statement
    std::shared_ptr<BIf> m_parentIf;
public:
    /**
     * Get else label
     */
    std::stringstream getLabel();

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
