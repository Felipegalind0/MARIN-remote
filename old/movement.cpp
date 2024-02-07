#include <M5StickCPlus.h>
#include "variables.h"
#include "movement.h"
#include "IO.h"
#include "LCD.h"
#include <WebSerial.h>


// Update the rotation rate and direction based on the input rotation value
void Movement_UpdateRotation(int rotation) {
  moveRate = 0.0;
  spinContinuous = false;
  spinStep = 0.0;

  // Normalize the rotation value to be within the range of -1.0 and 1.0
  float normalizedRotation = rotation / 10.0;

  // Set the movement rate based on the normalized rotation value
  moveRate = abs(normalizedRotation);

  // Set the spinContinuous flag to true if the rotation value is non-zero
  spinContinuous = rotation != 0;

  // Set the spinStep based on the normalized rotation value and the clock value
  spinStep = -40.0 * normalizedRotation * clk;
}

// Update the movement rate and direction based on the input movement value
void Movement_UpdateMovement(int movement) {
  // Normalize the movement value to be within the range of -2.0 and 2.0
  float normalizedMovement = movement / 5.0;

  // Set the movement rate based on the normalized movement value
  moveRate = normalizedMovement;
}


// Main Loop 
void Movement_Loop() {
    
    #ifdef DEBUG
        if (debugLoop1()) return;
    #endif

    getGyro();

    if (!standing) { // If Robot is not Standing

        aveAbsOmg = aveAbsOmg * 0.9 + abs(varOmg) * 0.1;
        aveAccZ   = aveAccZ * 0.9 + accZdata * 0.1;

        

        // 0.9*90= 72
        //if Pointed 72 deg up 
        if (abs(aveAccZ) > 0.9 && aveAbsOmg < 1.5) {

            // Run Second Calibration
            LCD_calib2_Message();
            calib2();

            if (demoMode == 1) startDemo();

            standing = true;
        }
    } 

    else { // Check if Robot has fallen and disable

        if (abs(varAng) > maxAngle) {
            Abort("Max DEG");
        } 

        else if (counterOverPwr > maxOvp) { 
            Abort("Max PWR");
        } 

        else { //Robot is okay to drive

            drive(); // What makes it move
        }

    }

}


void report_DEG_Abort(){
    LCD_Abort_DEG_Message();
}

void report_PWR_Abort(){
    LCD_Abort_PWR_Message();
}

void Abort(String MSG){

    LCD_Abort_Message();

    if(MSG == "Max DEG") report_DEG_Abort();

    if(MSG == "Max PWR") report_PWR_Abort();

    resetMotor();

    WebSerial.print(MSG);

    standing = false;
    hasFallen = true;

    while(!abortWasHandled){
        vTaskDelay(500);
        ledcWriteTone(SPEAKER_CH,911);
        digitalWrite(LED, HIGH);
        CheckButtons();
        vTaskDelay(50);
        digitalWrite(LED, LOW);
        ledcWriteTone(SPEAKER_CH,0);

        if (Abtn) {
            abortWasHandled = true;
            break;
        };
    }

    LCD_Resume_from_Abort_Message();
    vTaskDelay(1000);


    Movement_Setup();
    
}


    

// First Calibration
void calib1() {
    calDelay(30);
    digitalWrite(LED, HIGH);
    calDelay(80);
    LCD_calib1_Message();
    gyroYoffset = 0.0;
    for (int i = 0; i < N_CAL1; i++) {
        readGyro();
        gyroYoffset += gyroYdata;
        vTaskDelay(9);
    }
    gyroYoffset /= (float)N_CAL1;
    M5.Lcd.fillScreen(BLACK);
    digitalWrite(LED, LOW);
}

