#ifndef BASHCLASS_BVARIABLE_H
#define BASHCLASS_BVARIABLE_H

#include <string>
#include <sstream>
#include <memory>
#include <easycc/LexicalToken.h>
#include <bashclass/BScope.h>
#include <bashclass/IBCallable.h>

class BScope;
class BVariable : public IBCallable {
private:
    std::shared_ptr<ecc::LexicalToken> m_type;
    std::shared_ptr<BScope> m_typeScope;
    std::shared_ptr<ecc::LexicalToken> m_name;
    std::shared_ptr<BScope> m_parentScope;
    std::shared_ptr<ecc::LexicalToken> m_referenceToken;
    bool m_isParam;
    bool m_knownType = false;
public:
    BVariable();

    /**
     * Get variable name token
     * @return name
     */
    std::shared_ptr<ecc::LexicalToken> getName() const {return m_name;}

    /**
     * Set variable name token
     * @param name
     */
    void setName(std::shared_ptr<ecc::LexicalToken> name) {m_name = name;}

    /**
     * Get variable type token
     * @return type
     */
    std::shared_ptr<ecc::LexicalToken> getType() const {return m_type;}

    /**
     * Set variable type token
     * @param type
     */
    void setType(std::shared_ptr<ecc::LexicalToken> type) {m_type = type;}

    /**
     * Get variable type scope
     * @return type
     */
    std::shared_ptr<BScope> getTypeScope() const {return m_typeScope;}

    /**
     * Set variable type scope
     * @param type
     */
    void setTypeScope(std::shared_ptr<BScope> scope) {m_typeScope = scope;}

    /**
     * Get type value as a string
     * @return type string
     */
    std::string getTypeValue() const;

    /**
     * Get parent scope
     * @return pointer to parent scope
     */
    std::shared_ptr<BScope> getParentScope() {return m_parentScope;};

    /**
     * Set parent scope
     * @param parent scope pointer
     */
    void setParentScope(std::shared_ptr<BScope> scope) {m_parentScope = scope;}

    /**
     * Check if variable is a function parameter
     * @return true if it is a parameter
     */
    bool isParam() const {return m_isParam;}

    /**
     * Set parameter flag for a variable
     * @param isParam True if the variable is a function parameter
     */
    void setIsParam(bool isParam) {m_isParam = isParam;}

    /**
     * Set lexical token used a reference
     * @param pointer to the reference token
     */
    void setReferenceToken(std::shared_ptr<ecc::LexicalToken> lexicalToken) { m_referenceToken = lexicalToken;}

    /**
     * Get reference token
     * @return pointer to the reference token
     */
    std::shared_ptr<ecc::LexicalToken> getReferenceToken() {return m_referenceToken;}

    /**
     * Get variable label
     */
    std::stringstream getLabel();

    /**
     * Set if the variable has a known type
     */
    void setKnownType(bool knownType) {m_knownType = knownType;}

    /**
     * Check if variable is a class member
     */
    bool isClassMember();

    /**
     * Check if variable has a known type
     */
    bool hasKnownType() const {return m_knownType;}
};

#endif
