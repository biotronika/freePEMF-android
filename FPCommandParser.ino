//
// Created by blos on 2017-10-14.
//

#include "FP_functions.h"
#include "FPCommandParser.h"

FPCommandParser fpCommandParser;

void FPCommandParser::tokenizeParams(String &inputString) {
    for (int i = 0; i < MAX_CMD_PARAMS; i++) param[i] = "";

    int from = 0;
    int to = 0;
    for (int i = 0; i < MAX_CMD_PARAMS; i++) {
        to = inputString.indexOf(' ', from); //Wykryj spacje

        if (to == -1) {
            to = inputString.indexOf('\n', from); //Wykryj NL #10
            if (to > 0) param[i] = inputString.substring(from, to);
            param[i].trim();
            break;
        }

        if (to > 0)
            param[i] = inputString.substring(from, to);
        param[i].trim();
        from = to + 1;
    }
}

void FPCommandParser::executeCmd(String cmdLine, boolean directMode) {
    // Main interpreter function
    tokenizeParams(cmdLine);

    if (param[0] == F("mem")) {
        // Upload terapy to EEPROM
        fpEEPROM.uploadProgram();
        fpIO.printOK(param[0]);
    } else if (param[0] == F("ls")) {
        //List terapy
        if (param[1] == F("-n")) {
            fpEEPROM.printProgramExtended();
        } else {
            fpEEPROM.printProgram();
        }
        fpIO.printOK(param[0]);
    } else if (param[0].charAt(0) == '#') {
        // Comment
        fpIO.println("Comment", param[0]);
    } else if (param[0] == F("")) {
        // Emptyline

    } else if (param[0] == F("rm")) {

        fpEEPROM.removeProgram(); // Remove, clear terapty
        fpIO.printOK(param[0]);

    } else if (param[0] == F("print")) {
        // Print command
        if (cmdLine.length() > 6) {
            fpIO.println(cmdLine.substring(6, cmdLine.length() - 1));
        } else {
            fpIO.println();
        }
        fpIO.printOK(param[0]);

    } else if (param[0] == F("bat")) {
        // Print baterry voltage
        fpIO.println(bat());
        fpIO.printOK(param[0]);
    } else if (param[0] == F("cbat")) {
        // Calibrate battery voltage
        bzBattery.callibrateBattery(param[1].toInt());
        fpIO.printOK(param[0]);

    } else if (param[0] == F("hr")) {
        // Print heart rate
        fpIO.println("to be implemented");
        fpIO.printOK(param[0]);

    } else if (param[0] == F("beep")) {
        // Beep [time_ms]

        if (param[1] != "\0") {
            beep(param[1].toInt());
            fpIO.printOK(param[0]);
        } else {
            fpIO.printNOTOK(param[0]);
        }

    } else if (param[0] == "off") {
        // Turn off
        off();
        fpIO.printOK(param[0]);

    } else if (param[0] == "out") {

        if (param[1] != "\0") {
            digitalWrite(coilPin, param[1].toInt());
            fpIO.printOK(param[0]);
        } else {
            fpIO.printNOTOK(param[0]);
        }

    } else if (param[0] == "chp") {
        // Change output signal polarity
        if (param[1] == "\0") {
            chp();
        } else {
            chp(byte(param[1].toInt()));
        }
        fpIO.printOK(param[0]);
    } else if (param[0] == F("wait")) {
        // Wait millis
        if (param[1] != "\0") {
            wait(param[1].toInt());
            fpIO.printOK(param[0]);
        } else {
            fpIO.printNOTOK(param[0]);
        }

    } else if (param[0] == F("rec") || param[0] == F("freq")) {
        // Generate rectangle signal - rec [freq] [time_sec]
        if (param[3] == "\0") {
            rec(param[1].toInt(), param[2].toInt());
        } else {
            rec(param[1].toInt(), param[2].toInt(), param[3].toInt());
        }
        fpIO.printOK(param[0]);

    } else if (param[0] == F("scan")) {
        // Scan from lastFreq  - scan [freq to] [time_ms]
        if (param[1] != "\0" && param[2] != "\0") {
            scan(param[1].toInt(), param[2].toInt());
            fpIO.printOK(param[0]);
        } else {
            fpIO.printNOTOK(param[0]);
        }

    } else if (param[0] == F("jump")) {
        // jump label

        Serial.print(cmdLine);

        if (param[1] != "\0" && param[2] != "\0") {
            for (int i = 0; i < rowsCount; i++) {
                if (param[1] == labelArray[i]) {
                    if (loopsArray[lineNumber] < (param[2].toInt() - 1)) {
                        loopsArray[lineNumber] = loopsArray[lineNumber] + 1;

                        if (labelCounter[i] > 0) {
                            fpIO.println("loop", loopsArray[lineNumber]);
                            lineNumber = i;
                            return;
                        }
                    }
                }
            }

        } else {
            fpIO.printNOTOK(param[0]);
        }

    } else if (param[0] == F("restart")) {
        // przeskok do początku programu
        lineNumber = 0;
        fpIO.printOK(param[0]);

    } else if (param[0].charAt(0) == ':') {
        // jump definition
        labelCounter[lineNumber] = labelCounter[lineNumber] - 1;
        lineNumber++;
        fpIO.printOK(param[0]);
    } else if (param[0] == F("exe")) {
        // Execute eeprom program only in direc mode
        if (directMode) {
            exe();
        } else {
            fpIO.println(F("Error: can't execute program from eeprom program!"));
        }
    } else if (param[0] == F("params")) {
        // Print command
        fpIO.println("param0:", param[0]);
        fpIO.println("param1:", param[1]);
        fpIO.println("param2:", param[2]);
        fpIO.println("param3:", param[3]);

        fpIO.println("Xparam0:", param[0] == "\0");
        fpIO.println("Xparam1:", param[1] == "\0");
        fpIO.println("Xparam2:", param[2] == "\0");
        fpIO.println("Xparam3:", param[3] == "\0");

    } else {
        //Unknown command
        fpIO.println(F("Unknown command: "), param[0]);
    }
}


void FPCommandParser::loadProgramToArray() {

    int adr = 0;
    rowsCount = 0;
    String line;

    while (int endLine = fpEEPROM.readEepromLine(adr, line)) {

        Serial.print(F("building array: "));
        Serial.print(line);
        adr = adr + endLine;

        if (line.charAt(0) == ':') {
            // dodajemy definicję jump
            tokenizeParams(line);
            //String labelDefinition = line.substring(1, line.length() - 2);
            labelArray[rowsCount] = param[0].substring(1);
            if (param[1] != "\0") {
                labelCounter[rowsCount] = param[1].toInt();
            }

        } else {
            labelArray[rowsCount] = "";
            commandArray[rowsCount] = line;
        }
        loopsArray[rowsCount] = 0;
        rowsCount++;
    }
}


void FPCommandParser::executeProgram() {

    lineNumber = 0;
    while (lineNumber < rowsCount) {
        String command = commandArray[lineNumber];
        lineNumber++;
        executeCmd(command, false);
    }
}
