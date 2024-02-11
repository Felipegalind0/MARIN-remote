#include "variables.h"
#include "LCD.h"
#include <WebSerial.h>



float vBatt, voltAve             = -1.0;

float vBatt_min = 3.3, vBatt_max = 4.1;

float deviceTemp = -1.0;

int perCentBatt = -1;

boolean isCharging = false;

boolean debug_core0 = true;
boolean debug_core1 = false;

// Variables and stuff
boolean serialMonitor   = true;
boolean standing        = false;
// boolean hasFallen       = false;
boolean abortWasHandled = false;

boolean is_paired = false;
boolean is_booted = false;

boolean WiFi_connected = false;

boolean WiFi_With_Remote_Name_Found = false;

boolean robot_wifi_in_range = false;

//boolean Should_Init_WiFi = false;

boolean WiFi_Is_Initialized = false;

boolean WiFi_Is_Initializing = false;

byte Warn_User_WiFi_Will_Be_Init = 0;
byte Warn_User_WiFi_Will_Be_Init_Threshold = 100;

boolean Warn_User_WiFi_Will_Be_Init_Selector_Abort = 0;




String exec_status = "OFF";
boolean exec_status_has_changed = false;



uint8_t lcd_brightness = 12;



uint16_t counter       = 0;
uint32_t time0 = 0, time1 = 0;
int16_t counterOverPwr = 0, maxOvp = 80, maxAngle = 30;
float power, powerR, powerL, yawPower;
float varAng, varOmg, varSpd, varDst, varIang;
float gyroXoffset, gyroYoffset, gyroZoffset, accXoffset;
float gyroXdata, gyroYdata, gyroZdata, accXdata, accYdata, accZdata;
float aveAccX = 0.0, aveAccZ = 0.0, aveAbsOmg = 0.0;
float cutoff            = 0.1;                     //~=2 * pi * f (Hz)
const float clk         = 0.01;                    // in sec,
const uint32_t interval = (uint32_t)(clk * 1000);  // in msec
float Kang, Komg, KIang, Kyaw, Kdst, Kspd;
int16_t maxPwr;
float yawAngle = 0.0;
float moveDestination, moveTarget;
float moveRate        = 0.0;
const float moveStep  = 0.2 * clk;
int16_t fbBalance     = 0;
int16_t motorDeadband = 0;
float mechFactR, mechFactL;
int8_t motorRDir = 0, motorLDir = 0;

bool spinContinuous = false;
float spinDest, spinTarget, spinFact = 1.0;
float spinStep  = 0.0;  // deg per 10msec
int16_t ipowerL = 0, ipowerR = 0;
int16_t motorLdir = 0, motorRdir = 0;  // 0:stop 1:+ -1:-

int16_t punchPwr, punchPwr2, punchDur, punchCountL = 0, punchCountR = 0;

uint32_t Joyc_X_min = 500, Joyc_X_center = 2000, Joyc_X_max = 3500, Joyc_Y_min = 500, Joyc_Y_center = 2000, Joyc_Y_max = 3500, JoyC_X_raw = 2000, JoyC_Y_raw = 2000, JoyC_X_raw_prev = 2000, JoyC_Y_raw_prev = 2000;

byte JoyC_X_deadzone = 200, JoyC_Y_deadzone = 200;

byte JoyC_X = 50, JoyC_Y = 50;

boolean JoyC_btn = false;

float JoyC_r = 0;

float JoyC_Phi = 0;

byte JoyC_X_Cycles_In_Deadzone = 0, JoyC_Y_Cycles_In_Deadzone = 0;

byte demoMode = 0;

byte Abtn = 0;
byte Bbtn = 0;
byte Pbtn = 0;

boolean pairRequested = false;



// Organized Variables


//-----------------Joystick Variables-----------------
boolean JoyC_In_X_DeadZone = true;
boolean JoyC_In_y_DeadZone = true;

int JoyC_X_left_right = 0;
int JoyC_Y_up_down    = 0;

boolean JoyC_left = false;
boolean JoyC_right = false;
boolean JoyC_up = false;
boolean JoyC_down = false;

boolean JoyC_needs_to_return_to_center = false;

boolean JoyC_Xinput = false;

//-----------------Sleep Variables-----------------
int sleep_enter_timer = 0;
int sleep_exit_timer  = 0;

boolean is_sleeping = false;


