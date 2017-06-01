#include <bashclass/BException.h>

BException::BException(std::string message) : std::runtime_error(message + " -- Please report this error") {}