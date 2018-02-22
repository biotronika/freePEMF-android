//
// Created by blos on 2017-10-14.
//

#ifndef FREEPEMF_FPTIMER_H
#define FREEPEMF_FPTIMER_H
#include "FP_constants.h"

class FPTimer {
public:
    FPTimer();
    void resetTimer();
    bool checkTimeout(unsigned long timeout);
    bool checkPauseTimeout();
    bool checkButtonTimeout();

    bool isTicking(unsigned long time);

    unsigned long getTimerValue();

private:
    unsigned long startIntervalA;    // For unused timeout off.
    const unsigned long pauseTimeOut = 600000UL;                            // 600000 Time of waiting in pause state as turn power off. (60000 = 1 min.)
    const unsigned int btnTimeOut = 5000UL;                                 // Choose therapy program time out. Counted form released button.
};

extern FPTimer fpTimer;

#endif //FREEPEMF_FPTIMER_H
