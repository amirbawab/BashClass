#include <easycc/EasyCCDev.h>
#include <iostream>
#include <bashclass/BashClass.h>
#include <bashclass/BReport.h>

int main(int argc, char *argv[]) {

    // Create easycc developer mode
    std::shared_ptr<ecc::EasyCC> easyccdev = std::make_shared<ecc::EasyCC>();

    // Create an easycc developer mode instance
    int code;
    code = easyccdev->init(argc, argv);
    if(code != ecc::EasyCC::OK_CODE) {
        return code;
    }

    // Create a bashclass
    BashClass bashClass;

    // Set easycc in bashclass
    bashClass.setEasyCC(easyccdev);

    // Initialize semantic action handlers
    bashClass.initHandlers();

    // Start compiling
    std::vector<int> phases = {BashClass::PHASE_CREATE, BashClass::PHASE_EVAL, BashClass::PHASE_GENERATE};
    for(int phase : phases) {

        // Generate code only if no semantic errors were reported
        if(phase == BashClass::PHASE_GENERATE) {
            if(BReport::getInstance().hasError()) {
                return BashClass::ERR_CODE_SEMANTIC;
            }

            // Print structure of the compiled files
            //bashClass.printStructure();
        }

        // Set the phase number
        easyccdev->setParsingPhase(phase);

        // Show error message on create phase only
        easyccdev->setSilentSyntaxErrorMessages(phase != BashClass::PHASE_CREATE);

        // Compile all files passed as arguments
        for(std::string fileName : easyccdev->getInputFilesNames()) {
            code = easyccdev->compile(fileName);
            if(code != ecc::EasyCC::OK_CODE) {
                return code;
            }
        }
    }
}