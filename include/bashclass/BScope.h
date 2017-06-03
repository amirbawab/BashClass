#ifndef BASHCLASS_BSCOPE_H
#define BASHCLASS_BSCOPE_H

#include <map>
#include <vector>
#include <memory>
#include <sstream>
#include <bashclass/BVariable.h>
#include <easycc/LexicalToken.h>

class BVariable;
class BChainCall;
class BReturn;
class BFunction;
class BClass;
class BScope : public std::enable_shared_from_this<BScope> {
protected:

    // Register the variables defined in this scope
    std::map<unsigned int,std::shared_ptr<BVariable>> m_variables;

    // Register the scopes defines in this scope
    std::map<unsigned int,std::shared_ptr<BScope>> m_scopes;

    // Register the chain calls defined in this scope
    std::map<unsigned int,std::shared_ptr<BChainCall>> m_chainCalls;

    // Register the return statements defined in this scope
    std::shared_ptr<BReturn> m_return;

    // Store the parent for this scope
    std::shared_ptr<BScope> m_parentScope;
public:
    virtual ~BScope(){}

    /**
     * Get a string representation of the scope structure
     */
    std::stringstream getStructure();

    /**
     * Find all variables
     * @param name Name of the variable | nullptr
     * @return if name is a nullptr, then return all the variables
     * in their order of insertion, otherwise return the variables
     * that matches the passed name
     */
    std::vector<std::shared_ptr<BVariable>> findAllVariables(const char* name = nullptr);

    /**
     * Find all parameter variables
     * @param name Name of the parameter variable | nullptr
     * @return if name is a nullptr, then return all the parameter variables
     * in their order of insertion, otherwise return the parameter variables
     * that matches the passed name
     */
    std::vector<std::shared_ptr<BVariable>> findAllParameters(const char* name = nullptr);

    /**
     * Find all classes
     * @param name Name of the class | nullptr
     * @return if name is a nullptr, then return all the classes
     * in their order of insertion, otherwise return the classes
     * that matches the passed name
     */
    std::vector<std::shared_ptr<BScope>> findAllClasses(const char* name = nullptr);

    /**
     * Find all functions
     * @param name Name of the function | nullptr
     * @return if name is a nullptr, then return all the functions
     * in their order of insertion, otherwise return the functions
     * that matches the passed name
     */
    std::vector<std::shared_ptr<BScope>> findAllFunctions(const char* name = nullptr);

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

    /**
     * Get scope by reference key
     * @param referenceKey
     * @return pointer to scope | nullptr
     */
    std::shared_ptr<BScope> getScopeByReferenceKey(unsigned int referenceKey);

    /**
     * Get all scopes
     * @return vector fo scopes
     */
    std::vector<std::shared_ptr<BScope>> getAllScopes();

    /**
     * Get variable by reference key
     * @param referenceKey
     * @return pointer to variable | nullptr
     */
    std::shared_ptr<BVariable> getVariableByReferenceKey(unsigned int referenceKey);

    /**
     * Get label for that scope
     */
    virtual std::stringstream getLabel()=0;

    /**
     * Find closest variable in current or ancestor scope
     * @param name Name of the variable
     * @return closest variable pointer | nullptr if not found
     */
    std::shared_ptr<BVariable> findClosestVariable(std::string name);

    /**
     * Find closest ancestor function
     * @return closest function pointer | nullptr if not found
     */
    std::shared_ptr<BFunction> findClosestFunction();

    /**
     * Find closest ancestor class
     * @return closest class pointer | nullptr if not found
     */
    std::shared_ptr<BClass> findClosestClass();

    /**
     * Register class
     * @param referenceKey
     * @param classScope
     */
    void registerClass(unsigned int referenceKey, std::shared_ptr<BScope> classScope);

    /**
     * Register function
     * @param referenceKey
     * @param functionScope
     */
    void registerFunction(unsigned int referenceKey, std::shared_ptr<BScope> functionScope);

    /**
     * Register scope
     * @param referenceKey
     * @param scope
     */
    void registerScope(unsigned int referenceKey, std::shared_ptr<BScope> scope);

    /**
     * Register variable
     * @param referenceKey
     * @param variable
     */
    void registerVariable(unsigned int referenceKey, std::shared_ptr<BVariable> variable);

    /**
     * Register a chain call
     * @param referenceKey
     * @param chainCall
     */
    void registerChainCall(unsigned int referenceKey, std::shared_ptr<BChainCall> chainCall);

    /**
     * Get chain call by reference key
     * @param referenceKey
     * @return chainCall
     */
    std::shared_ptr<BChainCall> getChainCallByReferenceKey(unsigned int referenceKey);

    /**
     * Set return statement
     * @param ret
     */
    void setReturn(std::shared_ptr<BReturn> ret);

    /**
     * Get return statement
     * @return return statement
     */
    std::shared_ptr<BReturn> getReturn() {return m_return;}
};

#endif
