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

    // Start registering semantic actions handlers
    easyCC.registerSemanticAction("#start#", bashClass.m_start);
    easyCC.registerSemanticAction("#end#", bashClass.m_end);

    easyCC.registerSemanticAction("#startClass#", bashClass.m_startClass);
    easyCC.registerSemanticAction("#className#", bashClass.m_className);
    easyCC.registerSemanticAction("#endClass#", bashClass.m_endClass);

    easyCC.registerSemanticAction("#startFunction#", bashClass.m_startFunction);
    easyCC.registerSemanticAction("#functionType#", bashClass.m_functionType);
    easyCC.registerSemanticAction("#functionName#", bashClass.m_functionName);
    easyCC.registerSemanticAction("#endFunction#", bashClass.m_endFunction);

    easyCC.registerSemanticAction("#startVar#", bashClass.m_startVar);
    easyCC.registerSemanticAction("#varType#", bashClass.m_varType);
    easyCC.registerSemanticAction("#varName#", bashClass.m_varName);
    easyCC.registerSemanticAction("#endVar#", bashClass.m_endVar);

    easyCC.registerSemanticAction("#startParam#", bashClass.m_startParam);
    easyCC.registerSemanticAction("#paramType#", bashClass.m_paramType);
    easyCC.registerSemanticAction("#paramName#", bashClass.m_paramName);
    easyCC.registerSemanticAction("#endParam#", bashClass.m_endParam);

    easyCC.registerSemanticAction("#startWhile#", bashClass.m_startWhile);
    easyCC.registerSemanticAction("#endWhile#", bashClass.m_endWhile);

    easyCC.registerSemanticAction("#startIf#", bashClass.m_startIf);
    easyCC.registerSemanticAction("#endIf#", bashClass.m_endIf);

    easyCC.registerSemanticAction("#startOuterCall#", bashClass.m_startOuterCall);
    easyCC.registerSemanticAction("#endOuterCall#", bashClass.m_endOuterCall);

    easyCC.registerSemanticAction("#startInnerCall#", bashClass.m_startInnerCall);
    easyCC.registerSemanticAction("#endInnerCall#", bashClass.m_endInnerCall);

    easyCC.registerSemanticAction("#varCall#", bashClass.m_varCall);
    easyCC.registerSemanticAction("#functionCall#", bashClass.m_functionCall);
    easyCC.registerSemanticAction("#tokenCall#", bashClass.m_tokenCall);

    easyCC.registerSemanticAction("#startExpr#", bashClass.m_startExpr);
    easyCC.registerSemanticAction("#endExpr#", bashClass.m_endExpr);

    // Start compiling
    std::vector<int> phases = {BashClass::PHASE_CREATE, BashClass::PHASE_EVAL, BashClass::PHASE_GENERATE};
    for(int phase : phases) {

        // Set the phase number
        easyCC.setParsingPhase(phase);

        // Show error message on create phase only
        easyCC.setSilentSyntaxErrorMessages(phase != BashClass::PHASE_CREATE);

        // Compile all files passed as arguments
        for(std::string fileName : easyCC.getInputFilesNames()) {
            code = easyCC.compile(fileName);
            if(code != ecc::EasyCC::OK_CODE) {
                return code;
            }
        }
    }

    // Print structure of the compiled files
    bashClass.printStructure();
}