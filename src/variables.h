#ifndef _VARIABLES_H_
#define _VARIABLES_H_

#define btnCounter 10
#define logCounter 100


#include <M5StickCPlus.h>
#include <Preferences.h>
//#include <Vector.h>



//-----------------System Variables-----------------


#define RealTcore 1
#define BackgroundCore 0

struct Network {
  String ssid;
  String mac;
  int rssi;
};

extern byte robot_state;

enum ROBOT_ARMED_STATUS {
   UNKNOWN_ARMED_STATUS, ROBOT_DISARMED, ROBOT_DISARMING, ROBOT_ARMING, ROBOT_ARMED, TAKEOFF_REQUESTED, TAKING_OFF, LANDING_REQUESTED, LANDING
};

enum Remote_Menu_X_Options {
  REMOTE_MENU = -1, 
  ROBOT_MENU = 0,
  WIFI_NETWORKS = 1,
  WIFI_MENU = 111,
  SET_NUM_MENU = 120
};

enum Robot_Menu_Y_Options {
  ROBOT_MENU_LAND = -1, 
  ROBOT_MENU_TOOGLE_ARM = 0,
  ROBOT_MENU_TAKEOFF = 1
};

enum Wifi_States {
  WIFI_NOT_INITIALIZED, WIFI_INITIALIZING, WIFI_INITIALIZED, WIFI_CONNECTED, WIFI_DISCONNECTED, WIFI_DEACTIVATED
};

enum Pairing_States {
  PAIRING_NOT_REQUESTED, SCANNING_SSIDS,
  PAIRING_REQUESTED, PAIRING_FAILED, NO_ROBOTS_FOUND, ROBOT_NUM_NOT_FOUND, MULTIPLE_ROBOTS_FOUND, DUPLICATE_REMOTE_FOUND, PAIRING_SUCCESSFUL
};

#define WIFI_NETWORKS_MENU_Y_MAX 1

enum WIFI_MENU_OPTIONS {
  WIFI_SET_NUM = -1,
  WIFI_MENU_SCAN = 0,
  WIFI_MENU_CONNECT_DISCONNECT = 1,
  WIFI_SHOW_INFO = 2,
};
#define WIFI_MENU_Y_MIN WIFI_SET_NUM
#define WIFI_MENU_Y_MAX WIFI_SHOW_INFO
#define SET_NUM_MENU_Y_MIN -1
#define SET_NUM_MENU_Y_MAX 16

extern uint8_t broadcastAddress[6]; // Array to store the MAC address


//-----------------Robot IMU Variables-----------------
extern float robot_X_deg, robot_Y_deg, robot_Z_deg;
extern float Avg_IMU_X_deg_per_sec, Avg_IMU_Y_deg_per_sec, Avg_IMU_Z_deg_per_sec;

//-----------------Robot Motor Variables-----------------
extern int Rmotor, Lmotor;
extern boolean isArmed;

# define Robot_menu_min_Y -1

# define Robot_menu_max_Y 1

# define Robot_menu_min_X -1

# define Robot_menu_max_X 1

extern int g_menu_X_selector, menu_Y_selector;

extern int menu_min_X, menu_max_X, menu_min_Y, menu_max_Y;

extern String robot_msg;

extern boolean menu_active; // robot_ARM_requested, robot_DISARM_requested;

extern boolean isCharging;

extern boolean debug_core0, debug_core1;

extern float deviceTemp;

extern int x, y;

extern int n_WiFi_Networks;

extern String g_menu_selected_WiFi;

extern boolean g_set_num_remote_too;


extern float vBatt, voltAve;

extern float vBatt_min, vBatt_max;

extern int perCentBatt;

extern int Robot_perCentBatt;

extern boolean standing, hasFallen, abortWasHandled, is_paired, is_booted, JoyC_In_X_DeadZone, JoyC_In_y_DeadZone;


// extern boolean WiFi_Is_Initialized;

extern boolean WiFi_With_Remote_Name_Found;


// extern boolean WiFi_Is_Initializing;

// extern boolean WiFi_Just_Finished_Initializing;

extern byte Warn_User_WiFi_Will_Be_Init, Warn_User_WiFi_Will_Be_Init_Threshold;

extern boolean Warn_User_WiFi_Will_Be_Init_Selector_Abort;

extern byte WiFi_State;

extern byte Pairing_State;


// extern boolean WiFi_connected;
extern String exec_status;
extern int exec_status_color;
extern boolean exec_status_has_changed;

extern boolean robot_wifi_in_range;

extern boolean robot_connected;

extern uint8_t lcd_brightness;

extern boolean JoyC_Xinput;

extern int JoyC_X_left_right, JoyC_Y_up_down;

extern int sleep_enter_timer, sleep_exit_timer;

extern boolean is_sleeping;

enum JoyC_Y_selector_states {
  JoyC_selector_UP = 1,
  JoyC_selector_CENTER = 0,
  JoyC_selector_DOWN = -1
};
enum JoyC_X_selector_states {
  JoyC_selector_LEFT = -1,
  JoyC_selector_RIGHT = 1
  
};

extern boolean  JoyC_needs_to_return_to_center; 
//extern boolean JoyC_left, JoyC_right, JoyC_up, JoyC_down, JoyC_needs_to_return_to_center;
// extern byte JoyC_UP_DOWN_selector;
// extern byte JoyC_LEFT_RIGHT_selector;



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

extern double Robot_RealTcode_CPU_load, Robot_BackgroundTask_CPU_load;

extern bool spinContinuous;
extern float spinDest, spinTarget, spinFact;
extern float spinStep;
extern int16_t ipowerL, ipowerR;
extern int16_t motorLdir, motorRdir;
extern int16_t punchPwr, punchPwr2, punchDur, punchCountL, punchCountR;

extern uint32_t Joyc_X_raw_min, Joyc_X_center, Joyc_X_max, Joyc_Y_raw_min, Joyc_Y_center, Joyc_Y_raw_max, JoyC_X_raw, JoyC_Y_raw, JoyC_X_raw_prev, JoyC_Y_raw_prev;



extern byte JoyC_X_deadzone, JoyC_Y_deadzone;

extern byte JoyC_X_Cycles_In_Deadzone, JoyC_Y_Cycles_In_Deadzone;

extern byte JoyC_X, JoyC_Y;

extern byte Robot_JoyC_X, Robot_JoyC_Y;

extern float Robot_JoyC_r, Robot_JoyC_Phi;

extern float JoyC_r, JoyC_Phi;

extern boolean JoyC_btn;

extern byte demoMode;
extern byte Abtn;
extern byte Bbtn;
extern byte Pbtn;

extern boolean initMSG_has_been_flushed;

// extern boolean pairRequested;
// extern boolean pairFailed;



void updateBatVolt();
void setMode(bool inc);


void resetVar();
void resetPara();

void setPairingState(byte state);

void update_status(String status, int color);





#endif // _VARIABLES_H_