// Second Calibration
void calib2() {
    resetVar();
    resetMotor();
    digitalWrite(LED, HIGH);
    calDelay(80);
    M5.Lcd.setCursor(30, LCDV_MID);
    M5.Lcd.println(" Cal-2  ");
    accXoffset  = 0.0;
    gyroZoffset = 0.0;
    for (int i = 0; i < N_CAL2; i++) {
        readGyro();
        accXoffset += accXdata;
        gyroZoffset += gyroZdata;
        vTaskDelay(9);
    }
    accXoffset /= (float)N_CAL2;
    gyroZoffset /= (float)N_CAL2;
    M5.Lcd.fillScreen(BLACK);
    digitalWrite(LED, LOW);
}



void calDelay(int n) {
    for (int i = 0; i < n; i++) {
        getGyro();
        vTaskDelay(9);
    }
}

void startDemo() {
    moveRate       = 1.0;
    spinContinuous = true;
    spinStep       = -40.0 * clk;
}





void drive() {
#ifdef DEBUG
    debugDrive();
#endif
    if (abs(moveRate) > 0.1)
        spinFact = constrain(-(powerR + powerL) / 10.0, -1.0, 1.0);  // moving
    else
        spinFact = 1.0;  // standing
    if (spinContinuous)
        spinTarget += spinStep * spinFact;
    else {
        if (spinTarget < spinDest) spinTarget += spinStep;
        if (spinTarget > spinDest) spinTarget -= spinStep;
    }
    moveTarget += moveStep * (moveRate + (float)fbBalance / 100.0);
    varSpd += power * clk;
    varDst += Kdst * (varSpd * clk - moveTarget);
    varIang += KIang * varAng * clk;
    power =
        varIang + varDst + (Kspd * varSpd) + (Kang * varAng) + (Komg * varOmg);
    if (abs(power) > 1000.0)
        counterOverPwr += 1;
    else
        counterOverPwr = 0;
    if (counterOverPwr > maxOvp) return;
    power    = constrain(power, -maxPwr, maxPwr);
    yawPower = (yawAngle - spinTarget) * Kyaw;
    powerR   = power - yawPower;
    powerL   = power + yawPower;

    ipowerL      = (int16_t)constrain(powerL * mechFactL, -maxPwr, maxPwr);
    int16_t mdbn = -motorDeadband;
    int16_t pp2n = -punchPwr2;
    if (ipowerL > 0) {
        if (motorLdir == 1)
            punchCountL = constrain(++punchCountL, 0, 100);
        else
            punchCountL = 0;
        motorLdir = 1;
        if (punchCountL < punchDur)
            drvMotorL(max(ipowerL, punchPwr2));
        else
            drvMotorL(max(ipowerL, motorDeadband));
    } else if (ipowerL < 0) {
        if (motorLdir == -1)
            punchCountL = constrain(++punchCountL, 0, 100);
        else
            punchCountL = 0;
        motorLdir = -1;
        if (punchCountL < punchDur)
            drvMotorL(min(ipowerL, pp2n));
        else
            drvMotorL(min(ipowerL, mdbn));
    } else {
        drvMotorL(0);
        motorLdir = 0;
    }

    ipowerR = (int16_t)constrain(powerR * mechFactR, -maxPwr, maxPwr);
    if (ipowerR > 0) {
        if (motorRdir == 1)
            punchCountR = constrain(++punchCountR, 0, 100);
        else
            punchCountR = 0;
        motorRdir = 1;
        if (punchCountR < punchDur)
            drvMotorR(max(ipowerR, punchPwr2));
        else
            drvMotorR(max(ipowerR, motorDeadband));
    } else if (ipowerR < 0) {
        if (motorRdir == -1)
            punchCountR = constrain(++punchCountR, 0, 100);
        else
            punchCountR = 0;
        motorRdir = -1;
        if (punchCountR < punchDur)
            drvMotorR(min(ipowerR, pp2n));
        else
            drvMotorR(min(ipowerR, mdbn));
    } else {
        drvMotorR(0);
        motorRdir = 0;
    }
}

void resetMotor() {
    drvMotorR(0);
    drvMotorL(0);
    counterOverPwr = 0;
}


