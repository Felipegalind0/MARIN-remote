#ifndef CPU_SCHEDULING_H
#define CPU_SCHEDULING_H

#include <Arduino.h>


extern TaskHandle_t Task0, Task1;

extern SemaphoreHandle_t syncSemaphore;


#endif // CPU_SCHEDULING_H