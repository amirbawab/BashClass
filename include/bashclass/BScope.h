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
class BScope : public std::enable_shared_from_this<BScope> {
protected:
    std::map<unsigned int,std::shared_ptr<BVariable>> m_variables;
    std::map<unsigned int,std::shared_ptr<BScope>> m_scopes;
    std::map<unsigned int,std::shared_ptr<BChainCall>> m_chainCalls;
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
     * Get scope by lexical token
     * @param lexicalToken
     * @return pointer to scope | nullptr
     */
    std::shared_ptr<BScope> getScopeByToken(std::shared_ptr<ecc::LexicalToken> lexicalToken);

    /**
     * Get all scopes
     * @return vector fo scopes
     */
    std::vector<std::shared_ptr<BScope>> getAllScopes();

    /**
     * Get variable by lexical token
     * @param lexicalToken
     * @return pointer to variable | nullptr
     */
    std::shared_ptr<BVariable> getVariableByToken(std::shared_ptr<ecc::LexicalToken> lexicalToken);

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
     * Register class
     * @param token
     * @param classScope
     */
    void registerClass(std::shared_ptr<ecc::LexicalToken> token, std::shared_ptr<BScope> classScope);

    /**
     * Register function
     * @param token
     * @param functionScope
     */
    void registerFunction(std::shared_ptr<ecc::LexicalToken> token, std::shared_ptr<BScope> functionScope);

    /**
     * Register scope
     * @param token
     * @param scope
     */
    void registerScope(std::shared_ptr<ecc::LexicalToken> token, std::shared_ptr<BScope> scope);

    /**
     * Register variable
     * @param token
     * @param variable
     */
    void registerVariable(std::shared_ptr<ecc::LexicalToken> token, std::shared_ptr<BVariable> variable);

    /**
     * Register a chain call
     * @param token
     * @param chainCall
     */
    void registerChainCall(std::shared_ptr<ecc::LexicalToken> token, std::shared_ptr<BChainCall> chainCall);

    /**
     * Get chain call by token
     * @param token
     * @return chainCall
     */
    std::shared_ptr<BChainCall> getChainCallByToken(std::shared_ptr<ecc::LexicalToken> token);
};

#endif
