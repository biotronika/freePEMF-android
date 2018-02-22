//
// Created by blos on 2017-10-14.
//

#ifndef FREEPEMF_FPCOMMANDPARSER_H
#define FREEPEMF_FPCOMMANDPARSER_H

#include <Arduino.h>
#include "FP_constants.h"

class FPCommandParser {
public:
    void executeCmd(String cmdLine, boolean directMode);

    void loadProgramToArray();
    void executeProgram();

    String param[MAX_CMD_PARAMS];           // param[0] = cmd name

private:
    void tokenizeParams(String &inputString);


    String commandArray[MAX_COMMANDS];
    String labelArray[MAX_COMMANDS];
    int labelCounter[MAX_COMMANDS];
    int loopsArray[MAX_COMMANDS];
    int rowsCount;
    int lineNumber;



};

extern FPCommandParser fpCommandParser;


#endif //FREEPEMF_FPCOMMANDPARSER_H
