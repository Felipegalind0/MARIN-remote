#include "BackgroundTask.h"

void BackgroundTask( void * pvParameters ) {
  for (;;) {  // Infinite loop for background task


    BackgroundTask_execution_time_start = esp_timer_get_time(); // Record the start time of the loop

    BackgroundTask_no_execution_time = BackgroundTask_execution_time_start - BackgroundTask_execution_time_end; // Calculate the time not spent executing the loop

    yield(); // Yield the processor to other tasks
    // Wait for the syncSemaphore to be given by the RealTcode task

    exec_BackgroundTask(); // Execute the background task

    //Serial.println("@BackgroundTask: exec DONE");

    yield(); // Yield the processor to other tasks


    // Record the end time of the loop
    BackgroundTask_execution_time_end = esp_timer_get_time();

    // Calculate the time taken to execute the loop
    BackgroundTask_execution_time = BackgroundTask_execution_time_end - BackgroundTask_execution_time_start;

    // Calculate the total execution time and CPU load
    BackgroundTask_total_execution_time = BackgroundTask_execution_time + BackgroundTask_no_execution_time;

    if (BackgroundTask_total_execution_time != 0 && BackgroundTask_execution_time != 0) {

      // Calculate the CPU load
      BackgroundTask_CPU_load = (BackgroundTask_execution_time * 100/ BackgroundTask_total_execution_time);

      // Serial.print("@BackgroundTask_no_execution_time_start: ");
      // Serial.print(BackgroundTask_no_execution_time_start);
      // Serial.print(" @BackgroundTask_no_execution_time_end: ");
      // Serial.print(BackgroundTask_no_execution_time_end);

      // Serial.print("@BackgroundTask_no_execution_time: "); 
      // Serial.print(BackgroundTask_no_execution_time);
      // Serial.print(" @BackgroundTask_total_execution_time: ");
      // Serial.print(BackgroundTask_total_execution_time);
      // Serial.print(" @BackgroundTask_execution_time: ");
      // Serial.print(BackgroundTask_execution_time);
      // Serial.print(" @BackgroundTask: CPU load = ");
      // Serial.println(BackgroundTask_CPU_load);

    }

    vTaskDelay(pdMS_TO_TICKS(interval)); // Delay the task for a specific interval (in milliseconds) to control execution frequency  
  }
}