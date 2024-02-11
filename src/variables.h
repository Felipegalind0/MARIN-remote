#ifndef _VARIABLES_H_
#define _VARIABLES_H_

#define btnCounter 10
#define logCounter 100

#define RealTcore 1
#define BackgroundCore 0

#include <M5StickCPlus.h>
#include <Vector.h>

struct Network {
  String ssid;
  String mac;
  int rssi;
};

extern boolean isCharging;

extern boolean debug_core0, debug_core1;

extern float deviceTemp;





extern int x, y;

extern Vector<Network> WiFi_Networks;

extern int n_WiFi_Networks;

extern float vBatt, voltAve;

extern float vBatt_min, vBatt_max;

extern int perCentBatt;

extern boolean standing, hasFallen, abortWasHandled, is_paired, is_booted, JoyC_In_X_DeadZone, JoyC_In_y_DeadZone;


extern boolean WiFi_Is_Initialized;


extern boolean WiFi_Is_Initializing;

extern byte Warn_User_WiFi_Will_Be_Init, Warn_User_WiFi_Will_Be_Init_Threshold;

extern boolean Warn_User_WiFi_Will_Be_Init_Selector_Abort;


extern boolean WiFi_connected;
extern String exec_status;
extern boolean exec_status_has_changed;

extern boolean robot_wifi_in_range;

extern uint8_t lcd_brightness;

extern boolean JoyC_Xinput;

extern int JoyC_X_left_right, JoyC_Y_up_down;

extern int sleep_enter_timer, sleep_exit_timer;

extern boolean is_sleeping;

extern boolean JoyC_left, JoyC_right, JoyC_up, JoyC_down, JoyC_needs_to_return_to_center;



extern boolean serialMonitor;
extern uint16_t counter;
extern uint32_t time0, time1;
extern int16_t counterOverPwr, maxOvp, maxAngle;
extern float power, powerR, powerL, yawPower;
extern float varAng, varOmg, varSpd, varDst, varIang;
extern float gyroXoffset, gyroYoffset, gyroZoffset, accXoffset;
extern float gyroXdata, gyroYdata, gyroZdata, accXdata, accYdata, accZdata;
extern float aveAccX, aveAccZ, aveAbsOmg;
extern float cutoff;
extern const float clk;
extern const uint32_t interval;
extern float Kang, Komg, KIang, Kyaw, Kdst, Kspd;
extern int16_t maxPwr;
extern float yawAngle;
extern float moveDestination, moveTarget;
extern float moveRate;
extern const float moveStep;
extern int16_t fbBalance;
extern int16_t motorDeadband;
extern float mechFactR, mechFactL;
extern int8_t motorRDir, motorLDir;


extern int64_t RealTcode_start_time, RealTcode_end_time, RealTcode_execution_time, BackgroundTask_execution_time_start, BackgroundTask_execution_time_end, BackgroundTask_execution_time, BackgroundTask_total_execution_time, BackgroundTask_no_execution_time, RealTcode_no_execution_time, RealTcode_total_execution_time;
extern double RealTcode_CPU_load, BackgroundTask_CPU_load;


extern bool spinContinuous;
extern float spinDest, spinTarget, spinFact;
extern float spinStep;
extern int16_t ipowerL, ipowerR;
extern int16_t motorLdir, motorRdir;
extern int16_t punchPwr, punchPwr2, punchDur, punchCountL, punchCountR;

extern uint32_t Joyc_X_min, Joyc_X_center, Joyc_X_max, Joyc_Y_min, Joyc_Y_center, Joyc_Y_max, JoyC_X_raw, JoyC_Y_raw, JoyC_X_raw_prev, JoyC_Y_raw_prev;

extern byte JoyC_X_deadzone, JoyC_Y_deadzone;

extern byte JoyC_X_Cycles_In_Deadzone, JoyC_Y_Cycles_In_Deadzone;

extern byte JoyC_X, JoyC_Y;

extern float JoyC_r, JoyC_Phi;

extern boolean JoyC_btn;

extern byte demoMode;
extern byte Abtn;
extern byte Bbtn;

extern boolean pairRequested;



void updateBatVolt();
void setMode(bool inc);


void resetVar();
void resetPara();



#endif // _VARIABLES_H_
