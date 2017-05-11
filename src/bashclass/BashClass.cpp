#include <bashclass/BashClass.h>
#include <iostream>

void BashClass::initHandlers() {
    m_startClass = [&](int phase, LexicalTokens &lexicalVector, int index, bool stable){
        std::cout << "Class created!" << std::endl;
    };
}
