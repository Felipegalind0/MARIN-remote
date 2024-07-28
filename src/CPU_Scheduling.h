
#ifndef CPU_SCHEDULING_H
#define CPU_SCHEDULING_H


// #include "systeminit.h"
// #include "IO.h"
// //#include "movement.h"
// #include "gyro.h"
// #include "variables.h"
// #include "LCD.h"
// #include "COMS.h"
// #include "Wireless.h"
// //#include <Vector.h>
// #include <Wire.h>
// #include "JoyC.h"

// #include "esp_timer.h"

// #include <esp32-hal-cpu.h>


// // Including FreeRTOS headers for TaskHandle_t and SemaphoreHandle_t
// #include "FreeRTOS.h"
// #include "task.h"
// #include "semphr.h"
#include <Arduino.h>


extern TaskHandle_t Task0, Task1;

extern SemaphoreHandle_t syncSemaphore;

#endif // CPU_SCHEDULING_H