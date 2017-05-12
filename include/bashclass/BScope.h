#ifndef BASHCLASS_BSCOPE_H
#define BASHCLASS_BSCOPE_H

#include <vector>
#include <memory>
#include <bashclass/BVariable.h>

class BScope {
protected:
    std::vector<std::shared_ptr<BVariable>> m_variables;
    std::vector<std::shared_ptr<BScope>> m_scopes;
public:
    virtual ~BScope(){}
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
};

#endif
