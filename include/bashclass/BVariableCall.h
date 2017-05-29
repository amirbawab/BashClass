#ifndef BASHCLASS_BVARIABLECALL_H
#define BASHCLASS_BVARIABLECALL_H

#include <bashclass/BVariable.h>
#include <bashclass/IBCallable.h>
#include <memory>

class BVariableCall : public IBCallable {
private:
    std::shared_ptr<BVariable> m_variable;
public:

    /**
     * Get variable
     * @return  variable
     */
    std::shared_ptr<BVariable> getVariable() { return m_variable;}

    /**
     * Set variable
     * @param variable
     */
    void setVariable(std::shared_ptr<BVariable> variable) {m_variable = variable;}
};


#endif
