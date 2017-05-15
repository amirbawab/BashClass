#ifndef BASHCLASS_BSCOPE_H
#define BASHCLASS_BSCOPE_H

#include <map>
#include <vector>
#include <memory>
#include <sstream>
#include <bashclass/BVariable.h>
#include <easycc/LexicalToken.h>

class BVariable;
class BScope : public std::enable_shared_from_this<BScope> {
protected:
    std::map<unsigned int,std::shared_ptr<BVariable>> m_variables;
    std::map<unsigned int,std::shared_ptr<BScope>> m_scopes;
    std::shared_ptr<ecc::LexicalToken> m_lexicalToken;
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
    std::shared_ptr<BScope> createClass(std::shared_ptr<ecc::LexicalToken> lexicalToken);

    /**
     * Create a function that belong to this scope
     * @return pointer to the create function
     */
    std::shared_ptr<BScope> createFunction(std::shared_ptr<ecc::LexicalToken> lexicalToken);

    /**
     * Create a variable that belong to this scope
     * @return pointer to the create variable
     */
    std::shared_ptr<BVariable> createVariable(std::shared_ptr<ecc::LexicalToken> lexicalToken);

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
    std::vector<std::shared_ptr<BVariable>> findAllParameters(char* name = nullptr);

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
     * Set lexical token that defines this scope
     * @param lexicalToken
     */
    void setLexicalToken(std::shared_ptr<ecc::LexicalToken> lexicalToken) {m_lexicalToken=lexicalToken;}

    /**
     * Get lexical token
     * @return lexical token pointer
     */
    std::shared_ptr<ecc::LexicalToken> getLexicalToken() const {return m_lexicalToken;}

    /**
     * Get scope by lexical token
     * @param lexicalToken
     * @return pointer to scope | nullptr
     */
    std::shared_ptr<BScope> getScope(std::shared_ptr<ecc::LexicalToken> lexicalToken);

    /**
     * Get variable by lexical token
     * @param lexicalToken
     * @return pointer to variable | nullptr
     */
    std::shared_ptr<BVariable> getVariable(std::shared_ptr<ecc::LexicalToken> lexicalToken);

    /**
     * Get label for that scope
     */
    virtual std::stringstream getLabel()=0;
};

#endif
