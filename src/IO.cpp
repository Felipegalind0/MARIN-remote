#include <WebSerial.h>
#include "variables.h"
#include "speaker.h"
#include "IO.h"

void RED_LED(bool state){
    digitalWrite(LED, !state);
}

//Check if button has been pressed
void CheckButtons() {
    Pbtn = M5.Axp.GetBtnPress();
    //byte Abtn = 
    // M5.BtnA.read();
    // M5.BtnB.read();


    if (Pbtn == 2){ // short push

        // Shutdown_Sound();

        // esp_restart();

    }
        
        
    else if (Pbtn == 1){  //long push
        //setMode(true);  // long push
    }

    // if (Abtn){
    //   Serial.println("A Button Pressed");
    // }

    // if (Bbtn){
    //   Serial.println("B Button Pressed");
    // }

    if (Pbtn == 2){
      Serial.println("Power Button Pressed");
    }
    else if (Pbtn == 1){
      Serial.println("Power Button Long Pressed");
    }

}

// void drvMotorL(int16_t pwm) {
//     drvMotor(0, (int8_t)constrain(pwm, -127, 127));
// }

// void drvMotorR(int16_t pwm) {
//     drvMotor(1, (int8_t)constrain(-pwm, -127, 127));
// }

// void drvMotor(byte ch, int8_t sp) {
//     Wire.beginTransmission(0x38);
//     Wire.write(ch);
//     Wire.write(sp);
//     Wire.endTransmission();
// }