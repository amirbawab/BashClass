#ifndef BASHCLASS_BREPORT_H
#define BASHCLASS_BREPORT_H

#include <sstream>

class BReport {
private:

    // Hold messages
    std::stringstream m_errorStream;

    // Any error changes the value of this flag to true
    bool m_hasError;

    // Make constructor private
    BReport() : m_hasError(false){}

    // Hold temporary file name
    // This file name correspond to the one currently
    // being processed by the semantic analyzer
    std::string m_fileName;
public:

    // Delete constructor and operator
    BReport(BReport const&) = delete;
    void operator=(BReport const&) = delete;

    /**
     * Flush buffer
     */
    ~BReport();

    /**
     * Get singleton instance
     * @return singleton instance
     */
    static BReport& getInstance() {
        static BReport instance;
        return instance;
    }

    /**
     * Set file name currently being processed
     * @param fileName
     */
    void setFileName(std::string fileName) { m_fileName = fileName; }

    /**
     * Print error message
     */
    void printError();

    /**
     * Store the error message
     * @return
     */
    std::stringstream& error();

    /**
     * Check if an error was reported
     * @return true if an error was reported
     */
    bool hasError() {return m_hasError;}
};

#endif
