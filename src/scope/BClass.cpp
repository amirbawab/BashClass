#include <bashclass/BClass.h>
#include <bashclass/BGenerateCode.h>
#include <bashclass/BException.h>
#include <bashclass/BFunction.h>

std::stringstream BClass::getLabel() {
    std::stringstream stream = m_parentScope->getLabel();
    stream << "_c_" << m_name->getValue();
    return stream;
}

std::shared_ptr<BClass> BClass::findClosestClass() {
    return std::static_pointer_cast<BClass>(shared_from_this());
}

std::shared_ptr<BFunction> BClass::findClosestFunction() {
    if(!m_parentScope) {
        throw BException("Cannot find closest function for a class with an undefined parent scope");
    }
    return m_parentScope->findClosestFunction();
}

std::vector<std::shared_ptr<BFunction>> BClass::findAllConstructors() {
    std::vector<std::shared_ptr<BFunction>> functions;
    for(auto scope : m_scopes) {
        std::shared_ptr<BFunction> functionScope = std::dynamic_pointer_cast<BFunction>(scope.second);
        if(functionScope && functionScope->isConstructor()) {
            if(!functionScope->getName()) {
                throw BException("Finding constructors requires constructor name to be defined");
            }

            if(!m_name) {
                throw BException("Class name is required to search for constructors");
            }

            if (functionScope->getName()->getValue() == m_name->getValue()) {
                functions.push_back(functionScope);
            }
        }
    }
    return functions;
}