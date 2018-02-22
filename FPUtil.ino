//
// Created by blos on 2017-10-14.
//

#include <USBAPI.h>
#include "FPUtil.h"
#include "FPTimer.h"
#include "FPInputOutput.h"

FPUtil fpUtil;

void FPUtil::setupPins() {
    // Initialize the digital pin as an in/output
    pinMode(coilPin, OUTPUT);  // Coil driver
    pinMode(powerRelayPin, OUTPUT);  // Power relay
    pinMode(greenPin, OUTPUT);  // LED on board
    pinMode(redPin, OUTPUT);  // LED on board
    pinMode(relayPin, OUTPUT);  // Direction signal relay
    pinMode(buzzPin, OUTPUT);  // Buzzer relay (5V or 12V which is no so loud)
    pinMode(btnPin, INPUT);  // Main button
    pinMode(hrmPin, INPUT_PULLUP); //Devices connection
}

void FPUtil::beep(unsigned int period) {
    // beep [period_ms]
    FPTimer timer;
    FPTimer secondCounter;

    digitalWrite(buzzPin, HIGH);
    while (timer.isTicking(period)) {
        //time loop
        //TODO serial break command - mark @
        //count each second
        if (secondCounter.checkTimeout(1000)) { //one second
            fpIO.printDot();
            secondCounter.resetTimer();
        }
    }
    fpIO.println();
    digitalWrite(buzzPin, LOW);
}

void FPUtil::off() {
    // Power off function
    digitalWrite(coilPin, LOW);     // Turn coil off by making the voltage LOW
    delay(20);
    digitalWrite(relayPin, LOW);    // Relay off
    digitalWrite(greenPin, LOW);    // Green LED off
    digitalWrite(powerRelayPin, LOW);  // Turn power off //If not USB power

    while (digitalRead(btnPin) == HIGH); // Wait because still power on
    //If USB PC connection is pluged microcontroller cannot turn power off
    //detachInterrupt(digitalPinToInterrupt(btnPin));
    while (1); //forever loop
}

void FPUtil::powerOn() {
    //Power on
    digitalWrite(powerRelayPin, HIGH);
}

void FPUtil::chp(byte outputDir) {

    digitalWrite(coilPin, LOW);  // turning coil off
    if (outputDir) {
        digitalWrite(relayPin, HIGH); // turn relay on
    } else {
        digitalWrite(relayPin, LOW); // turn relay off
    }
}

void FPUtil::chp() {
    polarity ^= 1;
    chp(polarity);
}

void FPUtil::beepShort() {
    beep(100);
}

void FPUtil::beepNormal() {
    beep(200);
}

void FPUtil::beepLong() {
    beep(500);
}

void FPUtil::switchCoilState() {
    coilState ^= 1;
}

byte FPUtil::getCoilState() {
    return coilState;
}

void FPUtil::setCoilState(byte state) {
    coilState = state;
}