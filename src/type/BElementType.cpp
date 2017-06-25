#include <bashclass/BElementType.h>
#include <bashclass/BException.h>
#include <bashclass/BGlobal.h>
#include <bashclass/BReport.h>

std::string BElementType::getTypeValue() {
    if(!m_lexicalToken) {
        throw BException("Cannot get type value of lexical token that was not set");
    }
    return m_lexicalToken->getValue();
}

std::string BElementType::getTypeName() {
    if(!m_lexicalToken) {
        throw BException("Cannot get type name of lexical token that was not set");
    }
    return m_lexicalToken->getName();
}

void BElementType::linkType() {

    // Link type iff the type is an identifier
    if(isIdentifier()) {
        // Find class scope of that type
        auto cls = BGlobal::getInstance()->findAllClasses(m_lexicalToken->getValue().c_str());
        if(cls.empty()) {
            BReport::getInstance().error()
                    << "Undefined type " << m_lexicalToken->getValue()
                    << " at line " << m_lexicalToken->getLine()
                    << " and column " << m_lexicalToken->getColumn()
                    << std::endl;
            BReport::getInstance().printError();
        } else {
            m_typeScope = cls.front();
        }
    }
}

std::shared_ptr<BExpressionType> BElementType::cloneToExpressionType() {
    auto newType = std::make_shared<BExpressionType>();
    newType->setDimension(getDimension());
    newType->setTypeName(getTypeName());
    newType->setTypeValue(getTypeValue());
    newType->setTypeScope(getTypeScope());
    return newType;
}

void BElementType::cast(std::shared_ptr<BElementType> type) {
    IBType::cast(type);
    m_lexicalToken  = type->getLexicalToken();
}