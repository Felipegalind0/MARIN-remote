#include <M5StickCPlus.h>
#include "variables.h"
#include "gyro.h"
#include "IO.h"
#include "LCD.h"
#include <WebSerial.h>


void imuInit() {
    M5.Imu.Init();
    // if (M5.Imu.imuType = M5.Imu.IMU_MPU6886) {
    //     M5.Mpu6886.SetGyroFsr(
    //         M5.Mpu6886.GFS_250DPS);  // 250DPS 500DPS 1000DPS 2000DPS
    //     M5.Mpu6886.SetAccelFsr(M5.Mpu6886.AFS_4G);  // 2G 4G 8G 16G
    //     if (serialMonitor) Serial.println("MPU6886 found");
    // } else if (serialMonitor)
    //     Serial.println("MPU6886 not found");

    M5.Imu.SetGyroFsr(
    M5.Imu.GFS_250DPS);  // 250DPS 500DPS 1000DPS 2000DPS
    M5.Imu.SetAccelFsr(M5.Imu.AFS_4G);  // 2G 4G 8G 16G
    if (serialMonitor) WebSerial.println("MPU6886 found");
}

void getGyro() {
    readGyro();
    varOmg = (gyroYdata - gyroYoffset);           // unit:deg/sec
    yawAngle += (gyroZdata - gyroZoffset) * clk;  // unit:g
    varAng += (varOmg + ((accXdata - accXoffset) * 57.3 - varAng) * cutoff) *
              clk;  // complementary filter
}

void readGyro() {
  // Declare variables for gyro and accelerometer data
  float gX, gY, gZ, aX, aY, aZ;

  // Get the gyro data for the X, Y, and Z axes
  M5.Imu.getGyroData(&gX, &gY, &gZ);

  // Get the accelerometer data for the X, Y, and Z axes
  M5.Imu.getAccelData(&aX, &aY, &aZ);

  // Assign the gyro data to the corresponding global variables
  // (Note the sign changes to match the robot's coordinate system)
  gyroYdata = gX;
  gyroZdata = -gY;
  gyroXdata = -gZ;

  // Assign the accelerometer data to the corresponding global variables
  // (Note the change in variable usage for aX)
  accXdata = aZ;
  accYdata = aX; // New assignment to use the previously unused variable aX
  accZdata = aY;
}

// First Calibration
void calib1() {
    calDelay(30);
    //digitalWrite(LED, HIGH);
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
    //digitalWrite(LED, LOW);
}



void calDelay(int n) {
    for (int i = 0; i < n; i++) {
        getGyro();
        vTaskDelay(9);
    }
}
