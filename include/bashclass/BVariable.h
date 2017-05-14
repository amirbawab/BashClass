#ifndef BASHCLASS_BVARIABLE_H
#define BASHCLASS_BVARIABLE_H

#include <string>
#include <memory>
#include <easycc/LexicalToken.h>

class BScope;
class BVariable {
private:
    std::string m_type;
    std::string m_name;
    std::string m_value;
    bool m_isParam;
    std::shared_ptr<BScope> m_parentScope;
    std::shared_ptr<ecc::LexicalToken> m_lexicalToken;
public:
    BVariable();

    /**
     * Get variable name
     * @return name
     */
    std::string getName() const {return m_name;}

    /**
     * Set variable name
     * @param name
     */
    void setName(std::string name) {m_name = name;}

    /**
     * Get variable type
     * @return type
     */
    std::string getType() const {return m_type;}

    /**
     * Set variable type
     * @param type
     */
    void setType(std::string type) {m_type = type;}

    /**
     * Get variable value
     * @return value
     */
    std::string getValue() const {return m_value;}

    /**
     * Set variable value
     * @param value
     */
    void setValue(std::string value) {m_value = value;}

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
     * Set lexical token that defines this variable
     * @param lexicalToken
     */
    void setLexicalToken(std::shared_ptr<ecc::LexicalToken> lexicalToken) { m_lexicalToken = lexicalToken;}

    /**
     * Get lexical token
     * @return lexical token pointer
     */
    std::shared_ptr<ecc::LexicalToken> getLexicalToken() const {return m_lexicalToken;}
};

#endif
