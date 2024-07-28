
#include "exec_BackgroundTask.h"


void exec_BackgroundTask() {
  if ((xSemaphoreTake(syncSemaphore, portMAX_DELAY) == pdTRUE) && (is_booted)) { // check that syncSemaphore is avaliable and the device is booted
    //Serial.print("@BackgroundTask: ");


    // 
    // if (counter % logCounter == 0) {
    //   if (serialMonitor) {
    //     Serial.print("Logging data");
    //     sendStatus();
    //     logData();
    //   }
    // }

    if (Warn_User_WiFi_Will_Be_Init >= Warn_User_WiFi_Will_Be_Init_Threshold) {

      //WiFi_Is_Initializing = true;
      WiFi_State = WIFI_INITIALIZING;
      JoyC_Xinput = false;
      
      Warn_User_WiFi_Will_Be_Init = 0;  
      Serial.println("\nWireless_Setup()");

      // xTaskCreatePinnedToCore(
      //     Wireless_Setup,   /* Function to implement the task */
      //     "Wireless_Setup", /* Name of the task */
      //     10000,      /* Stack size in words */
      //     NULL,       /* Task input parameter */
      //     -2,          /* Priority of the task */
      //     NULL,       /* Task handle. */
      //     BackgroundCore);  /* Core where the task should run */


      //digitalWrite(LED, HIGH);
      //yield();  // Yield the processor to other tasks so no other FreeRTOS wireless tasks are pending
      Wireless_Setup();
      //yield(); // Yield the processor to other tasks so no other FreeRTOS wireless tasks are pending

      

      return;
    }


    if ((Pairing_State == PAIRING_REQUESTED) && WiFi_State == WIFI_INITIALIZED){
    //if (pairRequested && WiFi_State == WIFI_INITIALIZED){  //WiFi_Is_Initialized) {

      Serial.println("Pairing_State = PAIRING_REQUESTED\nWiFi_State = WIFI_INITIALIZED");
      // Check for pairing
      update_status("Scan WiFi", BLUE);

      get_ssids();

      //print_WiFi_Networks();

      // Serial.print("Pairing process complete; is_paired = ");
      // Serial.println((is_paired ? "true" : "false"));
      // pairRequested = false;
      return;
    }

    

    //Serial.println("Updating LCD");
    // Update the LCD display
    //Serial.println("@exec_BackgroundTask: DONE");
  }

}