//
// Created by blos on 2017-10-14.
//

#ifndef FREEPEMF_FPINPUTOUTPUT_H
#define FREEPEMF_FPINPUTOUTPUT_H
#include "FP_constants.h"
#include <Arduino.h>

class FPInputOutput {

public:


    void setup();
    void printWelcomeScreen();
    void serialEvent();
    void printFormattedLine(int adr, String line);
    bool commandExecuted();
    void resetCommand();
    String getCommand();
    void println();
    void println(String s);
    void println(int s);
    void println(String s, String param);
    void println(String s, int param);
    void printDot();
    void printOK(String s);
    void printNOTOK(String s);

private:
    boolean stringComplete = false;         // whether the string is complete
    String inputString = "";                // a string to hold incoming serial data
    boolean memComplete = false;
};


extern FPInputOutput fpIO;

#endif //FREEPEMF_FPSERIAL_H
