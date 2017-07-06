#include <easycc/EasyCCPro.h>
#include <iostream>
#include <bashclass/BashClass.h>
#include <bashclass/BReport.h>
#include <getopt.h>
#include <fstream>
#include <bashclass/BGenerateCode.h>

// Output file
std::string outputFile;

// Input files
std::vector<std::string> inputFiles;

/**
 * Print help message
 */
void printUsage() {
    std::cout
            << "bashc - Compiler converting BashClass syntax into bash 4.3+" << std::endl
            << "Usage: bashc [OPTION]... [FILE]..." << std::endl
            << "\t-o, --output\t\t\tOutput file" << std::endl
            << "\t-h, --help\t\t\tDisplay this help message" << std::endl;
}

/**
 * Initialize parameter
 * @param argc
 * @param argv
 */
void initParams(int argc, char *argv[]) {

    struct option longOptions[] = {
            {"output", required_argument, 0, 'o'},
            {"help",   no_argument,       0, 'h'},
            {0, 0,                        0, 0}
    };

    int optionIndex = 0;
    int c;
    while ((c = getopt_long(argc, argv, "ho:", longOptions, &optionIndex)) != -1) {
        switch (c) {
            case 'o':
                outputFile = optarg;
                break;
            case 'h':
            default:
                // Print by default
                break;
        }
    }
}

int main(int argc, char *argv[]) {

    // Fetch files names
    for(int i = optind; i < argc; ++i) {
        std::string fileName = argv[i];
        if(!fileName.empty()) {

            // Check if it's a file containing a list of files names
            if(fileName.front() == '@') {

                // Open file starting from the second character
                std::ifstream input(fileName.substr(1));

                // Add all lines in that file
                for( std::string line; getline( input, line ); ) {
                    inputFiles.push_back(line);
                }

                // Close file
                input.close();
            } else {
                inputFiles.push_back(fileName);
            }
        }
    }

    // Initialize parameters
    initParams(argc, argv);

    // Check missing requirements
    if(outputFile.empty() || inputFiles.empty()) {
        printUsage();
        return ecc::EasyCC::ERR_CODE_PARAMS;
    }

    // Set output file
    BGenerateCode::get().openFile(outputFile);

    // Create easycc production mode
    std::shared_ptr<ecc::EasyCC> easyccpro = std::make_shared<ecc::EasyCC>();

    // Create a bashclass
    BashClass bashClass;

    // Set easycc in bashclass
    bashClass.setEasyCC(easyccpro);

    // Initialize semantic action handlers
    bashClass.initHandlers();

    // Configure easycc on syntax error
    easyccpro->setOnSyntaxError([&](){
        easyccpro->setSilentSemanticEvents(true);
    });

    // Define the compiler phases
    std::vector<int> phases = {
            BashClass::PHASE_CREATE,
            BashClass::PHASE_EVAL,
            BashClass::PHASE_GENERATE
    };

    // Start compiling
    for(int phase : phases) {

        // Don't generate code if semantic errors were reported
        if(phase == BashClass::PHASE_GENERATE) {
            if(BReport::getInstance().hasError()) {
                return BashClass::ERR_CODE_SEMANTIC;
            }
        }

        // Set the phase number
        easyccpro->setParsingPhase(phase);

        // Show error message on create phase only
        easyccpro->setSilentSyntaxErrorMessages(phase != BashClass::PHASE_CREATE);

        // Compile all files passed as arguments
        for(std::string fileName : inputFiles) {
            int code = easyccpro->compile(fileName);

            // Store compiling if a file has syntax errors
            if(code != ecc::EasyCC::OK_CODE) {
                return code;
            }
        }
    }
    return 0;
}