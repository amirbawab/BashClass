#include <bashclass/BVariableChainAccess.h>
#include <bashclass/BException.h>
#include <bashclass/BTypeFactory.h>
#include <bashclass/BReport.h>

bool BVariableChainAccess::isFound() {
    return m_variable != nullptr;
}

std::shared_ptr<IBType> BVariableChainAccess::getType() {
    if(!m_variable || !m_variable->getType()->hasKnownType()) {
        return BTypeFactory::createUndefinedExpressionType();
    }
    return m_variable->getType();
}

void BVariableChainAccess::addIndex(std::shared_ptr<IBExpression> expression) {

    // Add expression
    m_indices.push_back(expression);

    if(!isFound()) {
        BReport::getInstance().error()
                << "Index expression " << m_indices.size() << " cannot be accessed for variable "
                << m_lexicalToken->getValue()
                << " at line " << m_lexicalToken->getLine()
                << " and column " << m_lexicalToken->getColumn()
                << " because the variable is not found"
                << std::endl;
        BReport::getInstance().printError();

    } else if(getType()->isUndefined()) {
        BReport::getInstance().error()
                << "Index expression " << m_indices.size() << " cannot be verified for variable "
                << m_lexicalToken->getValue()
                << " at line " << m_lexicalToken->getLine()
                << " and column " << m_lexicalToken->getColumn()
                << " because the variable is of an undefined type"
                << std::endl;
        BReport::getInstance().printError();
    } else if(!getType()->isArray()) {
        BReport::getInstance().error()
                << "Index expression " << m_indices.size() << " cannot be accessed for variable "
                << m_lexicalToken->getValue()
                << " at line " << m_lexicalToken->getLine()
                << " and column " << m_lexicalToken->getColumn()
                << " because the variable is not an array"
                << std::endl;
    } else if(m_indices.size() > getType()->getDimension()) {
        BReport::getInstance().error()
                << "Index expression " << m_indices.size() << " cannot be accessed for variable "
                << m_lexicalToken->getValue()
                << " at line " << m_lexicalToken->getLine()
                << " and column " << m_lexicalToken->getColumn()
                << " because the variable is array of dimension " << getType()->getDimension()
                << std::endl;
    }

    // Check if the expression is of type integer
    if(!expression->getType()->isInt()) {
        BReport::getInstance().error()
                << "Index expression " << m_indices.size()
                << " must evaluate to an integer for variable " << m_lexicalToken->getValue()
                << " at line " << m_lexicalToken->getLine()
                << " and column " << m_lexicalToken->getColumn()
                << std::endl;
        BReport::getInstance().printError();
    }
}