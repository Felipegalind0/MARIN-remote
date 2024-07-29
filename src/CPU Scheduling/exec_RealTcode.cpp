#include "exec_RealTcode.h"


boolean print_RealTcode_start_time = false;

boolean print_Warn_User_WiFi_Will_Be_Init = false;

#define DEBUG_robot_menu_active 1

void dim_screen() {
  M5.Axp.ScreenBreath(lcd_brightness-4);
}

void un_dim_screen() {
  M5.Axp.ScreenBreath(lcd_brightness);
}


void enter_sleep() {

  Serial.println("@enter_sleep: ");

  update_status("Sleeping", WHITE);

  dim_screen();

  is_sleeping = true;
  sleep_exit_timer = 0;
  sleep_enter_timer = 0;

  // Set CPU Frequency to 80MHz
  setCpuFrequencyMhz(80);
  Serial.println("@enter_sleep: CPU Frequency set to " + String(getCpuFrequencyMhz()));

}

void exit_sleep() {
  is_sleeping = false;
  update_status("ON", GREEN);
  Serial.println("@exit_sleep: ON");

  un_dim_screen();

  sleep_exit_timer = 0;
  sleep_enter_timer = 0;

  setCpuFrequencyMhz(240);
  Serial.println("@exit_sleep: CPU Frequency set to " + String(getCpuFrequencyMhz()));
}

void processAbtn() {

    if(M5.BtnA.wasPressed()){
      Serial.println("Button A was pressed");
      Abtn = 1;



      if(menu_active){ // if the menu is active

      #define WIFI_MENU_SCAN 1


        if (g_menu_X_selector == ROBOT_MENU){

          if(menu_Y_selector == ROBOT_MENU_TOOGLE_ARM){
            //robot_ARM_requested = true;
            if (robot_state == ROBOT_DISARMED){
              robot_state = ROBOT_ARMING;
              set_JoyC_LED_color(TFT_GREEN);
            }
            else {
              robot_state = ROBOT_DISARMING;
              set_JoyC_LED_color(DEEP_SKY_BLUE);
            }
            
            menu_active = false;
            JoyC_Xinput = false;
            LCD_flush();
          }

          if(menu_Y_selector == ROBOT_MENU_TAKEOFF){
            //robot_ARM_requested = true;
            robot_state = TAKEOFF_REQUESTED;
            
            menu_active = false;
            JoyC_Xinput = false;
            LCD_flush();
          }

        }
        else if (g_menu_X_selector == WIFI_NETWORKS){

          if(menu_Y_selector == WIFI_MENU_SCAN){
            //robot_ARM_requested = true;
            //robot_state = TAKEOFF_REQUESTED;
            setPairingState(PAIRING_REQUESTED);
            
            menu_active = false;
            JoyC_Xinput = false;
            LCD_flush();
          }
          else if(-menu_Y_selector < n_WiFi_Networks){ // if clicked on a network
            g_menu_selected_WiFi = WiFi.SSID(-menu_Y_selector);
            g_menu_X_selector = WIFI_MENU;
            menu_Y_selector = 0;
          }


        }
        else if (g_menu_X_selector == WIFI_MENU){

            if(menu_Y_selector == WIFI_SHOW_INFO){
                //TODO
                ;
            }
            else if (menu_Y_selector == WIFI_MENU_CONNECT_DISCONNECT){
                //TODO
                ;
            }
            else if (menu_Y_selector == WIFI_SET_NUM){
                g_menu_X_selector = SET_NUM_MENU;
                menu_Y_selector = -1;

            }
          


        }

        else{
          menu_active = false;
          JoyC_Xinput = false;
          LCD_flush();
        }

      }

      
      // else if menu is not active an
      else if (WiFi_State != WIFI_INITIALIZED && WiFi_State != WIFI_INITIALIZING && Warn_User_WiFi_Will_Be_Init < Warn_User_WiFi_Will_Be_Init_Threshold){
        if (Warn_User_WiFi_Will_Be_Init_Selector_Abort){
          Warn_User_WiFi_Will_Be_Init = 0;
          LCD_flush();
        }
        else{
          Warn_User_WiFi_Will_Be_Init = 100;
        }
      }


      else{ // if the menu is not active, activate it
        menu_active = true;
        JoyC_Xinput = true;
        LCD_flush();
      }
      
      #ifdef DEBUG_robot_menu_active
        Serial.print("menu_active: ");
        Serial.println(menu_active);
      #endif

      // if (robot_connected){

        
        
      // }

    }
    else {
      Abtn = 0;
    }
}

