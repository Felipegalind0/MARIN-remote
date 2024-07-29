#include "LCD_loop.h"

boolean print_LCD_Loop = false;

boolean print_updating_battery_voltage = false;

void LCD_loop(){
    

    if (print_LCD_Loop){
        Serial.println("LCD_Print_JoyC_widget()");
    }

    if (JoyC_Xinput){
        
        LCD_Print_JoyC_widget();
        
    }


    LCD_Status_Message();

    // LCD_CPU_Widget();

    LCD_CPU_Widget(35, 220, BackgroundTask_CPU_load, RealTcode_CPU_load);

    

    if (print_LCD_Loop){
        Serial.println("LCD_Print_JoyC_widget() DONE");
    }


    if (menu_active){
        if (initMSG_has_been_flushed){
            LCD_Menu();
        }
        else{
            LCD_flush();
            initMSG_has_been_flushed = true;
        }
    }


    else if (robot_connected) {


        if (JoyC_Xinput){
                
        }

        else{ // if Xinput is disabled

            if (initMSG_has_been_flushed){ //make sure screen has been flushed before displaying the Artificial Horizon
                LCD_Western_Artificial_Horizon();
                LCD_CPU_Widget(33, 122, Robot_BackgroundTask_CPU_load, Robot_RealTcode_CPU_load);
                LCD_DispBatVolt(0, 122, Robot_perCentBatt, isCharging);
            }
            else{
                LCD_flush();
                initMSG_has_been_flushed = true;
            }   
        }

        
    }


    
    // if (print_RealT_Times){

    //     LCD_Print_RealT_Times();
    // }



    if ((counter % 269) == 0) {
    //if (true) {
        if (print_updating_battery_voltage){
            Serial.print("Updating Battery Voltage: ");
        }
        //LCD_flush();
        
        updateBatVolt();

        if(print_updating_battery_voltage){
            Serial.print("LCD_loop() running on core ");
            Serial.println(xPortGetCoreID());
        }

        
    }
//    else if ((counter % 169) == 0) {

//         LCD_flush();
//     }



    // if (!(is_paired)) {
    //     //Serial.println("Showing Pairing Message");
    //     LCD_Pairing_Message();
    // }

    // if (Warn_User_WiFi_Will_Be_Init && !WiFi_Is_Initializing && !WiFi_Is_Initialized) {
    if (Warn_User_WiFi_Will_Be_Init && WiFi_State != WIFI_INITIALIZING && WiFi_State != WIFI_INITIALIZED) {
        LCD_Warn_WiFi_Message();
    }
    // else if (WiFi_Is_Initializing && !WiFi_Is_Initialized) {
    else if (WiFi_State == WIFI_INITIALIZING) {
        LCD_WiFi_Initializing_Message();
        Warn_User_WiFi_Will_Be_Init = false;
    }
    else if (Pairing_State == SCANNING_SSIDS){
        LCD_WiFi_Scanning_Message();
    }
    else if (WiFi_State == WIFI_CONNECTED){
        
    }
    else if (WiFi_State == WIFI_DISCONNECTED){
        
    }
    else if (WiFi_State == WIFI_NOT_INITIALIZED){
        
        
    }


    //LCD_DispAngle();

    //Serial.println("LCD_Status_Message()");




    LCD_DispBatVolt(0, 217, perCentBatt, isCharging);



   // LCD_print_JoyC_Debug();

    //Serial.println("pushSprite()");
    canvas.pushSprite(0, 0);
}