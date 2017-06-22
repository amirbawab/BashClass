#ifndef BASHCLASS_BTYPEFACTORY_H
#define BASHCLASS_BTYPEFACTORY_H

#include <memory>
#include <bashclass/BClass.h>

class BTypeFactory {
public:

    /**
     * Create expression type
     * @param cls
     * @return expression type
     */
    static std::shared_ptr<IBType> createClassExpressionType(std::shared_ptr<BClass> cls);

    /**
     * Create int expression type
     * @return expression type
     */
    static std::shared_ptr<IBType> createIntExpressionType();

    /**
     * Create string expression type
     * @return expression type
     */
    static std::shared_ptr<IBType> createStringExpressionType();

    /**
     * Create char expression type
     * @return expression type
     */
    static std::shared_ptr<IBType> createCharExpressionType();

    /**
     * Create boolean expression type
     * @return expression type
     */
    static std::shared_ptr<IBType> createBooleanExpressionType();

    /**
     * Create null expression type
     * @return expression type
     */
    static std::shared_ptr<IBType> createNullExpressionType();

    /**
     * Create undefined expression type
     * @return expression type
     */
    static std::shared_ptr<IBType> createUndefinedExpressionType();
};

#endif
