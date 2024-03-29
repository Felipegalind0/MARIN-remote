// Copyright (c) M5Stack. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for full
// license information.

#include "M5StickCPlus.h"
#include "startup_ASCII_artwork.h"

M5StickCPlus::M5StickCPlus() : isInited(0) {
}

void M5StickCPlus::begin(bool LCDEnable, bool PowerEnable, bool SerialEnable) {
    //! Correct init once
    if (isInited)
        return;
    else
        isInited = true;

    //! UART
    if (SerialEnable) {
        Serial.begin(115200);
        Serial.flush();
        delay(50);
        Serial.println(startup_ASCII_artwork);
    }

    // Power

    if (PowerEnable) {
        Serial.println("Axp.begin()");
        Axp.begin();
    }

    // LCD INIT
    if (LCDEnable) {
        Serial.println("Lcd.begin()");
        Lcd.begin();
    }

    // Serial.println("Beep.begin()");
    // Beep.begin();

    Serial.println("Rtc.begin()");
    Rtc.begin();
}

void M5StickCPlus::update() {
    M5.BtnA.read();
    M5.BtnB.read();
    M5.Beep.update();
}

M5StickCPlus M5;