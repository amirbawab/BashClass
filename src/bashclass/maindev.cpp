#include <easycc/EasyCCDev.h>
#include <iostream>
#include <bashclass/BashClass.h>
#include <bashclass/BReport.h>

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
    easyCC.registerSemanticAction("#newKey#", bashClass.m_newKey);

    easyCC.registerSemanticAction("#startClass#", bashClass.m_startClass);
    easyCC.registerSemanticAction("#className#", bashClass.m_className);
    easyCC.registerSemanticAction("#endClass#", bashClass.m_endClass);

    easyCC.registerSemanticAction("#startFunction#", bashClass.m_startFunction);
    easyCC.registerSemanticAction("#functionConstructor#", bashClass.m_functionConstructor);
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
    easyCC.registerSemanticAction("#whileCond#", bashClass.m_whileCond);
    easyCC.registerSemanticAction("#endWhile#", bashClass.m_endWhile);

    easyCC.registerSemanticAction("#startIf#", bashClass.m_startIf);
    easyCC.registerSemanticAction("#ifCond#", bashClass.m_ifCond);
    easyCC.registerSemanticAction("#endIf#", bashClass.m_endIf);

    easyCC.registerSemanticAction("#startElif#", bashClass.m_startElif);
    easyCC.registerSemanticAction("#elifCond#", bashClass.m_elifCond);
    easyCC.registerSemanticAction("#endElif#", bashClass.m_endElif);

    easyCC.registerSemanticAction("#startElse#", bashClass.m_startElse);
    easyCC.registerSemanticAction("#endElse#", bashClass.m_endElse);

    easyCC.registerSemanticAction("#startOuterCall#", bashClass.m_startOuterCall);
    easyCC.registerSemanticAction("#endOuterCall#", bashClass.m_endOuterCall);
    easyCC.registerSemanticAction("#tokenUse#", bashClass.m_tokenUse);
    easyCC.registerSemanticAction("#thisAccess#", bashClass.m_thisAccess);

    easyCC.registerSemanticAction("#startInnerCall#", bashClass.m_startInnerCall);
    easyCC.registerSemanticAction("#endInnerCall#", bashClass.m_endInnerCall);
    easyCC.registerSemanticAction("#varChainAccess#", bashClass.m_varChainAccess);
    easyCC.registerSemanticAction("#functionChainCall#", bashClass.m_functionChainCall);
    easyCC.registerSemanticAction("#thisChainAccess#", bashClass.m_thisChainAccess);

    easyCC.registerSemanticAction("#bashCode#", bashClass.m_bashCode);

    easyCC.registerSemanticAction("#putOp#", bashClass.m_putOp);

    easyCC.registerSemanticAction("#startExpr#", bashClass.m_startExpr);
    easyCC.registerSemanticAction("#createExpr1#", bashClass.m_createExpr1);
    easyCC.registerSemanticAction("#createExpr2#", bashClass.m_createExpr2);
    easyCC.registerSemanticAction("#endExpr#", bashClass.m_endExpr);

    easyCC.registerSemanticAction("#startReturn#", bashClass.m_startReturn);
    easyCC.registerSemanticAction("#returnExpr#", bashClass.m_returnExpr);
    easyCC.registerSemanticAction("#endReturn#", bashClass.m_endReturn);

    easyCC.registerSemanticAction("#varAssign#", bashClass.m_varAssign);
    easyCC.registerSemanticAction("#varAccess#", bashClass.m_varAccess);
    easyCC.registerSemanticAction("#functionCall#", bashClass.m_functionCall);
    easyCC.registerSemanticAction("#functionExec#", bashClass.m_functionExec);

    easyCC.registerSemanticAction("#setArgument#", bashClass.m_setArgument);
    easyCC.registerSemanticAction("#startArgument#", bashClass.m_startArgument);
    easyCC.registerSemanticAction("#endArgument#", bashClass.m_endArgument);

    // Start compiling
    std::vector<int> phases = {BashClass::PHASE_CREATE, BashClass::PHASE_EVAL, BashClass::PHASE_GENERATE};
    for(int phase : phases) {

        // Generate code only if no semantic errors were reported
        if(phase == BashClass::PHASE_GENERATE) {
            if(BReport::getInstance().hasError()) {
                return BashClass::ERR_CODE_SEMANTIC;
            }

            // Print structure of the compiled files
            bashClass.printStructure();
        }

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
}