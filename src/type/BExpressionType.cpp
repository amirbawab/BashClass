#include <bashclass/BExpressionType.h>
#include <bashclass/BElementType.h>

void BExpressionType::cast(std::shared_ptr<BElementType> type) {
    IBType::cast(type);
    m_typeName = type->getTypeName();
    m_typeValue = type->getTypeValue();
}