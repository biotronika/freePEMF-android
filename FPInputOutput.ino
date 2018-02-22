//
// Created by blos on 2017-10-14.
//

#include <HardwareSerial.h>
#include <USBAPI.h>
#include "FPInputOutput.h"
#include "FP_variables.h"

FPInputOutput fpIO;

void FPInputOutput::setup() {
    //BIOzap
    // Initialize serial communication to PC communication
    Serial.begin(9600);
    // reserve the bytes for the inputString:
    inputString.reserve(65); //NANO serial buffer has 63 bytes


}

void FPInputOutput::printWelcomeScreen() {

    Serial.println(F(WELCOME_SCR));
    Serial.print(F("Device free-PEMF "));
    Serial.print(F(HRDW_VER));
    Serial.print(F(" "));
    Serial.println(F(SOFT_VER));
    Serial.print(F(">"));

}

void FPInputOutput::printFormattedLine(int adr, String line) {

    String printLine;
    printLine.reserve(22);
    printLine = "000" + String(adr, DEC);
    printLine = printLine.substring(printLine.length() - 3, printLine.length());
    printLine += ": " + line; //end marker for appending program

    println(printLine);

}


void FPInputOutput::serialEvent() {
    //if (!eepromLoad) {
    while (Serial.available()) {
        // get the new byte:
        char inChar = (char) Serial.read();
        // add it to the inputString:
        if (inChar != '\r') {
            inputString += inChar;
        }
        Serial.print(inChar); //echo
        // if the incoming character is a newline, set a flag
        if (inChar == '\n') {
            stringComplete = true;
        }

        if (inChar == '@') {
            memComplete = true;
        }
    }
    //}
}

bool FPInputOutput::commandExecuted() {
    return stringComplete;
}

void FPInputOutput::resetCommand() {
    Serial.print('>'); //Currsor for new command

    // clear the command string
    inputString = "";
    stringComplete = false;
}

String FPInputOutput::getCommand() {
    return inputString;
}

void FPInputOutput::println() {
    Serial.println();
}

void FPInputOutput::println(String s) {
    Serial.println(s);
}

void FPInputOutput::println(int s) {
    Serial.println(s);
}
void FPInputOutput::println(String s, String param) {
    Serial.print(s);
    Serial.print(F(" "));
    Serial.println(param);
}

void FPInputOutput::println(String s, int param) {
    Serial.print(s);
    Serial.print(F(" "));
    Serial.println(param);
}

void FPInputOutput::printDot() {
    Serial.print(F("."));
}



void FPInputOutput::printOK(String s) {
    Serial.print(F("OK"));
    Serial.print(F(" - "));
    Serial.println(s);
}

void FPInputOutput::printNOTOK(String s) {
    Serial.print(F("NOT OK"));
    Serial.print(F(" - "));
    Serial.println(s);
}
