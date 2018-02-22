//
// Created by blos on 2017-10-14.
//

#ifndef FREEPEMF_FPEEPROM_H
#define FREEPEMF_FPEEPROM_H
#include "FP_constants.h"
#include <Arduino.h>

class FPEEPROM {
public:
    void setup();
    int readEepromLine(int fromAddress, String &lineString);
    void eepromUpload(int adr);
    boolean readSerial2Buffer(int &endBuffer);
    bool hasRecordedProgram();
    int getProgramEnd();

    void printProgram();
    void printProgramExtended();
    void removeProgram();
    void uploadProgram();

private:
    char memBuffer[PROGRAM_BUFFER];
};

extern FPEEPROM fpEEPROM;

#endif //FREEPEMF_FPEEPROM_H
