#include "M5StickCPlus.h"
#include "systeminit.h"
#include "speaker.h"
#include "IO.h"
#include "LCD.h"
#include "wireless.h"
#include "COMS.h"
#include "JoyC.h"
#include "variables.h"
#include "pinout.h"

#include <Arduino.h>

void printESP32Info() {
    // Retrieve and print the ESP32 chip information
    esp_chip_info_t chip_info;
    esp_chip_info(&chip_info);

    Serial.println("ESP32 Information:");
    
    // Print the number of cores
    Serial.print("  - CPU cores: ");
    Serial.println(chip_info.cores);

    // Print the chip revision
    Serial.print("  - Chip revision: ");
    Serial.println(chip_info.revision);

    // Print the features
    Serial.print("  - Features: ");
    if (chip_info.features & CHIP_FEATURE_EMB_FLASH) {
        Serial.print("Embedded Flash, ");
    }
    if (chip_info.features & CHIP_FEATURE_WIFI_BGN) {
        Serial.print("802.11bgn WiFi, ");
    }
    if (chip_info.features & CHIP_FEATURE_BLE) {
        Serial.print("Bluetooth LE, ");
    }
    if (chip_info.features & CHIP_FEATURE_BT) {
        Serial.print("Classic Bluetooth, ");
    }
    Serial.println();

    // Print the ESP32 model
    Serial.print("  - Model: ");
    Serial.println(ESP.getChipModel());

    // Print the chip revision number
    Serial.print("  - Revision number: ");
    Serial.println(ESP.getChipRevision());

    // Print the Flash chip size
    Serial.print("  - Flash chip size: ");
    Serial.print(ESP.getFlashChipSize() / (1024 * 1024));
    Serial.println(" MB");

    // Print the free heap size
    Serial.print("  - Free heap size: ");
    Serial.print(ESP.getFreeHeap() / 1024);
    Serial.println(" KB");

    // Print the minimum free heap size since boot
    Serial.print("  - Minimum free heap size since boot: ");
    Serial.print(ESP.getMinFreeHeap() / 1024);
    Serial.println(" KB");

    // Print the chip ID
    Serial.print("  - Chip ID: ");
    Serial.println((uint32_t)ESP.getEfuseMac());
}

void SysInit_Setup(void){

    //disableCore0WDT();
    
    M5.begin();

    Serial.println("");

    Serial.println("Wire.begin(wire_sda, wire_scl);");
    Wire.begin(wire_sda, wire_scl);


    Serial.println("LCD_UI_Setup()");
    LCD_UI_Setup();

    Serial.println("Setup_Speaker()");
    Setup_Speaker();

    Setup_Pinout();


    Serial.println("Playing StartUp Sound");
    xTaskCreatePinnedToCore(
        StartUp_Sound,   /* Function to implement the task */
        "StartUp_Sound", /* Name of the task */
        10000,      /* Stack size in words */
        NULL,       /* Task input parameter */
        -2,          /* Priority of the task */
        NULL,       /* Task handle. */
        BackgroundCore);  /* Core where the task should run */



    JoyC_setup();

    Serial.println("CPU Cores: ");

    Serial.print("Core");
    Serial.print(BackgroundCore);
    Serial.print(": BackgroundCore");

    Serial.print("\nCore");
    Serial.print(RealTcore);
    Serial.print(": RealTcore");

    Serial.println();

    Serial.print("@RealTcode: Boot process complete in core #");
    Serial.println(xPortGetCoreID());

    Serial.println("is_booted = true");
    is_booted = true;

    printESP32Info();
    

    Serial.println("@SysInit_Setup COMPLETE");
}


void SysInit_Check(void){
    //digitalWrite(LED, HIGH);
    Serial.println("");
    Serial.println("StartUp Complete!");
    //digitalWrite(LED, LOW);
}