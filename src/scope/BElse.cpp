#include <bashclass/BElse.h>

std::stringstream BElse::getLabel() {
    std::stringstream stream = m_parentScope->getLabel();
    stream << "_else_";
    return stream;
}