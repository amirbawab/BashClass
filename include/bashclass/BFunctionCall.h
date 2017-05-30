#ifndef BASHCLASS_BFUNCTIONCALL_H
#define BASHCLASS_BFUNCTIONCALL_H

#include <bashclass/BFunction.h>
#include <bashclass/IBCallable.h>
#include <memory>
#include <vector>

class BFunctionCall : public IBCallable {
private:
    std::shared_ptr<BFunction> m_function;
    std::vector<std::shared_ptr<IBCallable>> m_arguments;
public:

    /**
     * Get function
     * @return function
     */
    std::shared_ptr<BFunction> getFunction() {return m_function;}

    /**
     * Set function
     * @param function
     */
    void setFunction(std::shared_ptr<BFunction> function) {m_function = function;}

    /**
     * Add argument
     * @param argument
     */
    void addArgument(std::shared_ptr<IBCallable> argument) {m_arguments.push_back(argument);};

    /**
     * Get function type
     * @return function type
     */
    std::string getTypeValueAsString();
};

#endif
