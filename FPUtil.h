//
// Created by blos on 2017-10-14.
//

#ifndef FREEPEMF_FPUTIL_H
#define FREEPEMF_FPUTIL_H

#include "FP_constants.h"
#include <Arduino.h>

class FPUtil {

public:

    void setupPins();

    void beep(unsigned int period);

    void off();

    void powerOn();

    void chp(byte outputDir);
    void chp();

    void beepShort();
    void beepNormal();
    void beepLong();

    void switchCoilState();
    byte getCoilState();
    void setCoilState(byte state);

private:
    byte polarity = 0;
    byte coilState = LOW;
};

extern FPUtil fpUtil;


#endif //FREEPEMF_FPUTIL_H
