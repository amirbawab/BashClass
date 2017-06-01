#ifndef BASHCLASS_BFUNCTION_H
#define BASHCLASS_BFUNCTION_H

#include <vector>
#include <memory>
#include <bashclass/BScope.h>
#include <bashclass/IBCallable.h>
#include <bashclass/BReturn.h>

class BFunction : public BScope {
private:

    // Function name
    std::shared_ptr<ecc::LexicalToken> m_name;

    // Function type
    std::shared_ptr<ecc::LexicalToken> m_type;

    // Link to the actual function if exists.
    // Check hasKnownType() function for more details
    std::shared_ptr<BScope> m_typeScope;

    // Store the return statement for this function
    std::map<unsigned int,std::shared_ptr<BReturn>> m_returns;

public:

    /**
     * Get function name token
     * @return name
     */
    std::shared_ptr<ecc::LexicalToken> getName() const{return m_name;}

    /**
     * Set function name token
     * @param name
     */
    void setName(std::shared_ptr<ecc::LexicalToken> name) { m_name = name;}

    /**
     * Get function type token
     * @return type
     */
    std::shared_ptr<ecc::LexicalToken> getType() const {return m_type; }

    /**
     * Set function type token
     * @param type
     */
    void setType(std::shared_ptr<ecc::LexicalToken> type) { m_type = type;}

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
     * Check if this function is a class member
     */
    bool isClassMember();

    /**
     * Get function label
     */
    std::stringstream getLabel();

    /**
     * A type is known if:
     * - The type is built-in (e.g. int, char, ...)
     * - The typeScope links to the actual function
     */
    bool hasKnowType() const;

    /**
     * A function member of a class must have the class as the type of the first argument
     */
    void verifyParameters();

    /**
     * Check if function requires a return expression
     * @return true if function requires a return expression
     */
    bool requiresReturn();

    /**
     * Check if the function has a return statement
     * @return true if the function has a return statement
     */
    bool hasReturn();

    /**
     * Register return statement
     * @param token
     * @param ret
     */
    void registerReturn(std::shared_ptr<ecc::LexicalToken> token, std::shared_ptr<BReturn> ret);

    /**
     * Get registered return expression
     * @param lexicalToken
     * @return return
     */
    std::shared_ptr<BReturn> getReturnByToken(std::shared_ptr<ecc::LexicalToken> lexicalToken);

    /**
     * A void function should not a return statement
     */
    void verifyReturns();
};

#endif
