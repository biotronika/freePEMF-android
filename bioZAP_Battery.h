//
// Created by blos on 2017-10-14.
//

#ifndef FREEPEMF_FPBATTERY_H
#define FREEPEMF_FPBATTERY_H
#include "FP_constants.h"
#include "FP_variables.h"
#include "FPUtil.h"

class bioZAP_Battery {
public:
    bool isPcConnected();
    int getBatteryLevel();
    void rechargeBattery();
    void callibrateBattery() ;
    void callibrateBattery(int i) ;
    void verifyBatteryLevel();

private:
    int minBatteryLevel = 0;
    const unsigned long checkDeltaBatteryIncreasingVoltageTime = 600000UL;  // During charging battery minimum inreasing voltage after x millisecounds.
};

extern  bioZAP_Battery bzBattery;

#endif //FREEPEMF_FPBATTERY_H
