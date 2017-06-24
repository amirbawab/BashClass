#ifndef BASHCLASS_BVARIABLECHAINACCESS_H
#define BASHCLASS_BVARIABLECHAINACCESS_H

#include <bashclass/BVariable.h>
#include <bashclass/IBChainable.h>
#include <bashclass/BExpressionType.h>
#include <memory>

class BVariableChainAccess : public IBChainable {
private:

    // The variable this variable call refers to.
    // If this variable is not set, then the actual variable
    // was not found
    std::shared_ptr<BVariable> m_variable;

    // Hold the type of this variable access
    std::shared_ptr<BExpressionType> m_type;
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
    void setVariable(std::shared_ptr<BVariable> variable);

    /**
     * Check if reference is found
     * @return true if found
     */
    bool isFound();

    /**
     * Get variable type
     * @return type
     */
    std::shared_ptr<IBType> getType();
};


#endif
