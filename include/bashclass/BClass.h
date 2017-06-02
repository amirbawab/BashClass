#ifndef BASHCLASS_BCLASS_H
#define BASHCLASS_BCLASS_H

#include <vector>
#include <memory>
#include <bashclass/BScope.h>

class BClass : public BScope {
private:

    // Class name
    std::shared_ptr<ecc::LexicalToken> m_name;
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
};

#endif
