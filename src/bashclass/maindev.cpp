#include <easycc/EasyCCDev.h>
#include <iostream>
#include <bashclass/BashClass.h>
#include <bashclass/BGenerateCode.h>

int main(int argc, char *argv[]) {

    // Create easycc developer mode
    std::shared_ptr<ecc::EasyCC> easyccdev = std::make_shared<ecc::EasyCC>();

    // Create an easycc developer mode instance
    int code = easyccdev->init(argc, argv);
    if(code != ecc::EasyCC::OK_CODE) {
        return code;
    }

    // Create a bashclass
    BashClass bashClass(easyccdev);

    // Compile files
    return bashClass.compile(easyccdev->getInputFilesNames(), easyccdev->getOutputFileName());
}