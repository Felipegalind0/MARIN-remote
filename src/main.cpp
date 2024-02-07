/*
    BalaC balancing robot with IMU:MPU6886 

By Felipe Galindo



This code is based of the example BalaC with the following changes:

  - Improved Controller

      General Improvements to the PID, position estimation and balanceing 
      systems that enable the robot to be controlled at a high level. 

      Robot will stand up and minimize movement, then can be controlled by 2
      numbers. X = Rotation, Y = Foward/Backward.

      Both X and Y are numbers betweel -10 and+10


    * X represents (L/R) rotation rate on the robot. 

      By setting it to 0 robot will attempt to mantain heading. 

      Positive numbers make it turn right. Negatives make it turn left. 


    * Y represents (FWD/BACK) advance rate on the robot. 

      At 0 robot attempts to stay upright and stationary.

      Positive numbers make it move foward. Negative makes it move backwards. 


  - ESPNOW communications & remote control:
      Enables to robot to be remotely controlled by other robot or a human 
      with a remote controller. 

  - WebSerial: 
      Robot will create it's own WiFi Network and attempt connect to a 
      pre-defined network on which the Serial is avaliable at <ip-address>/webserial
      and at ? on the robot hosted WiFi

  - FreeRTOS Optimizations: 
      The arduino framework comes with a Default implementation of freRTOS.

      We optimized code 1 to run as a 'real-time' core, running code that 
      needs to be exectuted with tight deadlines or bad stuff will happen.
      Here we run code that 

      Core 0 is used by FreeRTOS for wireless COM & other background stuff. 
      A lot of the time this core is not busy so background tasks are delegated to
      this core whenever possible. 


Robot Setup instructions:

1. Lay the robot flat, and power on. DO NOT MOVE THE ROBOT AT ALL. 
    Robot will Display Startup Message, and "Executing Stationary Calibration" in red
    If you moved the robot please press the PWR/RESET button, to restart device. 

2. Wait until you see a "Stationary Calibration Complete" message. DO NOT MOVE ROBOT UNTIL COMPLETE
    The Stationary calibration is important, and your robot will not work if not calibrated properly.

3. Shortly after the first calibration is done Wireless Status when calibration is complete. 
    You may now move the robot if you'd like. On the display of the robot and the Physical serial interface(USB serial)
    you will see the IP address of the extenal network the robot connected to.
    
    WebSerial is now avaliable at <ip-address>/webserial
    and at ? on the robot hosted WiFi

3. Complete Balance calibration. 
    Your robot has an angle at which it balances itself. We need to find it to be able to
    make the robot stand up and not fall over. 

    Hold the robot at as close as possible to the balancing point. 

    Wait until the motors start up and the robot is balacing itself, then quickly let go. 

    Robot should stay upright. 

4. Web Camera
    After your robot is standing up the external web camera module should start to stream images. 
    Note that the WebCam module runs separate code on a separate micro controller.
    It might take longer or shorter for the WebCam to start
    The two systems are independent, this code runs on ESP 32, which hopefully will be able to communicate in the future and 
    receive instructions from the external camera but that is not added yet.

5. Remote controller 
    This robot can receive comments from a ESPN now enabled device. I haven't designed code for the M5stack MiniJoyC.
    This is a Joystick ESP32 MCU, and it sends commands formated like this:

    c-00-00 This is the 'zeroed' meesage, it includes '-' even through 0 is not negative for readability

    c-03-05 This would mean   x = -3  y = -5

    c+03-05 This would mean   x = 3  y = -5

    c-03+05 This would mean   x = -3  y = 5

    c+03+05 This would mean   x = 3  y = 5

    c means command, I might setup other letters to do other stuff

     -11 < (Both X & Y) < 11
        AKA -10 to 10 


short push of power button: Gyro calibration

long push (>1sec) of power button: switch mode between standig and demo(circle)

*/

#include "systeminit.h"
#include "IO.h"
//#include "movement.h"
#include "gyro.h"
#include "variables.h"
#include "LCD.h"
#include "COMS.h"
#include "Wireless.h"
#include <Vector.h>
#include <Wire.h>
#include "JoyC.h"

#include "esp_timer.h"

#include <esp32-hal-cpu.h>


TaskHandle_t Task0, Task1;

SemaphoreHandle_t syncSemaphore;

void update_status(String status) {
  exec_status = status;
  exec_status_has_changed = true;
  Serial.println("\n@update_status: " + status);
}

