#include <bashclass/BIf.h>

std::stringstream BIf::getLabel() {
    std::stringstream stream = m_parentScope->getLabel();
    stream << "_if_";
    return stream;
}
