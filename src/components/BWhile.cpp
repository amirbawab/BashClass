#include <bashclass/BWhile.h>

std::stringstream BWhile::getLabel() {
    std::stringstream stream = m_parentScope->getLabel();
    stream << "_for_";
    return stream;
}
