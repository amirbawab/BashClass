#ifndef BASHCLASS_BFUNCTION_H
#define BASHCLASS_BFUNCTION_H

#include <vector>
#include <memory>
#include <bashclass/BScope.h>
#include <bashclass/IBCallable.h>

class BFunction : public BScope, public IBCallable {
private:
    std::shared_ptr<ecc::LexicalToken> m_name;
    std::shared_ptr<ecc::LexicalToken> m_type;
    std::shared_ptr<BScope> m_typeScope;
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
     * Get function label
     */
    std::stringstream getLabel();
};

#endif
