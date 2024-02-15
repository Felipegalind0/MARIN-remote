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

    Serial.print("CPU Cores: ");

    Serial.print("Core");
    Serial.print(BackgroundCore);
    Serial.print(": BackgroundCore");

    Serial.print("Core");
    Serial.print(RealTcore);
    Serial.print(": RealTcore");

    Serial.println();

    Serial.print("@RealTcode: Boot process complete in core #");
    Serial.println(xPortGetCoreID());

    Serial.println("is_booted = true");
    is_booted = true;
    

    Serial.println("@SysInit_Setup COMPLETE");
}


void SysInit_Check(void){
    //digitalWrite(LED, HIGH);
    Serial.println("");
    Serial.println("StartUp Complete!");
    //digitalWrite(LED, LOW);
}