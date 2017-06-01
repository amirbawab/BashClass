#ifndef BASHCLASS_BGENERATECODE_H
#define BASHCLASS_BGENERATECODE_H

#include <sstream>

class BGenerateCode {
private:

    // Mark destructor as private
    BGenerateCode(){}
public:

    /**
     * Get singleton instance
     * @return singleton instance
     */
    static BGenerateCode& get() {
        static BGenerateCode instance;
        return instance;
    }

    // Delete constructor and operator
    BGenerateCode(BGenerateCode const&) = delete;
    void operator=(BGenerateCode const&) = delete;

    /**
     * Code generated before parsing the input
     */
    void writePreCode();

    /**
     * Code generated after parsing the input
     */
    void writePostCode();

    /**
     * Write code
     * @param code
     */
    void write(std::stringstream code);
};

#endif
