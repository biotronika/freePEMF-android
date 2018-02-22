//
// Created by blos on 2017-10-14.
//

#ifndef FREEPEMF_FPPROGRAM_H
#define FREEPEMF_FPPROGRAM_H


#define PROGRAM_PC 0
#define PROGRAM1 1
#define PROGRAM2 2
#define PROGRAM3 3
#define PROGRAM_COITTEST 4
#define PROGRAM_MAGNETIZER 5

#include <Arduino.h>

class FPProgram {
public:
    void setProgram(byte programNumber);
    byte getProgram();
    void programSelectMenu();

    void runCoilTest();
    void runUserProgram();
    void runMagnetizerProgram();
    void runStandardProgram1();
    void runStandardProgram2();
    void runStandardProgram3();
    void runPcProgram();

private:
    byte programNo = 1;             // 0 = PC connection, 1= first program etc.
};

extern  FPProgram fpProgram;


#endif //FREEPEMF_FPPROGRAM_H
