//
// Created by blos on 2017-10-14.
//

#ifndef CONSTANTS_H
#define CONSTANTS_H

#define HRDW_VER "NANO 4.2"
#define SOFT_VER "2017-07-28"

//Pin definition
#define coilPin 5      // Coil driver IRF510
#define powerRelayPin 4     // Power relay
#define relayPin 9     // Direction relay
#define buzzPin 10     // Buzzer
#define btnPin 3       // Power On-Off / Pause / Change program button
#define redPin 12      // Red LED
#define greenPin 11    // Green LED
#define hrmPin 2       // Biofeedback HR meter on 3th plug pin.

//Battery staff
#define batPin A7                               // Analog-in battery level
#define BATTERY_VOLTAGE_RATIO 0.153             // nclude div 10k/4,7k resistors. 5V*(10k+4,7k)/4,7k = 0,0153 (x10)
#define EEPROM_BATTERY_CALIBRATION_ADDRESS 1023 // Memory address of battery correction factor - 100 means RATIO x 1,00
#define MIN_BATTERY_LEVEL 90                    // 90 means 9.0 V  (x10), less then that turn off
#define USB_POWER_SUPPLY_LEVEL 65               // Maximum usb voltage level means 6.5V



//BIOzap
#define WELCOME_SCR "Free BIOzap interpreter welcome! See http://biotronika.pl"
#define PROGRAM_SIZE 1000   // Maximum program size
#define PROGRAM_BUFFER 500  // SRAM buffer size, used for script loading
#define MAX_CMD_PARAMS 4    // Count of command params
#define LCD_SCREEN_LINE -1  // LCD user line number, -1 = no lcd
#define MIN_FREQ_OUT 1      //  0.01 Hz
#define MAX_FREQ_OUT 5000   // 50.00 Hz
#define SCAN_STEPS 100      // For scan function puropose - default steps
#define XON 17  //0x11
#define XOFF 19 //0x13

#define DEFAULT_LOOPS_COUNT 5    // Count of command params
#define MAX_COMMANDS 20    // Count of command params

#endif