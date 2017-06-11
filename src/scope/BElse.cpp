#include <bashclass/BElse.h>
#include <bashclass/BException.h>

std::stringstream BElse::getLabel() {
    std::stringstream stream = m_parentScope->getLabel();
    stream << "_else_";
    return stream;
}

std::shared_ptr<BFunction> BElse::findClosestFunction() {
    if(!m_parentScope) {
        throw BException("Cannot find closest function for an else statement with an undefined parent scope");
    }
    return m_parentScope->findClosestFunction();
}

std::shared_ptr<BClass> BElse::findClosestClass() {
    if(!m_parentScope) {
        throw BException("Cannot find closest class for an else statement with an undefined parent scope");
    }
    return m_parentScope->findClosestClass();
}
