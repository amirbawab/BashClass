#include <bashclass/BVariable.h>
#include <bashclass/BTypes.h>
#include <bashclass/BClass.h>
#include <bashclass/BException.h>
#include <bashclass/BGlobal.h>
#include <bashclass/BReport.h>

BVariable::BVariable() {
    m_isParam = false;
}

std::stringstream BVariable::getLabel() {
    std::stringstream stream = m_parentScope->getLabel();
    stream << (m_isParam ? "_p_" : "_v_") << m_name->getValue();
    return stream;
}

bool BVariable::isClassMember() {
    if(!m_parentScope) {
        throw BException("Cannot check if a variable is a class member if it does not have a parent scope");
    }
    return std::dynamic_pointer_cast<BClass>(m_parentScope) != nullptr;
}

bool BVariable::hasKnownType() const {
    return BType::isBuiltInType(m_type->getName()) || m_typeScope;
}

void BVariable::linkType() {
    if(!BType::isBuiltInType(m_type->getName())) {
        // Find class scope of that type
        auto cls = BGlobal::getInstance()->findAllClasses(m_type->getValue().c_str());
        if(cls.empty()) {
            BReport::getInstance().error()
                    << "Undefined type " << m_type->getValue() <<
                    " for variable " << m_name->getValue() << std::endl;
            BReport::getInstance().printError();
        } else {
            m_typeScope = cls.front();
        }
    }
}

std::string BVariable::getDefaultValue() {
    if(!m_type) {
        throw BException("Cannot get initial value without setting the variable type");
    }

    if(m_type->getName() == BType::TYPE_NAME_STRING) {
        return "\"\"";
    }

    if(m_type->getName() == BType::TYPE_NAME_CHAR) {
        return "\"$(echo -n -e \"\\u0000\")\"";
    }

    return "0";
}