#ifndef BASHCLASS_BEXCEPTION_H
#define BASHCLASS_BEXCEPTION_H

#include <stdexcept>
#include <string>

class BException : public std::runtime_error {
public:
    BException(std::string message);
};


#endif
