#include <bashclass/BReport.h>
#include <iostream>
#include <boost/filesystem/path.hpp>

void BReport::printError() {
    std::cerr << m_errorStream.str();
    m_errorStream.str(std::string());
    m_errorStream.clear();
    m_hasError = true;
}

std::stringstream& BReport::error() {
    m_errorStream << boost::filesystem::path(m_fileName).filename().c_str() << ": ";
    return m_errorStream;
}

BReport::~BReport() {

    // Get the size of the error stream
    m_errorStream.seekg(0, std::ios::end);
    long size = m_errorStream.tellg();
    m_errorStream.seekg(0, std::ios::beg);

    // Don't terminate before flushing all error messages
    if(size != 0) {
        printError();
    }
}