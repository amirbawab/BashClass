#ifndef BASHCLASS_BELEMENTTYPE_H
#define BASHCLASS_BELEMENTTYPE_H

#include <bashclass/IBType.h>
#include <easycc/LexicalToken.h>

class BElementType : public IBType {
private:

    // Hold lexical token
    std::shared_ptr<ecc::LexicalToken> m_lexicalToken;
public:

    /**
     * Get type name
     * @return type name
     */
    std::string getTypeName();

    /**
     * Get type value
     * @return type value
     */
    std::string getTypeValue();
};
#endif
