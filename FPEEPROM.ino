//
// Created by blos on 2017-10-14.
//

#include <USBAPI.h>
#include <EEPROM.h>
#include "FPEEPROM.h"
#include "FP_variables.h"
#include "FPInputOutput.h"
#include "FPCommandParser.h"

FPEEPROM fpEEPROM;

void FPEEPROM::setup() {


}

int FPEEPROM::readEepromLine(int fromAddress, String &lineString) {
    //Read one line from EEPROM memory
    int i = 0;
    lineString="";
    do {
        char eeChar=(char)EEPROM.read(fromAddress+i);
        if ((eeChar==char(255)) ||(eeChar==char('@'))) {
            if (i>0) {
                eeChar='\n';
            } else {
                i=0;
                break;
            }
        }
        lineString+=eeChar;
        i++;
        if (eeChar=='\n') break;
    } while (1);

    return i;
}

void FPEEPROM::eepromUpload(int adr) {
    unsigned int i = 0;
    boolean flagCompleted;
    boolean Xoff = false;
    int endBuffer;
    //eepromLoad = true;

    do {
        //Serial.print(char(XON));
        Xoff = readSerial2Buffer(endBuffer);
        int b =0; // buffer pointer
        flagCompleted = false;
        while (!flagCompleted){

            flagCompleted = !(i+adr<PROGRAM_SIZE) || (memBuffer[b]=='@') || !(b < endBuffer);
            if (memBuffer[b]==';') memBuffer[b]='\n';   //Semicollon as end line LF (#10) for windows script
            if (memBuffer[b]=='\r') memBuffer[b] = ' '; //#13 -> space, No continue because of changing script length
            EEPROM.write(i+adr, memBuffer[b]);
            //Serial.print(memBuffer[b]);
            i++; b++;
        }
        //End of shorter program then PROGRAM_SIZE size

    } while (Xoff);
    if (i+adr<PROGRAM_SIZE) EEPROM.write(i+adr, 255);
    //eepromLoad=false;
}


boolean FPEEPROM::readSerial2Buffer(int &endBuffer) {
    int i = 0; //buffer indicator
    char c;

    boolean Xoff = false;
    int highBufferLevel = 0.7 * PROGRAM_BUFFER;

    Serial.write(XON);
    //Serial.print("\nXON\n");

    while(true) {
        if (Xoff) {
            //after send Xoff

            if (Serial.available()){
                c = Serial.read();
                memBuffer[i] = c;
                //Serial.print(c);
                endBuffer = i;
                i++;

            } else {
                break;
            };
            //if (i>= PROGRAM_BUFFER) break;


        } else {
            //before send Xoff

            Xoff = (i > highBufferLevel);

            while(!Serial.available());

            c = Serial.read();

            memBuffer[i] = c;
            //Serial.print(c);
            endBuffer = i;

            if (c == '@' ) {
                break;
            }

            i++;
            if (Xoff) {
                for (int j=0; j<64; j++)
                    Serial.write(XOFF);
                //Serial._rx_complete_irq();
                //Serial._tx_udr_empty_irq();
                //Serial.print("\nXOFF\n");
            }
        }



    }
    return Xoff;
}


bool FPEEPROM::hasRecordedProgram() {
    return (byte) EEPROM.read(0) != 255 && (byte) EEPROM.read(0) != 0;
}

int FPEEPROM::getProgramEnd() {
    int endAdr = 0;
    for (int i = 0; i < PROGRAM_SIZE; i++) {
        if ((byte) EEPROM.read(i) == 255 || (char) EEPROM.read(i) == '@') {
            endAdr = i;
            break;
        }
    }
    return  endAdr;
}

void FPEEPROM::printProgram() {

    for (int i = 0; i < PROGRAM_SIZE; i++) {
        char eeChar = (char) EEPROM.read(i);
        if ((eeChar == '@') || (eeChar == char(255))) break;
        //if (eeChar==char(255)) break;
        Serial.print(eeChar);
    }
}


void FPEEPROM::printProgramExtended() {
    Serial.println("Adr  Command");
    int adr = 0;
    int endLine;
    String line;

    while ((endLine = fpEEPROM.readEepromLine(adr, line)) && (adr < PROGRAM_SIZE)) {
        //if (line.charAt(0)=='@') break;

        fpIO.printFormattedLine(adr, line);
        adr = adr + endLine;
    }

    //End marker informs an user where start append program
    if (adr < PROGRAM_SIZE) {
        fpIO.printFormattedLine(adr, "@");
    }
}

void FPEEPROM::removeProgram() {

    for (int i = 0; i < PROGRAM_SIZE; i++) {
        EEPROM.put(i, 255);
        if (!(i % 128))
            fpIO.printDot();
    }
    fpIO.println();
}

void FPEEPROM::uploadProgram() {

    if (fpCommandParser.param[1] == "\0") {
        fpEEPROM.eepromUpload(0);

    } else if (fpCommandParser.param[1] == "@") {
        //Find script end
        int endAdr = fpEEPROM.getProgramEnd();
        fpIO.printFormattedLine(endAdr, "appendin from...");
        fpEEPROM.eepromUpload(endAdr);

    } else if (fpCommandParser.param[1].toInt() > 0 && fpCommandParser.param[1].toInt() < PROGRAM_SIZE) {
        fpEEPROM.eepromUpload(fpCommandParser.param[1].toInt());
    } else {
        Serial.print("Error: unknow parametr ");
        Serial.println(fpCommandParser.param[1]);
    }
}