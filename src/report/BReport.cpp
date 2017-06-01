#include <bashclass/BReport.h>
#include <iostream>

void BReport::printError() {
    std::cerr << m_errorStream.str();
    m_hasError = true;
}