#ifndef BASHCLASS_BSCOPE_H
#define BASHCLASS_BSCOPE_H

#include <map>
#include <vector>
#include <memory>
#include <sstream>
#include <easycc/LexicalToken.h>
#include <bashclass/IBExpression.h>

class BVariable;
class BFunction;
class BClass;
class BWhile;
class BFor;
class BChain;
class BReturn;
class BScope : public std::enable_shared_from_this<BScope> {
protected:

    // Register the variables defined in this scope
    std::map<unsigned int,std::shared_ptr<BVariable>> m_variables;

    // Register the scopes defines in this scope
    std::map<unsigned int,std::shared_ptr<BScope>> m_scopes;

    // Register expressions
    std::map<unsigned int,std::shared_ptr<IBExpression>> m_expressions;

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
     * Find all classes
     * @param name Name of the class | nullptr
     * @return if name is a nullptr, then return all the classes
     * in their order of insertion, otherwise return the classes
     * that matches the passed name
     */
    std::vector<std::shared_ptr<BClass>> findAllClasses(const char* name = nullptr);

    /**
     * Find all functions
     * @param name Name of the function | nullptr
     * @return if name is a nullptr, then return all the functions
     * in their order of insertion, otherwise return the functions
     * that matches the passed name
     */
    std::vector<std::shared_ptr<BFunction>> findAllFunctions(const char* name = nullptr);

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
    virtual std::shared_ptr<BVariable> findClosestVariable(std::string name);

    /**
     * Find closest function: ancestor or itself
     * @return closest function pointer | nullptr if not found
     */
    virtual std::shared_ptr<BFunction> findClosestFunction();

    /**
     * Find closest class: ancestor or itself
     * @return closest class pointer | nullptr if not found
     */
    virtual std::shared_ptr<BClass> findClosestClass();

    /**
     * Find closest while: ancestor or itself
     * @return closest while pointer | nullptr if not found
     */
    virtual std::shared_ptr<BWhile> findClosestWhile();

    /**
     * Find closest for: ancestor or itself
     * @return closest for pointer | nullptr if not found
     */
    virtual std::shared_ptr<BFor> findClosestFor();

    /**
     * Register class
     * @param referenceKey
     * @param classScope
     */
    void registerClass(unsigned int referenceKey, std::shared_ptr<BClass> classScope);

    /**
     * Register function
     * @param referenceKey
     * @param functionScope
     */
    void registerFunction(unsigned int referenceKey, std::shared_ptr<BFunction> functionScope);

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
     * Register an expression
     * @param referenceKey
     * @param expression
     */
    void registerExpression(unsigned int referenceKey, std::shared_ptr<IBExpression> expression);

    /**
     * Get an expression
     * @param referenceKey
     * @return function call
     */
    std::shared_ptr<IBExpression> getExpressionByReferenceKey(unsigned int referenceKey);

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

    /**
     * Check if scope has a return statement
     * @return true if it has a return statement
     */
    bool hasReturn();

    /**
     * Check if can use break or continue in this scope
     */
    void canBreakOrContinue(std::shared_ptr<ecc::LexicalToken> lexicalToken);
};

#endif
