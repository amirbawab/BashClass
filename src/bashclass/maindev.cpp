#include <easycc/EasyCCDev.h>
#include <iostream>

typedef std::vector<std::shared_ptr<ecc::LexicalToken>> Tokens;

int main(int argc, char *argv[]) {
    ecc::EasyCC easyCC;
    int code;
    code = easyCC.init(argc, argv);
    if(code != ecc::EasyCC::OK_CODE) {
        return code;
    }

    easyCC.setParsingPhase(0);

    for(std::string fileName : easyCC.getInputFilesNames()) {
        code = easyCC.compile(fileName);
        if(code != ecc::EasyCC::OK_CODE) {
            return code;
        }
    }
}