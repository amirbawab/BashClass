#include <bashclass/BFunctionCall.h>
#include <bashclass/BTypes.h>

std::string BFunctionCall::getTypeValueAsString() {
    if(!m_function || !m_function->hasKnowType()) {
        return BType::UNDEFINED;
    }
    return m_function->getType()->getValue();
}
