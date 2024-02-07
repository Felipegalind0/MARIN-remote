#include "M5StickCPlus.h"
#include "systeminit.h"
#include "speaker.h"
#include "IO.h"
#include "LCD.h"
#include "wireless.h"
#include "COMS.h"
#include "JoyC.h"
#include "variables.h"

void SysInit_Setup(void){

    //disableCore0WDT();
    
    M5.begin();
    Wire.begin(0, 26);  // SDA,SCL
    LCD_Setup();

    serial_Init();

    pinMode(LED, OUTPUT);
    pinMode(SPEAKER, OUTPUT);

    //ledcSetup(LED_CH, 5000, 8);
    ledcSetup(SPEAKER_CH, 5000, 8);
    //ledcAttachPin(LED, LED_CH);
    ledcAttachPin(SPEAKER, SPEAKER_CH);
    
    digitalWrite(LED, HIGH);
    Serial.println("");
    Serial.println("Starting Up Systems");
    // Serial.print("Setup() running on core ");
    // Serial.println(xPortGetCoreID());
    digitalWrite(LED, LOW);
    
    //LCD_Setup();

    StartUp_Sound();

    digitalWrite(LED, HIGH);
    Wireless_Setup();
    digitalWrite(LED, LOW);

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

    

    Serial.println("@SysInit_Setup COMPLETE");
}


void SysInit_Check(void){
    digitalWrite(LED, HIGH);
    Serial.println("");
    Serial.println("StartUp Complete!");
    digitalWrite(LED, LOW);
}