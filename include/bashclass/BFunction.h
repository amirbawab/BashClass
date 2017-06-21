#ifndef BASHCLASS_BFUNCTION_H
#define BASHCLASS_BFUNCTION_H

#include <vector>
#include <memory>
#include <bashclass/BClass.h>
#include <bashclass/BElementType.h>

class BFunction : public BScope {
private:

    // Function name
    std::shared_ptr<ecc::LexicalToken> m_name;

    // Function type
    std::shared_ptr<BElementType> m_type;

    // Mark function as a constructor
    bool m_isConstructor;

public:

    /**
     * Initial variables
     */
    BFunction();

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
     * Get function type
     * @return type
     */
    std::shared_ptr<BElementType> getType() const {return m_type; }

    /**
     * Check if this function is a class member
     */
    bool isClassMember();

    /**
     * Get function label
     */
    std::stringstream getLabel();

    /**
     * Check if function requires a return expression
     * @return true if function requires a return expression
     */
    bool requiresReturn();

    /**
     * A void function should not a return statement
     */
    void verifyReturns();

    /**
     * The closest function to this scope is the scope itself
     * @return this scope
     */
    std::shared_ptr<BFunction> findClosestFunction();

    /**
     * Find closest class by asking the parent scope
     * @return class scope or nullptr if not found
     */
    std::shared_ptr<BClass> findClosestClass();

    /**
     * Set if the function is a constructor
     * @param isConstructor
     */
    void setIsConstructor(bool isConstructor) { m_isConstructor = isConstructor;}

    /**
     * Check if a function is a constructor
     * @return true if the function is a constructor
     */
    bool isConstructor();
};

#endif
