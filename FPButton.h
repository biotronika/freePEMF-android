//
// Created by blos on 2017-10-14.
//

#ifndef FREEPEMF_FPBUTTON_H
#define FREEPEMF_FPBUTTON_H

#include <USBAPI.h>
#include "FP_constants.h"
#include "FP_variables.h"
#include "FPUtil.h"

class FPButton {
public:
    void configureInterrupt();
    void waitForButtonRelease();
    void buttonEvent();
    bool butonPressed();
    bool isPaused();

private:
    unsigned long pressTime = 0;    // Time of pressing the button
    volatile boolean pause = false; // true = pause on
};

extern  FPButton fpButton;

#endif //FREEPEMF_FPBUTTON_H