void enter_sleep() {
  is_sleeping = true;
  update_status("Sleeping");
  Serial.println("@enter_sleep: Sleeping");
  M5.Axp.ScreenBreath(8);

  sleep_exit_timer = 0;
  sleep_enter_timer = 0;

  setCpuFrequencyMhz(80);
  Serial.println("@enter_sleep: CPU Frequency set to " + String(getCpuFrequencyMhz()));
}

void exit_sleep() {
  is_sleeping = false;
  update_status("ON");
  Serial.println("@exit_sleep: ON");
  M5.Axp.ScreenBreath(12);

  sleep_exit_timer = 0;
  sleep_enter_timer = 0;

  setCpuFrequencyMhz(240);
  Serial.println("@exit_sleep: CPU Frequency set to " + String(getCpuFrequencyMhz()));
}

// Setup Code
void Movement_Setup() {

    //StartUp IMU
    imuInit();

    //Zero Motors
    //resetMotor();

    // Reset to Default Parameters
    resetPara();

    //Zero Out Variables
    resetVar();

    // Run Calibration1
    LCD_calib1_Message();
    calib1();
    LCD_calib1_complete_Message();



#ifdef DEBUG
    debugSetup();
#else
    setMode(false);
#endif
}

void exec_RealTcode() {

    if (!(is_booted)) { // If Device is not booted, run the setup code
      
      // Initialize web serial, I2C, LCD, speaker, microphone, and other systems
      SysInit_Setup();

      // Initialize movement systems, motors, gyro, etc.
      // Movement_Setup();

      if (WiFi_connected) {
        //Serial.println("WiFi connected");
        update_status("WiFi connected");
      }
      else {
        //Serial.println("WiFi not connected");
        update_status("WiFi not connected");
      }

      Serial.println("is_booted = true");
      is_booted = true;
      return;
    }
    else {
      //Serial.println("\n@RealTcode: start_time = " + String(RealTcode_start_time));
    }


    if (!(is_paired)) { 
      Serial.println("@RealTcode: is_paired = false; Starting pairing process");
      // Check for pairing
      update_status("Scanning WiFi");
      Vector<Network> networks = get_ssids();

      Serial.println("@RealTcode: Pairing process complete; is_paired = true");
      update_status("Pairing DONE");
      is_paired = true;
      return;
    }

    M5.update();


    if(M5.BtnA.wasPressed()){
      Serial.println("Button A was pressed");
      Abtn = 1;
    }
    if(M5.BtnB.wasPressed()){
      Serial.println("Button B was pressed");
      Bbtn = 1;
    }

    JoyC_loop();

    // Serial.print("X_dz: ");
    // Serial.print(JoyC_X_Cycles_In_Deadzone);
    // Serial.print(" Y_dz: ");
    // Serial.print(JoyC_Y_Cycles_In_Deadzone);
    // Serial.print(" is_sleeping: ");
    // Serial.print(is_sleeping);
    // Serial.print(" sleep_enter_timer: ");
    // Serial.print(sleep_enter_timer);
    // Serial.print(" sleep_exit_timer: ");
    // Serial.println(sleep_exit_timer);

    if (is_sleeping){ // if sleeping, check if we should exit sleep mode
      sleep_enter_timer = 0;

      if ((JoyC_In_X_DeadZone) && (JoyC_In_y_DeadZone)){

        sleep_exit_timer = 0;

      }
      else{

        if (sleep_exit_timer > 10){
          exit_sleep();
          sleep_exit_timer = 0;
        }
        else{
          sleep_exit_timer++;
        }

        
      }

      return;

    }
    else{ // if not sleeping, check if we should enter sleep mode

      if ((JoyC_In_X_DeadZone) && (JoyC_In_y_DeadZone)){

        if (sleep_enter_timer > 100){

          enter_sleep();
        }
        else{
          sleep_enter_timer++;
        }

      }
      else{
        sleep_enter_timer = 0;

      }

    }

    //Movement_Loop(); // Call the movement loop for balancing and motion control

    
}


