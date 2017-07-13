#ifndef BASHCLASS_BCLASS_H
#define BASHCLASS_BCLASS_H

#include <vector>
#include <memory>
#include <bashclass/BScope.h>

class BClass : public BScope {
private:

    // Class name
    std::shared_ptr<ecc::LexicalToken> m_name;

    // Parent class
    std::shared_ptr<BClass> m_extends;
public:
    /**
     * Get name lexical token
     * @return token pointer
     */
    std::shared_ptr<ecc::LexicalToken> getName() const { return m_name;}

    /**
     * Set name lexical token
     * @param name
     */
    void setName(std::shared_ptr<ecc::LexicalToken> name) {m_name = name;}

    /**
     * Get class label
     */
    std::stringstream getLabel();

    /**
     * The closest class to this scope is this scope itself
     * @return this scope
     */
    std::shared_ptr<BClass> findClosestClass();

    /**
     * Find all constructors
     * @return return the constructors
     * that matches the class name
     */
    std::vector<std::shared_ptr<BFunction>> findAllConstructors();

    /**
     * Set extends from class
     * @param class lexical token
     */
    void setExtends(std::shared_ptr<ecc::LexicalToken> lexicalToken);

    /**
     * Get extends from class
     * @return extends class
     */
    std::shared_ptr<BClass> getExtends(){ return m_extends;}

    /**
     * Check if class derives from the provided class
     * @param cls
     * @return true if class derives from the provided class
     */
    bool inheritsFrom(std::shared_ptr<BClass> cls);

    /**
     * Find closest variable in current, ancestor or extended scopes
     * @param name Name of the variable
     * @return closest variable pointer | nullptr if not found
     */
    std::shared_ptr<BVariable> findClosestVariable(std::string name);

    /**
     * Find all functions extended
     * @param name Name of the function | nullptr
     * @return if name is a nullptr, then return all the functions
     * in their order of insertion and from closest to farthest scope, otherwise return the functions
     * that matches the passed name
     */
    std::vector<std::shared_ptr<BFunction>> findAllFunctionsExtended(const char* name = nullptr);

    /**
     * Check if constructor is created
     */
    void verifyConstructor();
};

#endif