//-----------------Robot Variables-----------------
// These are the 2 important global variables that are linked by variables.h

int x = 0;  // Defines robot rotation rate    + = R     &   - = L
int y = 0;  // Defines robot FWD/BACK         + = FWD   &   - = BACK


Vector<Network> WiFi_Networks = {};

int n_WiFi_Networks = 0;



//-----------------Time Variables-----------------
int64_t RealTcode_start_time = 0, RealTcode_end_time = 0, RealTcode_execution_time = 0, RealTcode_no_execution_time = 0, RealTcode_total_execution_time = 0;
int64_t BackgroundTask_execution_time_start = 0, BackgroundTask_execution_time_end = 0, BackgroundTask_execution_time = 0, BackgroundTask_total_execution_time = 0, BackgroundTask_no_execution_time = 0;

double RealTcode_CPU_load = 0.0, BackgroundTask_CPU_load = 0.0;


void resetVar() {
    abortWasHandled = false;
    power          = 0.0;
    moveTarget     = 0.0;
    moveRate       = 0.0;
    spinContinuous = false;
    spinDest       = 0.0;
    spinTarget     = 0.0;
    spinStep       = 0.0;
    yawAngle       = 0.0;
    varAng         = 0.0;
    varOmg         = 0.0;
    varDst         = 0.0;
    varSpd         = 0.0;
    varIang        = 0.0;
}

void resetPara() {
    Kang          = 37.0;
    Komg          = 0.84;
    KIang         = 800.0;
    Kyaw          = 4.0;
    Kdst          = 85.0;
    Kspd          = 2.7;
    mechFactL     = 0.45;
    mechFactR     = 0.45;
    punchPwr      = 20;
    punchDur      = 1;
    fbBalance     = -3;
    motorDeadband = 10;
    maxPwr        = 120;
    punchPwr2     = max(punchPwr, motorDeadband);
}

// -------Functions that should be run on second core-------

// // Update mode 
// void setMode(bool inc) {
//     if (inc) demoMode = ++demoMode % 2;

//     LCD_Update_Mode();
// }

float mapfloat(float x, float in_min, float in_max, float out_min, float out_max)
{
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

boolean print_web_serial_not_initialized = false;
// Update & show Battery Voltage On Display
void updateBatVolt(){

    //Serial.println("");

    //Serial.print("isCharging = M5.Axp.GetVBusVoltage() > 4.2");

    isCharging = M5.Axp.GetVBusVoltage() > 4.2;

    //Serial.print("isChargeing = ");
    //Serial.println(isCharging);

    //Serial.println("deviceTemp = M5.Axp.GetTempInAXP192();");

    deviceTemp = M5.Axp.GetTempInAXP192();


    //Serial.println("WebSerial");

    if (WebSerial.availableForWrite()) {
        Serial.println("WebSerial INITIALIZED");
        WebSerial.println(" Battery Voltage: " + String(voltAve) + "V" + " | " + String(perCentBatt) + "%");
        
    }
    else {
        if (print_web_serial_not_initialized) {
            Serial.println("WebSerial NOT INITIALIZED");
        }
        
    }

    
    if (counter > 100) {
    //if(vBatt < 0.0) { // If the battery voltage is not available
        //vBatt = M5.Axp.GetVBusVoltage();
        vBatt = M5.Axp.GetBatVoltage();
        voltAve = vBatt; // Set the average voltage to the current voltage
        
    } else {
        //vBatt = M5.Axp.GetVBusVoltage();
        vBatt = M5.Axp.GetBatVoltage();
        voltAve = (voltAve * 0.5) + (vBatt * 0.5);
    }


    if (vBatt < vBatt_min) {

        vBatt_min = vBatt;

        Serial.println("NEW vBatt_min = " + String(vBatt_min));
    
    }
    else if (vBatt > vBatt_max) {

        //vBatt_max = vBatt;

        //Serial.println("NEW vBatt_max = " + String(vBatt_max));
    }
    
    //perCentBatt = map(vBatt, vBatt_min, vBatt_max, 0, 100);

    if (counter < 100){
        perCentBatt = -1;
    }
    else {
        perCentBatt = mapfloat(voltAve, vBatt_min, vBatt_max, 0, 100);
        if (perCentBatt > 100) perCentBatt = 100;
        else if (perCentBatt < 0) perCentBatt = -1;
    }

    

    //isCharging = M5.Axp.GetBatChargeCurrent() > 0;


}