void exec_RealTcode() {

    // Serial.print("WiFi_Is_Initialized: ");
    // Serial.print(WiFi_Is_Initialized);
    // Serial.print(" Warn_User_WiFi_Will_Be_Init: ");
    // Serial.print(Warn_User_WiFi_Will_Be_Init);
    // Serial.print(" Warn_User_WiFi_Will_Be_Init_Threshold: ");
    // Serial.println(Warn_User_WiFi_Will_Be_Init_Threshold);

    // print GPIO35
    // Serial.print("GPIO35: ");
    // Serial.println(digitalRead(35)); DOES NOT WORK

    if (!(is_booted)) { // If Device is not booted, run the setup code
      
      // Initialize web serial, I2C, LCD, speaker, microphone, and other systems
      SysInit_Setup();

      // Initialize movement systems, motors, gyro, etc.
      // Movement_Setup();
      return;
    }

    else if (is_booted) { // If Device is booted, run the main code
    }

    // If the device is not booted and threshold has not been reached, increase the counter so that the warning is displayed
    //if (!WiFi_Is_Initialized && !WiFi_Is_Initializing && Warn_User_WiFi_Will_Be_Init < Warn_User_WiFi_Will_Be_Init_Threshold)
    if (WiFi_State != WIFI_INITIALIZED && WiFi_State != WIFI_INITIALIZING && Warn_User_WiFi_Will_Be_Init < Warn_User_WiFi_Will_Be_Init_Threshold){

      if (!Warn_User_WiFi_Will_Be_Init_Selector_Abort){
        Warn_User_WiFi_Will_Be_Init++;
      }
      else{
        Warn_User_WiFi_Will_Be_Init = 1;
      }
      
      //Warn_User_WiFi_Will_Be_Init = 1; // uncomment to disable warning timeout
      
      //enable JoyC_Xinput
      JoyC_Xinput = true;

      if (print_Warn_User_WiFi_Will_Be_Init){
        Serial.print("Warn_User_WiFi_Will_Be_Init: ");
        Serial.println(Warn_User_WiFi_Will_Be_Init);
      }
      
    }
    // if (WiFi_connected) {
    //   //Serial.println("WiFi connected");
    //   update_status("WiFi connected");
    // }
    // else {
    //   //Serial.println("WiFi not connected");
    //   update_status("WiFi not connected");
    // }



    if (print_RealTcode_start_time){
      Serial.println("\n@RealTcode: start_time = " + String(RealTcode_start_time));
    }


    

    if ((WiFi_State == WIFI_INITIALIZED) && Pairing_State == PAIRING_NOT_REQUESTED){ //&& WiFi_Is_Initialized) {
    //if (!(is_paired && WiFi_State == WIFI_INITIALIZED) && !pairFailed){ //&& WiFi_Is_Initialized) {
      // pairRequested = true;
      // Pairing_State = PAIRING_REQUESTED;
      setPairingState(PAIRING_REQUESTED);
    }

    M5.update();

    JoyC_loop();

    LCD_loop();

    //Serial.println("menu_active: " + String(menu_active) + " robot_ARM_requested: " + String(robot_ARM_requested));

    if (robot_connected) {
        sendData();
    }

    if(M5.BtnA.pressedFor(1000)){
      Serial.println("Button A was pressed for 1 second");

    }
    else {
   
    }
    processAbtn();

    if(M5.BtnB.wasPressed()){
      Serial.println("Button B was pressed");
      Bbtn = 1;
    }
    else {
      Bbtn = 0;
    }



    if ((counter % btnCounter) == 0) {
      //Serial.print("Checking buttons");
      CheckButtons();
    }

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

        if (sleep_exit_timer > 1){
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

        if (sleep_enter_timer > 100 && robot_connected){

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