#ifndef BASHCLASS_BARRAYUSE_H
#define BASHCLASS_BARRAYUSE_H

#include <bashclass/IBExpression.h>
#include <bashclass/BElementType.h>
#include <easycc/LexicalToken.h>

class BArrayUse : public IBExpression {
private:

    // Hold the type of this array
    std::shared_ptr<BElementType> m_type;

public:

    /**
     * Initialize members
     */
    BArrayUse();

    /**
     * Get type
     * @return type
     */
    std::shared_ptr<IBType> getType() {return m_type;}

    /**
     * Set type lexical token
     */
    void setTypeLexicalToken(std::shared_ptr<ecc::LexicalToken> lexicalToken);

    /**
     * Cast type
     * @param type
     */
    void castType(std::shared_ptr<BElementType> type);
};

#endif
