#include <bashclass/IBChainable.h>
#include <bashclass/BReport.h>

void IBChainable::addIndex(std::shared_ptr<IBExpression> expression) {

    // Add expression
    m_indices.push_back(expression);

    // Update type dimension
    if(!getType()->isUndefined()) {
        getType()->setDimension(getType()->getDimension()-1);
    }

    if(!isFound()) {
        BReport::getInstance().error()
                << "Index expression " << m_indices.size() << " cannot be accessed for element "
                << m_lexicalToken->getValue()
                << " at line " << m_lexicalToken->getLine()
                << " and column " << m_lexicalToken->getColumn()
                << " because the element is not found"
                << std::endl;
        BReport::getInstance().printError();

    } else if(getType()->isUndefined()) {
        BReport::getInstance().error()
                << "Index expression " << m_indices.size() << " cannot be verified for element "
                << m_lexicalToken->getValue()
                << " at line " << m_lexicalToken->getLine()
                << " and column " << m_lexicalToken->getColumn()
                << " because the element is of an undefined type"
                << std::endl;
        BReport::getInstance().printError();
    }

    // Check if the expression is of type integer
    if(!expression->getType()->isInt()) {
        BReport::getInstance().error()
                << "Index expression " << m_indices.size()
                << " must evaluate to an integer for element " << m_lexicalToken->getValue()
                << " at line " << m_lexicalToken->getLine()
                << " and column " << m_lexicalToken->getColumn()
                << std::endl;
        BReport::getInstance().printError();
    }
}
