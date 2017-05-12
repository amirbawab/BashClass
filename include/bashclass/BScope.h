#ifndef BASHCLASS_BSCOPE_H
#define BASHCLASS_BSCOPE_H

#include <vector>
#include <memory>
#include <bashclass/BVariable.h>

class BScope : public std::enable_shared_from_this<BScope> {
protected:
    std::vector<std::shared_ptr<BVariable>> m_variables;
    std::vector<std::shared_ptr<BScope>> m_scopes;
    std::shared_ptr<BScope> m_parentScope;
public:
    virtual ~BScope(){}

    /**
     * Get a string representation of the scope structure
     */
    std::stringstream getStructure();

    /**
     * Create a class that belong to this scope
     * @return pointer to the create class
     */
    std::shared_ptr<BScope> createClass();

    /**
     * Create a function that belong to this scope
     * @return pointer to the create function
     */
    std::shared_ptr<BScope> createFunction();

    /**
     * Create a variable that belong to this scope
     * @return pointer to the create variable
     */
    std::shared_ptr<BVariable> createVariable();

    /**
     * Find all variables
     * @param name Name of the variable | nullptr
     * @return if name is a nullptr, then return all the variables
     * in their order of insertion, otherwise return the variables
     * that matches the passed name
     */
    std::vector<std::shared_ptr<BVariable>> findAllVariables(char* name = nullptr);

    /**
     * Find all classes
     * @param name Name of the class | nullptr
     * @return if name is a nullptr, then return all the classes
     * in their order of insertion, otherwise return the classes
     * that matches the passed name
     */
    std::vector<std::shared_ptr<BScope>> findAllClasses(char* name = nullptr);

    /**
     * Find all functions
     * @param name Name of the function | nullptr
     * @return if name is a nullptr, then return all the functions
     * in their order of insertion, otherwise return the functions
     * that matches the passed name
     */
    std::vector<std::shared_ptr<BScope>> findAllFunctions(char* name = nullptr);

    /**
     * Get parent scope
     * @return pointer to parent scope
     */
    std::shared_ptr<BScope> getParentScope() const {return m_parentScope;}

    /**
     * Set parent scope
     * @param scope Parent scope
     */
    void setParentScope(std::shared_ptr<BScope> scope) { m_parentScope = scope;}
};

#endif
