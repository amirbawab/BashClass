#ifndef BASHCLASS_BVARIABLE_H
#define BASHCLASS_BVARIABLE_H

#include <string>
#include <sstream>
#include <memory>
#include <easycc/LexicalToken.h>
#include <bashclass/BClass.h>
#include <bashclass/BElementType.h>

class BVariable {
private:

    // Name of this variable
    std::shared_ptr<ecc::LexicalToken> m_name;

    // Type of this variable
    std::shared_ptr<BElementType> m_type;

    // Parent scope of this variable
    std::shared_ptr<BScope> m_parentScope;

    // Initialize expression set when declaring the variable
    std::shared_ptr<IBExpression> m_expression;

    // A parameter variable has this flag set to true
    bool m_isParam;

public:

    /**
     * Initialize member variables
     */
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
     * Get variable type
     * @return type
     */
    std::shared_ptr<BElementType> getType() const {return m_type;}

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
     * Get variable label
     */
    std::stringstream getLabel();

    /**
     * Check if variable is a class member
     */
    bool isClassMember();

    /**
     * Set expression
     * @param expression
     */
    void setExpression(std::shared_ptr<IBExpression> expression) { m_expression = expression;}

    /**
     * Get expression
     * @return expression
     */
    std::shared_ptr<IBExpression> getExpression() {return m_expression;}

    /**
     * Get the default value
     * @return default value
     */
    std::string getDefaultValue();
};

#endif
