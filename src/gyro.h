#ifndef _GYRO_H
#define _GYRO_H

#include <M5StickCPlus.h>

void imuInit();

void getGyro();

void readGyro();

void calib1();

void calDelay(int n);


#endif