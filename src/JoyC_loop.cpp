
#include "JoyC_loop.h"

int Y_selector_MAX = 0;
void JoyC_loop(){
    //joyc.update();

    JoyC_X_raw = joyc.getADCValue(0);

    JoyC_Y_raw = joyc.getADCValue(1);

    JoyC_btn = !joyc.getButtonStatus();


    cartesianToPolar(&JoyC_X, &JoyC_Y, &JoyC_r, &JoyC_Phi);
    //print_JoyC_raw_values();

    //print_JoyC_center_values();

    boolean should_print_raw = false;
    boolean should_print_min_max = false;
    boolean should_print_center = false;
    boolean should_print_mapped = false;

    boolean should_recalculate_X_center = false;
    boolean should_recalculate_Y_center = false;

    int delta_X = (JoyC_X_raw - Joyc_X_center);

    int abs_delta_X =  abs(delta_X);

    if (JoyC_X_raw < Joyc_X_raw_min) { // if the raw X value is less than the minimum value
        JoyC_In_X_DeadZone = false;

        Joyc_X_raw_min = JoyC_X_raw;

        JoyC_X = 1;
    }

    else if (JoyC_X_raw > Joyc_X_max) { // if the raw X value is greater than the maximum value
        JoyC_In_X_DeadZone = false;    
    
        Joyc_X_max = JoyC_X_raw;

        JoyC_X = 99;
    }

    else if(abs_delta_X < JoyC_X_deadzone){ // if the raw X value is within the deadzone
        JoyC_In_X_DeadZone = true;
        //Serial.print("dz_x ");
        JoyC_X = 50;

        int delta_X_prev = (JoyC_X_raw - JoyC_X_raw_prev);

        byte abs_delta_X_prev =  abs(delta_X_prev);

        if (abs_delta_X_prev < 10){

            JoyC_X_Cycles_In_Deadzone++;

            if (JoyC_X_Cycles_In_Deadzone > 100){
                should_recalculate_X_center = true;
                JoyC_X_Cycles_In_Deadzone = 0;
            }
            
        }
            
        else {
            JoyC_X_Cycles_In_Deadzone = 0;
        }
    }

    else { // if the raw X value is outside the deadzone and within the min and max values
        JoyC_In_X_DeadZone = false;
         JoyC_X = map(JoyC_X_raw, Joyc_X_raw_min, Joyc_X_max, 1, 99);
    }


    int delta_Y = (JoyC_Y_raw - Joyc_Y_center);

    int abs_delta_Y =  abs(delta_Y);


    if (JoyC_Y_raw < Joyc_Y_raw_min) { // if the Y raw value is less than the minimum value
        JoyC_In_y_DeadZone = false;
        Joyc_Y_raw_min = JoyC_Y_raw;

        JoyC_Y = 1;

    }

    else if (JoyC_Y_raw > Joyc_Y_raw_max) { // if the raw Y value is greater than the maximum value
        JoyC_In_y_DeadZone = false;
        Joyc_Y_raw_max = JoyC_Y_raw;

        JoyC_Y = 99;

    }

    else if (abs_delta_Y < JoyC_Y_deadzone){ // if the raw Y value is within the deadzone
        JoyC_In_y_DeadZone = true;
        //Serial.print("dz_y");

        JoyC_Y = 50;

        int delta_Y_prev = (JoyC_Y_raw - JoyC_Y_raw_prev);

        byte abs_delta_Y_prev = abs(delta_Y_prev);

        if (abs_delta_Y_prev < 10){ // if difference between the current and previous Y values is less than 10

            JoyC_Y_Cycles_In_Deadzone++;

            if (JoyC_Y_Cycles_In_Deadzone > 100){
                // average the center value
                should_recalculate_Y_center = true;
                JoyC_Y_Cycles_In_Deadzone = 0;
            }
            
        }
            
        else {
            JoyC_Y_Cycles_In_Deadzone = 0;
        }
        


    }

    else { // if the raw Y value is outside the deadzone and within the min and max values
        JoyC_In_y_DeadZone = false;
         JoyC_Y = map(JoyC_Y_raw, Joyc_Y_raw_min, Joyc_Y_raw_max, 1, 99);
    }



    

    boolean print_JoyC_Up_Down_Left_Right = false;


    if (JoyC_Xinput){
        // determine left/right and up/down
        if (JoyC_X < 25){

            if (JoyC_X_left_right == 0){ // if the X value was previously in the deadzone
                //JoyC_left = true;

                JoyC_X_left_right =  JoyC_selector_LEFT;



                if (print_JoyC_Up_Down_Left_Right){
                    Serial.print("left");
                }
            }
            //JoyC_X_left_right = -1;
        }
        else if (JoyC_X > 75){

            if (JoyC_X_left_right == 0){ // if the X value was previously in the deadzone
                //JoyC_right = true;
                JoyC_X_left_right =  JoyC_selector_RIGHT;
                if (print_JoyC_Up_Down_Left_Right){
                    Serial.print("right");
                }
            }

            //JoyC_X_left_right = 1;
        }
        else {
            JoyC_X_left_right = 0;
        }

        if (JoyC_Y < 25){

            if (JoyC_Y_up_down == 0){ // if the Y value was previously in the deadzone
                //JoyC_down = true;

                JoyC_Y_up_down =  JoyC_selector_DOWN;
                if (print_JoyC_Up_Down_Left_Right){
                    Serial.print("down");
                }
            }
            //JoyC_Y_up_down = -1;
        }
        else if (JoyC_Y > 75){

            if (JoyC_Y_up_down == 0){   // if the Y value was previously in the deadzone
                //JoyC_up = true;
                JoyC_Y_up_down =  JoyC_selector_UP;
                if (print_JoyC_Up_Down_Left_Right){
                    Serial.print("up");
                }
            }

            //JoyC_Y_up_down = 1;
        }
        else {
            JoyC_Y_up_down = 0;
        }

        // if both in deadzne
        if (JoyC_In_X_DeadZone && JoyC_In_y_DeadZone){
        //if (JoyC_X_left_right == 0 && JoyC_Y_up_down == 0){
            JoyC_needs_to_return_to_center = false;
        }


        if (Warn_User_WiFi_Will_Be_Init && WiFi_State == WIFI_NOT_INITIALIZED &&
        ( JoyC_X_left_right || JoyC_Y_up_down) && !JoyC_needs_to_return_to_center){
            // Serial.print("WAbort: ");
            // Serial.print(Warn_User_WiFi_Will_Be_Init_Selector_Abort);
            
            // if (print_JoyC_Up_Down_Left_Right){

            //     Serial.print(" Jup:");
            //     Serial.print(JoyC_up);

            //     Serial.print(" Jdown:");
            //     Serial.print(JoyC_down);

            //     Serial.print(" Jleft:");
            //     Serial.print(JoyC_left);

            //     Serial.print(" Jright:");
            //     Serial.print(JoyC_right);

            //     Serial.print(" Jntrtc:");
            //     Serial.print(JoyC_needs_to_return_to_center);

            //     Serial.println();
            // }
        //(JoyC_UP_DOWN_selector || JoyC_LEFT_RIGHT_selector)){
        // ( || JoyC_down || JoyC_left || JoyC_right) && 
        // !JoyC_needs_to_return_to_center){
            // Serial.print("WAbort: ");
            // Serial.print(Warn_User_WiFi_Will_Be_Init_Selector_Abort);
            
            // if (print_JoyC_Up_Down_Left_Right){
            //     Serial.print(" Jup:");
            //     Serial.print(JoyC_up);

            //     Serial.print(" Jdown:");
            //     Serial.print(JoyC_down);

            //     Serial.print(" Jleft:");
            //     Serial.print(JoyC_left);

            //     Serial.print(" Jright:");
            //     Serial.print(JoyC_right);

            //     Serial.print(" Jntrtc:");
            //     Serial.print(JoyC_needs_to_return_to_center);

            //     Serial.println();
            // }



            Warn_User_WiFi_Will_Be_Init_Selector_Abort = !Warn_User_WiFi_Will_Be_Init_Selector_Abort;
            Warn_User_WiFi_Will_Be_Init = 0;


            // JoyC_up = false;
            // JoyC_down = false;
            // JoyC_left = false;
            // JoyC_right = false;

            JoyC_needs_to_return_to_center = true;

            
            Serial.println("JoyC_needs_to_return_to_center = 1 Warn_User_WiFi_Will_Be_Init:" + String(Warn_User_WiFi_Will_Be_Init));
            
           
        } // if the Init WiFi Warning is active and the user moves the joystick, toggle the selector

        //Serial.println("menu_active: " + String(menu_active) + " JoyC_needs_to_return_to_center: " + String(JoyC_needs_to_return_to_center) + " JoyC_X_left_right: " + String(JoyC_X_left_right) + " JoyC_Y_up_down: " + String(JoyC_Y_up_down) + " g_menu_X_selector: " + String(g_menu_X_selector) + " menu_Y_selector: " + String(menu_Y_selector) + " robot_state: " + String(robot_state) + " robot_msg: " + robot_msg);

        else if(menu_active && !JoyC_needs_to_return_to_center){
            
            if ( JoyC_Y_up_down == JoyC_selector_UP){
                JoyC_Y_up_down = JoyC_selector_CENTER;
                JoyC_needs_to_return_to_center = true;
            //if (JoyC_up){


                switch (g_menu_X_selector){
                    case ROBOT_MENU:
                        Y_selector_MAX = Robot_menu_max_Y;
                        break;

                    case WIFI_NETWORKS_MENU:
                        Y_selector_MAX = WIFI_NETWORKS_MENU_Y_MAX;
                        break;
                    case WIFI_MENU:
                        Y_selector_MAX = WIFI_MENU_Y_MAX;
                        break;
                    case SET_NUM_MENU:
                        Y_selector_MAX = SET_NUM_MENU_Y_MAX;
                        break;
                }
                if (g_menu_Y_selector < Y_selector_MAX){
                // if (menu_Y_selector < Robot_menu_max_Y          && g_menu_X_selector == ROBOT_MENU    ||
                //     menu_Y_selector < WIFI_NETWORKS_MENU_Y_MAX  && g_menu_X_selector == WIFI_NETWORKS ||
                //     menu_Y_selector < WIFI_MENU_Y_MAX           && g_menu_X_selector == WIFI_MENU     ||
                //     menu_Y_selector < SET_NUM_MENU_Y_MAX        && g_menu_X_selector == SET_NUM_MENU
                //     ){

                    g_menu_Y_selector++;

                    Serial.println("menu_Y_selector: " + String(g_menu_Y_selector));

                    xTaskCreatePinnedToCore(
                    Click_Sound, /* Task function. */
                    "Click_Sound", /* Name of the task */
                    10000,      /* Stack size in words */
                    NULL,       /* Task input parameter */
                    -2,          /* Priority of the task */
                    NULL,       /* Task handle. */
                    BackgroundCore);  /* Core where the task should run */

                }



                else {
                    Serial.println("JoyC_up, but menu_Y_selector is already at max");

                    xTaskCreatePinnedToCore(
                    Click_DENIED_Sound, /* Task function. */
                    "Click_DENIED_Sound", /* Name of the task */
                    10000,      /* Stack size in words */
                    NULL,       /* Task input parameter */
                    -2,          /* Priority of the task */
                    NULL,       /* Task handle. */
                    BackgroundCore);  /* Core where the task should run */
                }
            }
            else if (JoyC_Y_up_down == JoyC_selector_DOWN){
                JoyC_Y_up_down = JoyC_selector_CENTER;
                JoyC_needs_to_return_to_center = true;
                if (g_menu_Y_selector > Robot_menu_min_Y && g_menu_X_selector == ROBOT_MENU ||
                    //menu_Y_selector > 0                && g_menu_X_selector == WIFI_MENU  ){ // we want negative for proper layout
                    g_menu_Y_selector > 1-n_WiFi_Networks && g_menu_X_selector == WIFI_NETWORKS_MENU||
                    g_menu_Y_selector > WIFI_MENU_Y_MIN   && g_menu_X_selector == WIFI_MENU   ||
                    g_menu_Y_selector > SET_NUM_MENU_Y_MIN && g_menu_X_selector == SET_NUM_MENU
                    ){
                    g_menu_Y_selector--;

                    Serial.println("menu_Y_selector: " + String(g_menu_Y_selector));

                    // xTaskCreatePinnedToCore(
                    // Click_Sound, /* Task function. */
                    // "Click_Sound", /* Name of the task */
                    // 10000,      /* Stack size in words */
                    // NULL,       /* Task input parameter */
                    // -2,          /* Priority of the task */
                    // NULL,       /* Task handle. */
                    // BackgroundCore);  /* Core where the task should run */
                    //scheduleSoundTask(Click_Sound, "Click_Sound");
                    scheduleSoundTask(Click_Sound);
                    
                }
                else {
                    Serial.println("JoyC_down, but menu_Y_selector is already at min");

                    xTaskCreatePinnedToCore(
                    Click_DENIED_Sound, /* Task function. */
                    "Click_DENIED_Sound", /* Name of the task */
                    10000,      /* Stack size in words */
                    NULL,       /* Task input parameter */
                    -2,          /* Priority of the task */
                    NULL,       /* Task handle. */
                    BackgroundCore);  /* Core where the task should run */
                }
            }



            else if (JoyC_X_left_right == JoyC_selector_LEFT){
                JoyC_X_left_right = JoyC_selector_CENTER;
                JoyC_needs_to_return_to_center = true;
                if (g_menu_X_selector == WIFI_MENU){
                    g_menu_X_selector = WIFI_NETWORKS_MENU;
                }
                else if (g_menu_X_selector > Robot_menu_min_X ){
                    g_menu_X_selector--;
                    
                    // make sure the Y selector is within the bounds of the menu
                    if (g_menu_X_selector == ROBOT_MENU){
                        if (g_menu_Y_selector > Robot_menu_max_Y){
                            g_menu_Y_selector = Robot_menu_max_Y;
                        }
                    }
                    else if (g_menu_X_selector == WIFI_NETWORKS_MENU){
                        if (g_menu_Y_selector > n_WiFi_Networks){
                            g_menu_Y_selector = n_WiFi_Networks;
                        }
                    }
                }
            }
            else if (JoyC_X_left_right == JoyC_selector_RIGHT){
                JoyC_X_left_right = JoyC_selector_CENTER;
                JoyC_needs_to_return_to_center = true;
                if (g_menu_X_selector < Robot_menu_max_X){
                    g_menu_X_selector++;

                    // make sure the Y selector is within the bounds of the menu
                    if (g_menu_X_selector == ROBOT_MENU){
                        if (g_menu_Y_selector > Robot_menu_max_Y){
                            g_menu_Y_selector = Robot_menu_max_Y;
                        }
                    }
                    else if (g_menu_X_selector == WIFI_NETWORKS_MENU){
                        if (g_menu_Y_selector > n_WiFi_Networks){
                            g_menu_Y_selector = n_WiFi_Networks;
                        }
                    }
                }
            }
            else {
                //Serial.println("L/R UP/DOWN MENU ERROR");
            }
        }

    }



    
    if (should_print_min_max){
        print_JoyC_min_max_values();
    }

    if (should_print_center){
        print_JoyC_center_values();
    }

    if (should_print_mapped){
        print_JoyC_mapped_values();
    }
    
    if (should_recalculate_X_center){
        recalculate_X_center();
    }
    
    if(should_recalculate_Y_center){
        recalculate_Y_center();
    }

    JoyC_X_raw_prev = JoyC_X_raw;
    JoyC_Y_raw_prev = JoyC_Y_raw;
}