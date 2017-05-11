#include <easycc/EasyCCDev.h>
#include <iostream>
#include <bashclass/BashClass.h>

int main(int argc, char *argv[]) {

    // Create an easycc developer mode instance
    ecc::EasyCC easyCC;
    int code;
    code = easyCC.init(argc, argv);
    if(code != ecc::EasyCC::OK_CODE) {
        return code;
    }

    // Create a bash class instance to hold the structure of the code
    BashClass bashClass;
    bashClass.initHandlers();

    // Start registering semantic actions handlers
    easyCC.registerSemanticAction("#startClass#", bashClass.m_startClass);

    const int MAX_PHASES = 2;
    for(int phase=1; phase <= MAX_PHASES; phase++) {

        // Set the phase number
        easyCC.setParsingPhase(phase);

        // Don't show error message on phase greater than 1
        easyCC.setSilentSyntaxErrorMessages(phase != 1);

        for(std::string fileName : easyCC.getInputFilesNames()) {
            code = easyCC.compile(fileName);
            if(code != ecc::EasyCC::OK_CODE) {
                return code;
            }
        }
    }
}