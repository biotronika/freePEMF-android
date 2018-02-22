//
// Created by blos on 2017-10-14.
//

#include <USBAPI.h>
#include <EEPROM.h>
#include "bioZAP_Battery.h"
#include "FPInputOutput.h"
#include "FPTimer.h"
#include "libraries/TimerOne/TimerOne.cpp"

bioZAP_Battery bzBattery;

int bioZAP_Battery::getBatteryLevel() {
    return (int) (analogRead(batPin) *
                  BATTERY_VOLTAGE_RATIO *
                  (byte) EEPROM.read(EEPROM_BATTERY_CALIBRATION_ADDRESS) /
                  100
    );

}

bool bioZAP_Battery::isPcConnected() {
    return getBatteryLevel() < USB_POWER_SUPPLY_LEVEL;
}

void bioZAP_Battery::rechargeBattery() {
    //Recharger is pluged

    digitalWrite(powerRelayPin, LOW); // turn power relay off
    digitalWrite(redPin, HIGH);
    fpUtil.beepNormal();
    digitalWrite(greenPin, LOW);
    int startBatLevel = analogRead(batPin);

    FPTimer rechargeTimer;

    do {
        if (rechargeTimer.checkTimeout(checkDeltaBatteryIncreasingVoltageTime)) {
            if (analogRead(batPin) - startBatLevel <= 0) { //no inreasing voltage
                //Battery rechareged

                digitalWrite(greenPin, HIGH);
                fpUtil.beepNormal();
                // ... and charege further.
                while (1);
            }

            //Start new charging period with new values
            rechargeTimer.resetTimer();
            startBatLevel = analogRead(batPin);
        }
    } while (1); //forever loop
}

void bioZAP_Battery::callibrateBattery() {
    //Auto-correction voltage - for new device
    if ((byte) EEPROM.read(EEPROM_BATTERY_CALIBRATION_ADDRESS) > 130 ||
        (byte) EEPROM.read(EEPROM_BATTERY_CALIBRATION_ADDRESS) < 70) {
        EEPROM.put(EEPROM_BATTERY_CALIBRATION_ADDRESS, 100); // 100 =  x 1.00
    }

    //Define minimum battery level uses in working for perfomance puropose.
    minBatteryLevel /*0-1023*/= (int) (100 *
                                       MIN_BATTERY_LEVEL /
                                       BATTERY_VOLTAGE_RATIO /
                                       (byte) EEPROM.read(EEPROM_BATTERY_CALIBRATION_ADDRESS));
}


void bioZAP_Battery::callibrateBattery(int newLevel) {
    byte i = (byte) 100 * newLevel / (int(analogRead(batPin) * BATTERY_VOLTAGE_RATIO));
    EEPROM.put(EEPROM_BATTERY_CALIBRATION_ADDRESS, i);
}

void bioZAP_Battery::verifyBatteryLevel() {

#ifndef TEST_MODE
    if (analogRead(batPin) < minBatteryLevel) {
        //Emergency turn off
        fpIO.println();
        fpIO.println(F("Error: battery too low:"), getBatteryLevel());

        // red LED on
        digitalWrite(redPin, HIGH);
        digitalWrite(greenPin, LOW);

        // Turn all off
        digitalWrite(coilPin, LOW);    // Turn coil off by making the voltage LOW
        digitalWrite(relayPin, LOW);    // Relay off

        for (int x = 0; x < 10; x++) {
            digitalWrite(buzzPin, HIGH);   // Turn buzzer on
            delay(100);
            digitalWrite(buzzPin, LOW);    // Turn buzzer off
            delay(200);
        }
        fpUtil.beepLong();
        fpUtil.off();
    }
#endif
}