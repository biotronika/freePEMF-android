// free-PEMF See: biotronika.pl  biotronics.eu
// Chris Czoba (c) krzysiek@biotronika.pl

#include <Arduino.h>
#include "FP_constants.h"
#include "FP_functions.h"
#include "FPInputOutput.h"
#include "FPButton.h"
#include "FPProgram.h"

#define TEST_MODE

void setup() {

    fpUtil.setupPins();
    fpIO.setup();

    if (bzBattery.isPcConnected()) {
        //Detected USB PC connection
        fpProgram.setProgram(PROGRAM_PC);
    } else if (fpButton.butonPressed()) {
        //Power button pressed
        //Turn power on
        digitalWrite(powerRelayPin, HIGH);
    } else if (digitalRead(hrmPin) == LOW) {
        //Power supplyer id pluged
        fpProgram.setProgram(PROGRAM_MAGNETIZER);
        exit;
    } else {
        //Work as a power charger
        bzBattery.rechargeBattery();
    }

    //Turn on green LED
    digitalWrite(greenPin, HIGH);
    fpUtil.beepNormal();
    fpButton.waitForButtonRelease();
    delay(10);
    bzBattery.callibrateBattery();

    if (fpProgram.getProgram() && fpProgram.getProgram() < 4) {
        //Program select
        fpProgram.programSelectMenu();

    } else {
        //PC option
        fpUtil.powerOn();
        fpIO.printWelcomeScreen();
    }

    fpButton.configureInterrupt();
    fpTimer.resetTimer();
}


void loop() {

    switch (fpProgram.getProgram()) {
        case PROGRAM1:
            //Check if user program is load in EEPROM memory
            if (fpEEPROM.hasRecordedProgram()) {
                fpProgram.runUserProgram(); //User program execute
            } else {
                fpProgram.runStandardProgram1(); //Standard program execute
            }
            break;
        case PROGRAM2:
            fpProgram.runStandardProgram2(); //Earth regeneration - 8 minutes
            break;
        case PROGRAM3:
            fpProgram.runStandardProgram3(); // Antistress & meditation (without feedback)
            break;
        case PROGRAM_COITTEST:
            fpProgram.runCoilTest(); // Coil test
            break;
        case PROGRAM_MAGNETIZER:
            fpProgram.runMagnetizerProgram();
            break;
        default:
            fpProgram.runPcProgram(); // PC controled program
            break;

    }

    if (fpTimer.checkPauseTimeout()) {
        fpUtil.off();
    }
}

void serialEvent() {
    fpIO.serialEvent();
}



