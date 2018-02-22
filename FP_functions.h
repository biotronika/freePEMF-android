//
// Created by blos on 2017-10-14.
//

#ifndef FREEPEMF_FP_FUNCTIONS_H
#define FREEPEMF_FP_FUNCTIONS_H

#include <Arduino.h>
#include "FPUtil.h"
#include "bioZAP_Battery.h"
#include "FPEEPROM.h"
#include "FPCommandParser.h"
#include "FPTimer.h"
#include "FPInputOutput.h"
#include "libraries/TimerOne/TimerOne.h"
#include "FPButton.h"

// plik z funkcjami, które może wykorzystywać użytkownik w terapiach

void beep(unsigned int period) {
    fpUtil.beep(period);
}

void off() {
    fpUtil.off();
}

void chp(byte outputDir) {
    fpUtil.chp(outputDir); //Change output polarity
}

void chp() {
    fpUtil.chp();
}

int bat() {
    return bzBattery.getBatteryLevel(); // Get battery voltage function
}


void executeCmd(String cmdLine, boolean directMode = false) {
    fpCommandParser.executeCmd(cmdLine, directMode);
}

void exe() {
    //Execute program
    fpCommandParser.loadProgramToArray();
    fpCommandParser.executeProgram();
}




void callback() {
    fpUtil.switchCoilState();
    digitalWrite(greenPin, fpUtil.getCoilState());
    digitalWrite(coilPin, fpUtil.getCoilState());
}

void rec(unsigned int freq, unsigned long period) {
    //Rectangle signal generate, freq=783 for 7.83Hz, period in secounds

    lastFreq = constrain(freq, MIN_FREQ_OUT, MAX_FREQ_OUT); //For scan() function puropose
    unsigned long interval = (unsigned long) (50000000 / lastFreq);
    FPTimer timer;
    FPTimer secondCounter;
    unsigned long timeUp = period * 1000;

    Timer1.initialize(interval);
    Timer1.attachInterrupt(callback);

    while (timer.isTicking(timeUp)) {
        //time loop

        //TODO serial break command - mark @
        if (fpButton.isPaused()) {
            //Pause - button pressed
            fpTimer.resetTimer();
            fpUtil.beepNormal();
            digitalWrite(coilPin, LOW);     // turn coil ooff
            digitalWrite(greenPin, HIGH);   // turn LED on
            while (fpButton.isPaused()) {
                //wait pauseTimeOut or button pressed
                if (fpTimer.checkPauseTimeout()) {
                    fpUtil.beepLong();
                    off();
                }
            }
            fpUtil.beepNormal();
            //Correct working time
            timeUp += millis() - fpTimer.getTimerValue();
            //Continue
            digitalWrite(coilPin, fpUtil.getCoilState());    // turn coil on
            digitalWrite(greenPin, fpUtil.getCoilState());   // turn LED on/
        }

        //count each secondCounter
        if (secondCounter.checkTimeout(1000)) {  //count each secondCounter
            fpIO.printDot();
            secondCounter.resetTimer();
            bzBattery.verifyBatteryLevel(); //If too low then off
        }
    }
    fpIO.println();

    Timer1.detachInterrupt();
    fpUtil.setCoilState(LOW);
    digitalWrite(coilPin, fpUtil.getCoilState());     // turn coil off
    digitalWrite(greenPin, HIGH);   // turn LED on
}


void rec(unsigned int freq, unsigned long totalTime, unsigned long changeTime) {
    int stepsCount = (int)(totalTime / changeTime);
    for (int i = 0; i < stepsCount; i++) {
        chp();
        rec(freq, changeTime);
    }
}

void scan(unsigned long freq, unsigned long period) {
    // Scan from lastFreq to freq used SCAN_STEPS by period

    long scanSteps = SCAN_STEPS;
    long stepPeriod = period / scanSteps;
    if (stepPeriod < 1) {
        scanSteps = period;
        stepPeriod = 1;
    }
    long startFreq = lastFreq;
    long stepFreq = long(constrain(freq, MIN_FREQ_OUT, MAX_FREQ_OUT) - lastFreq) / scanSteps;

    for (int i = 0; i < scanSteps; i++) {
        rec(startFreq + (i * stepFreq), stepPeriod);
    }
}


void wait(unsigned long period) {
    // wait [period_ms]
    FPTimer timer;
    FPTimer secondCounter;
    while (timer.isTicking(period)) {
        //TODO serial break command - mark @
        if (secondCounter.checkTimeout(1000)) {  //count each second
            fpIO.printDot();
            secondCounter.resetTimer();
        }
    }
    fpIO.println();
}

#endif //FREEPEMF_FP_FUNCTIONS_H
