//
// Created by blos on 2017-10-14.
//

#include "FPProgram.h"

#include "FP_variables.h"
#include "FP_functions.h"
#include "FPUtil.h"
#include "FPTimer.h"
#include "FPInputOutput.h"

FPProgram fpProgram;

void FPProgram::setProgram(byte programNumber) {
    programNo = programNumber;
}

byte FPProgram::getProgram() {
    return  programNo;
}

void FPProgram::programSelectMenu() {
    FPTimer timer;
    FPTimer secondCounter;
    while (timer.checkButtonTimeout()) {
        if (digitalRead(btnPin)) {
            //Reset start moment after btn preesed
            secondCounter.resetTimer();

            programNo++;
            if (programNo > 3) programNo = 1;
            for (int p = programNo; p > 0; p--) {
                //Signals count
                fpUtil.beepShort();
                delay(150);
            }

            //Wait until button is pressed
            while (digitalRead(btnPin));

            //Turn off if pressed more then 1 sec.
            if (secondCounter.checkTimeout(1000)) {
                fpUtil.beepLong();
                off();
            }
        }
    }
}



void FPProgram::runCoilTest() {
    digitalWrite(redPin, LOW);
    while (1) {

        bzBattery.verifyBatteryLevel();

        if (digitalRead(btnPin)) {
            fpTimer.resetTimer();
            fpUtil.beepShort();
            while (digitalRead(btnPin));

            if (fpTimer.checkButtonTimeout()) {
                fpUtil.beepLong();
                off();
            }
            fpUtil.switchCoilState();
            digitalWrite(coilPin, fpUtil.getCoilState());   // turn coil on/off
            digitalWrite(redPin, fpUtil.getCoilState());   // turn LED on/off
        }
    }
}

void FPProgram::runStandardProgram1() {

    rec(1179, 120); //2 min   11,79Hz Earth geomagnetic field
    chp(1);
    rec(783, 120); //2 min    7,83 Schuman
    chp(0);
    rec(2000, 60); //2min    20,0  Capillary (ukł. krwinośny)
    chp(1);
    rec(1500, 60); //2min    15,0  Capillary
    chp(0);
    rec(1000, 90); //1:30min 10,0  Ligament (wiązadła)
    chp(1);
    rec(700, 90); //1:30min  7,0  Bone (kości)
    chp(0);
    rec(200, 120); //2min     2,0  Nerve
    beep(500);
    off();
}

void FPProgram::runStandardProgram2() {

    rec(1179, 120); //4 min 11,79Hz Earth geomagnetic field
    chp(1);
    rec(1179, 120);
    chp(0);
    rec(783, 120); //4 min 7,83 Schumanns resonance
    chp(1);
    rec(783, 120);
    beep(500);
    off();
}

void FPProgram::runStandardProgram3() {

    // 16 min
    rec(1179, 120); //2 min 11,79Hz Earth geomagnetic field
    chp(1);
    rec(1200, 10); //4 min 12->8Hz  Alpha
    scan(800, 230);
    chp(0);
    rec(783, 120); //2 min  7,83 Schumanns resonance
    chp(1);
    rec(800, 10);
    scan(400, 230);  //4 min 8->4 Theta
    chp(0);
    scan(100, 240);  //4 min 4->0,5  Delta
    beep(500);
    off();
}

void FPProgram::runUserProgram() {
    executeCmd("exe\n", true);
    off();
}

void FPProgram::runPcProgram() {

    if (fpIO.commandExecuted()) {
        //Restart timeout interval to turn off.
        fpTimer.resetTimer();
        executeCmd(fpIO.getCommand(), true);
        fpIO.resetCommand();
    }
}

void FPProgram::runMagnetizerProgram() {
    //Special signal
    digitalWrite(greenPin, HIGH);
    digitalWrite(redPin, HIGH);
    fpUtil.beepNormal();
    delay(1000);
    digitalWrite(redPin, LOW);

    digitalWrite(greenPin, HIGH);

    fpUtil.beepNormal();
    delay(1000);

    //Forever  loop
    while (1) {
        digitalWrite(greenPin, HIGH);
        digitalWrite(powerRelayPin, HIGH);
        delay(50);
        executeCmd("exe\n", true); //default programNo;
        digitalWrite(powerRelayPin, LOW);
        delay(10000);
    }
}