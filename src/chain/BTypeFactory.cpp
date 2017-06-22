#include <bashclass/BTypeFactory.h>
#include <bashclass/BExpressionType.h>

// TODO Make the the returned pointers static, so each time the same instance is returned (except for custom ones)

std::shared_ptr<IBType> BTypeFactory::createClassExpressionType(std::shared_ptr<BClass> cls) {
    std::shared_ptr<BExpressionType> expressionType = std::make_shared<BExpressionType>();
    expressionType->setTypeName(cls->getName()->getName());
    expressionType->setTypeValue(cls->getName()->getValue());
    expressionType->setTypeScope(cls);
    return expressionType;
}

std::shared_ptr<IBType> BTypeFactory::createIntExpressionType() {
    std::shared_ptr<BExpressionType> expressionType = std::make_shared<BExpressionType>();
    expressionType->setTypeName(IBType::TYPE_NAME_INT);
    expressionType->setTypeValue(IBType::TYPE_VALUE_INT);
    return expressionType;
}

std::shared_ptr<IBType> BTypeFactory::createCharExpressionType() {
    std::shared_ptr<BExpressionType> expressionType = std::make_shared<BExpressionType>();
    expressionType->setTypeName(IBType::TYPE_NAME_CHAR);
    expressionType->setTypeValue(IBType::TYPE_VALUE_CHAR);
    return expressionType;
}

std::shared_ptr<IBType> BTypeFactory::createNullExpressionType() {
    std::shared_ptr<BExpressionType> expressionType = std::make_shared<BExpressionType>();
    expressionType->setTypeName(IBType::NULL_VALUE);
    expressionType->setTypeValue(IBType::NULL_VALUE);
    return expressionType;
}

std::shared_ptr<IBType> BTypeFactory::createStringExpressionType() {
    std::shared_ptr<BExpressionType> expressionType = std::make_shared<BExpressionType>();
    expressionType->setTypeName(IBType::TYPE_NAME_STRING);
    expressionType->setTypeValue(IBType::TYPE_VALUE_STRING);
    return expressionType;
}

std::shared_ptr<IBType> BTypeFactory::createBooleanExpressionType() {
    std::shared_ptr<BExpressionType> expressionType = std::make_shared<BExpressionType>();
    expressionType->setTypeName(IBType::TYPE_NAME_BOOLEAN);
    expressionType->setTypeValue(IBType::TYPE_VALUE_BOOLEAN);
    return expressionType;
}

std::shared_ptr<IBType> BTypeFactory::createUndefinedExpressionType() {
    std::shared_ptr<BExpressionType> expressionType = std::make_shared<BExpressionType>();
    expressionType->setTypeName(IBType::UNDEFINED);
    expressionType->setTypeValue(IBType::UNDEFINED);
    return expressionType;
}
