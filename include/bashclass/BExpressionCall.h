#ifndef BASHCLASS_BEXPRESSIONCALL_H
#define BASHCLASS_BEXPRESSIONCALL_H

#include <bashclass/IBCallable.h>
#include <easycc/LexicalToken.h>
#include <memory>

class BExpressionCall : public IBCallable {
private:
    std::shared_ptr<IBCallable> leftOperand;
    std::shared_ptr<IBCallable> rightOperand;
    std::shared_ptr<ecc::LexicalToken> operatorToken;
};

#endif
