#include <bashclass/BClass.h>
#include <bashclass/BGenerateCode.h>
#include <bashclass/BException.h>
#include <bashclass/BFunction.h>
#include <bashclass/BReport.h>
#include <bashclass/BGlobal.h>

std::stringstream BClass::getLabel() {
    std::stringstream stream = m_parentScope->getLabel();
    stream << "_c_" << m_name->getValue();
    return stream;
}

std::shared_ptr<BClass> BClass::findClosestClass() {
    return std::static_pointer_cast<BClass>(shared_from_this());
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

void BClass::setExtends(std::shared_ptr<ecc::LexicalToken> lexicalToken) {

    // Search for that class
    auto classes = BGlobal::getInstance()->findAllClasses(lexicalToken->getValue().c_str());

    // TODO To be tested
    auto thisClass = std::static_pointer_cast<BClass>(shared_from_this());

    // Check if class was found
    if (classes.empty()) {
        BReport::getInstance().error()
                << "Class " << lexicalToken->getValue()
                << " not found while establishing an inheritance relationship with "
                << m_name->getValue() << std::endl;
        BReport::getInstance().printError();
    } else {

        // Check if a circular inheritance will be created
        if (classes.front()->inheritsFrom(std::static_pointer_cast<BClass>(shared_from_this()))) {
            BReport::getInstance().error()
                    << "A circular inheritance detected while establishing an inheritance relationship between classes "
                    << m_name->getValue() << " and " << classes.front()->getName()->getValue() << std::endl;
            BReport::getInstance().printError();
        }

        // Extend anw
        m_extends = classes.front();
    }
}

bool BClass::inheritsFrom(std::shared_ptr<BClass> cls) {
    // TODO To be tested
    std::shared_ptr<BClass> tmpParent = std::static_pointer_cast<BClass>(shared_from_this());
    while(tmpParent && tmpParent != cls) {
        tmpParent = tmpParent->getExtends();
    }
    return tmpParent != nullptr;
}

std::shared_ptr<BVariable> BClass::findClosestVariable(std::string name) {

    // Call the original function
    auto variable = BScope::findClosestVariable(name);
    if(variable) {
        return variable;
    }

    // Check in extended classes
    if(m_extends) {
        return m_extends->findClosestVariable(name);
    }

    // Variable not found
    return nullptr;
}