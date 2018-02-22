//
// Created by blos on 2017-10-14.
//

#include <USBAPI.h>
#include "FPButton.h"
#include "FPTimer.h"
#include "FPProgram.h"

FPButton fpButton;

void FPButton::configureInterrupt() {
    attachInterrupt(digitalPinToInterrupt(btnPin), btnEvent, CHANGE);
}

void FPButton::waitForButtonRelease() {
    //Wait until turn-on button release
    fpTimer.resetTimer();
    while (digitalRead(btnPin)==HIGH){
        if (fpTimer.checkButtonTimeout() ) {
            fpProgram.setProgram(4); //Coil measurement test
            digitalWrite(redPin, HIGH);

        }
    };
}

void FPButton::buttonEvent() {
    if (digitalRead(btnPin) == HIGH) {
        pressTime = millis(); //Specific use of millis(). No increment in innteruption function.
    } else {
        if (pressTime && (millis() - pressTime > 50)) pause = !pause;
        if (pressTime && (millis() - pressTime > 1000)) {
            for (unsigned int i = 0; i < 50000; i++)
                digitalWrite(buzzPin, HIGH); //Cannot use delay() therefore beep() function in innteruption
            digitalWrite(buzzPin, LOW);
            fpUtil.off();
        }
        pressTime = 0;
    }
}


bool FPButton::butonPressed() {
    return digitalRead(btnPin) == HIGH;
}

bool FPButton::isPaused() {
    return pause;
}

void btnEvent() {
    //Change button state interruption
    fpButton.buttonEvent();
}