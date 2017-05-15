#include <bashclass/BWhile.h>

std::stringstream BWhile::getLabel() {
    std::stringstream stream = m_parentScope->getLabel();
    stream << "_while_";
    return stream;
}