// Function that contains code that needs to run in real-time
void RealTcode( void * pvParameters ){ 
  for(;;){ // Infinite loop for continuous execution

    RealTcode_no_execution_time_end = esp_timer_get_time(); // Record the end time of the loop

    //RealTcode_start_time = micros(); // Record the start time of the loop
    RealTcode_start_time = esp_timer_get_time();; // Record the start time of the loop


    // Execute the real-time code
    exec_RealTcode();

    //RealTcode_end_time = micros(); // Record the end time of the loop
    RealTcode_end_time = esp_timer_get_time(); // Record the end time of the loop

    //Serial.print("@RealTcode: end_time = ");
    //Serial.println(RealTcode_end_time);

    // Calculate the time taken to execute the loop
    RealTcode_execution_time = RealTcode_end_time - RealTcode_start_time;

    //Serial.print("@RealTcode: Execution time = ");
    //Serial.println(RealTcode_execution_time);
    //Serial.println();

    counter += 1; // Increment the counter variable by 1 each iteration
    

    RealTcode_no_execution_time_start = esp_timer_get_time(); // Record the start time of the loop

    // Give the semaphore to allow background tasks to run
    xSemaphoreGive(syncSemaphore);

    yield(); // Yield the processor to other tasks

    // Delay the task for a specific interval (in milliseconds) to control execution frequency
    vTaskDelay(pdMS_TO_TICKS(interval));

    RealTcode_no_execution_time = RealTcode_no_execution_time_end - RealTcode_no_execution_time_start; // Calculate the time not spent executing the loop

    RealTcode_total_execution_time = RealTcode_execution_time + RealTcode_no_execution_time;

    if (RealTcode_total_execution_time != 0 && RealTcode_execution_time != 0) {

      // Calculate the CPU load
      RealTcode_CPU_load = (RealTcode_execution_time / RealTcode_total_execution_time) * 100;


      Serial.print("@RealTcode_total_execution_time: ");
      Serial.print(RealTcode_total_execution_time);
      Serial.print(" @RealTcode_execution_time: ");
      Serial.print(RealTcode_execution_time);
      Serial.print(" @RealTcode: CPU load = ");
      Serial.println(RealTcode_CPU_load);

    }
    
  }
}

void BackgroundTask( void * pvParameters ) {
  for (;;) {  // Infinite loop for background task
    yield(); // Yield the processor to other tasks
    // Wait for the syncSemaphore to be given by the RealTcode task
    if ((xSemaphoreTake(syncSemaphore, portMAX_DELAY) == pdTRUE) && (is_booted)) {
      //Serial.print("@BackgroundTask: ");

      // 
      if ((counter % btnCounter) == 0) {
        //Serial.print("Checking buttons");
        CheckButtons();
      }

      // 
      if (counter % logCounter == 0) {
        if (serialMonitor) {
          //Serial.print("Logging data");
          sendStatus();
          logData();
        }
      }

      vTaskDelay(pdMS_TO_TICKS(interval));

      //Serial.println("Updating LCD");
      // Update the LCD display
      LCD_loop();
    }
    //Serial.println("Yielding to other tasks");
    // Yield the processor to other tasks
    yield();    
  }
}









void setup() {
  // Initialize various systems

  // Uncomment the line below to set the CPU frequency (if needed)
  // setCpuFrequencyMhz(512);

  // Create a binary semaphore for task synchronization
  syncSemaphore = xSemaphoreCreateBinary();

  // Create a task for real-time code execution
  // Task1code() function, with priority 1, executed on core 1
  xTaskCreatePinnedToCore(
                    RealTcode,   /* Task function. */
                    "RealTtask",     /* name of task. */
                    10000,       /* Stack size of task */
                    NULL,        /* parameter of the task */
                    1,           /* priority of the task */
                    &Task1,      /* Task handle to keep track of created task */
                    RealTcore);          /* pin task to core 1 */

  // Add a delay to give the task some time to start
  vTaskDelay(100); 

  // Create a background task for less time-sensitive operations
  // Executed on core 0 with lower priority than the real-time task
  xTaskCreatePinnedToCore(
                      BackgroundTask,   /* Task function. */
                      "BackgroundTask",          /* name of task. */
                      10000,            /* Stack size of task */
                      NULL,             /* parameter of the task */
                      -1,               /* priority of the task (lower than RealTcode) */
                      NULL,             /* Task handle to keep track of created task */
                      BackgroundCore);               /* pin task to core 0 */

  // Add another delay to give the background task some time to start
  vTaskDelay(100);

  // Validate that the system started up correctly
  SysInit_Check();
}



void loop() {
  // Delete this task to save resources 
  vTaskDelete(NULL);
  // Serial.print("loop() running on core ");
  // Serial.println(xPortGetCoreID());
  //Movement_Loop();
}