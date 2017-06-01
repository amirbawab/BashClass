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
public:

    // Delete constructor and operator
    BReport(BReport const&) = delete;
    void operator=(BReport const&) = delete;

    /**
     * Get singleton instance
     * @return singleton instance
     */
    static BReport& getInstance() {
        static BReport instance;
        return instance;
    }

    /**
     * Print error message
     */
    void printError();

    /**
     * Store the error message
     * @return
     */
    std::stringstream& error() {return m_errorStream;}
};

#endif
