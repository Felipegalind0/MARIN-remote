
#include "LCD.h"


uint16_t invertColor16(uint16_t color) {
    // Invert the color: 0xFFFF is max value for 16-bit, color is the original color
    return 0xFFFF - color;
}

TFT_eSprite canvas = TFT_eSprite(&M5.Lcd);

void LCD_flush(){
    canvas.pushSprite(0, 0);
    canvas.fillSprite(BLACK);
}

boolean print_LCD_Loop = false;

boolean print_updating_battery_voltage = false;

boolean print_RealT_Times = true;

void LCD_Top_1_line_text(String text, byte text_size, int color, byte widget_x, byte widget_y, byte widget_w, byte widget_h, byte widget_r){
    canvas.setTextFont(2);
    canvas.setTextSize(text_size);
    canvas.setTextColor(BLACK);

    // const int ROBOT_MENU_M_X = 5;
    // const int ROBOT_MENU_M_Y = 5;
    // const int ROBOT_MENU_M_W = 127;
    // const int ROBOT_MENU_M_H = 20;
    // const int ROBOT_MENU_M_R = 5;

    // const int ROBOT_MENU_M_COLOR = WHITE;
    // int ROBOT_MENU_M_TEXT_COLOR = invertColor16(ROBOT_MENU_M_COLOR);

    // canvas.fillRoundRect(ROBOT_MENU_M_X, ROBOT_MENU_M_Y,
    //  ROBOT_MENU_M_W, ROBOT_MENU_M_H, ROBOT_MENU_M_R, ROBOT_MENU_M_COLOR);
    
    // canvas.setTextColor(ROBOT_MENU_M_TEXT_COLOR);

    // canvas.setCursor(ROBOT_MENU_M_X+5, ROBOT_MENU_M_Y+2);

    canvas.fillRoundRect(widget_x, widget_y,
     widget_w, widget_h, widget_r, color);

    canvas.setTextColor(invertColor16(color));

    canvas.setCursor(widget_x+5, widget_y+2);



    canvas.print(text);



}

String get_Robot_menu_Y_indicator_str(int index){

    switch (index){
        case ROBOT_MENU_LAND:
            return "     LAND";
        case ROBOT_MENU_TOOGLE_ARM:
            if (robot_state == ROBOT_ARMED){
                return "  DISARM";
            }
            else{
                return "     ARM";
            }
        case ROBOT_MENU_TAKEOFF:
            return "    TAKEOFF";
        default:
            return "";
    }

    // if (index == ROBOT_MENU_LAND){
    //         return "     LAND";
    // }
    // else if (index == ROBOT_MENU_TOOGLE_ARM){
    //     //if (isArmed){
    //     if (robot_state == ROBOT_ARMED){
    //         return "  DISARM";
    //     }
    //     else{
    //         return "     ARM";
    //     }
    // }
    // else if (index == ROBOT_MENU_TAKEOFF){
    //         return "    TAKEOFF";
    // }
    // else{
    //         return "";
    // }
}


# define Robot_Menu_SELECTED_Extra_Space "  "

void LCD_Menu(){
    LCD_Top_1_line_text("    Robot Menu", 1, WHITE, 5, 5, 127, 20, 5);

    String robot_menu_text_buf = get_Robot_menu_Y_indicator_str(robot_menu_Y_selector+1);

    LCD_Top_1_line_text(robot_menu_text_buf, 1, TFT_LIGHTGREY, 15, 30, 107, 20, 10);

    robot_menu_text_buf = Robot_Menu_SELECTED_Extra_Space + get_Robot_menu_Y_indicator_str(robot_menu_Y_selector);

    LCD_Top_1_line_text(robot_menu_text_buf, 1, WHITE, 5, 83, 127, 20, 7);

    robot_menu_text_buf = get_Robot_menu_Y_indicator_str(robot_menu_Y_selector-1);

    LCD_Top_1_line_text(robot_menu_text_buf, 1, TFT_LIGHTGREY, 15, 110, 107, 20, 10);

}


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
            LCD_Menu();
    }


    else if (robot_connected) {
        if (JoyC_Xinput){
                
            }
        else{ // if Xinput is disabled
            LCD_Western_Artificial_Horizon();
            LCD_CPU_Widget(33, 122, Robot_BackgroundTask_CPU_load, Robot_RealTcode_CPU_load);
            LCD_DispBatVolt(0, 122, Robot_perCentBatt, isCharging);
        }

        
    }


    
    // if (print_RealT_Times){

    //     LCD_Print_RealT_Times();
    // }



    if ((counter % 100) == 0) {
    //if (true) {
        if (print_updating_battery_voltage){
            Serial.print("Updating Battery Voltage: ");
        }
        LCD_flush();
        
        updateBatVolt();

        if(print_updating_battery_voltage){
            Serial.print("LCD_loop() running on core ");
            Serial.println(xPortGetCoreID());
        }

        
    }
    if ((counter % 100) == 1) {

        LCD_flush();
    }



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


    //LCD_DispAngle();

    //Serial.println("LCD_Status_Message()");




    LCD_DispBatVolt(0, 217, perCentBatt, isCharging);



   // LCD_print_JoyC_Debug();

    //Serial.println("pushSprite()");
    canvas.pushSprite(0, 0);
}

// void LCD_Print_RealT_Times(){
//     canvas.setTextFont(1);
//     canvas.setTextSize(1);
//     canvas.setTextColor(BLACK);

//     const int REALT_M_X = 0;
//     const int REALT_M_Y = 50;
//     const int REALT_M_W = 90;
//     const int REALT_M_H = 50;
//     const int REALT_M_R = 5;

//     const int REALT_M_COLOR = RED;
//     int REALT_M_TEXT_COLOR = invertColor16(REALT_M_COLOR);

//     canvas.fillRoundRect(REALT_M_X, REALT_M_Y,
//      REALT_M_W, REALT_M_H, REALT_M_R, REALT_M_COLOR);
    
//     canvas.setTextColor(REALT_M_TEXT_COLOR);

//     // print the execution time

//     canvas.setCursor(REALT_M_X+3, REALT_M_Y+5);
//     canvas.print("exec");

//     canvas.setCursor(REALT_M_X+30, REALT_M_Y+5);
//     canvas.print(RealTcode_execution_time);

//     canvas.setCursor(REALT_M_X+70, REALT_M_Y+5);
//     canvas.print("us");


//     // print the no execution time

//     canvas.setCursor(REALT_M_X+3, REALT_M_Y+15);
//     canvas.print("n_ex");

//     canvas.setCursor(REALT_M_X+30, REALT_M_Y+15);
//     canvas.print(RealTcode_no_execution_time);

//     canvas.setCursor(REALT_M_X+70, REALT_M_Y+15);
//     canvas.print("us");

//     // print the total time

//     canvas.setCursor(REALT_M_X+3, REALT_M_Y+25);
//     canvas.print("ex_t");

//     canvas.setCursor(REALT_M_X+30, REALT_M_Y+25);
//     canvas.print(RealTcode_total_execution_time);

//     canvas.setCursor(REALT_M_X+70, REALT_M_Y+25);
//     canvas.print("us");

//     // calculate and print the refresh rate

//     canvas.setCursor(REALT_M_X+3, REALT_M_Y+35);
//     canvas.print("rate");

//     canvas.setCursor(REALT_M_X+35, REALT_M_Y+35);
//     canvas.print(1000000/RealTcode_total_execution_time);

//     canvas.setCursor(REALT_M_X+70, REALT_M_Y+35);
//     canvas.print("hz");



//     canvas.setTextColor(WHITE);
// }

void LCD_Print_JoyC_widget(){
    canvas.setTextFont(1);
    canvas.setTextSize(1);

    int JoyC_widget_X = 0;
    int JoyC_widget_Y = 179;
    const int JoyC_widget_W = 29;
    const int JoyC_widget_H = 42;
    const int JoyC_widget_R = 10;
    const int JoyCircle_IR = 3;
    const int JoyCircle_OC = BLACK;
    
    //define the status box UI variables
    // const int JoyC_widget_X = 10;
    // const int JoyC_widget_Y = 0;
    // const int JoyC_widget_W = 30;
    // const int JoyC_widget_H = 42 ;
    // const int JoyC_widget_R = 10;


    const int COUNTER_M_COLOR = WHITE;
    int COUNTER_M_TEXT_COLOR = invertColor16(COUNTER_M_COLOR);


    canvas.fillRoundRect(JoyC_widget_X, JoyC_widget_Y,
     JoyC_widget_W, JoyC_widget_H, JoyC_widget_R, COUNTER_M_COLOR);
    
    // canvas.setTextColor(COUNTER_M_TEXT_COLOR);

    // canvas.setCursor(JoyC_widget_X+100, JoyC_widget_Y+2);
    // canvas.print(counter);
    
    
    // canvas.setCursor(JoyC_widget_X+40, JoyC_widget_Y+2);
    // canvas.print("us");
    // canvas.setCursor(JoyC_widget_X+55, JoyC_widget_Y+2);
    // canvas.print(RealTcode_execution_time);

    //print CPU load

    // canvas.setCursor(JoyC_widget_X+30, JoyC_widget_Y+2);
    // canvas.print("CPU0:"+String(int(BackgroundTask_CPU_load))+"%"+ " CPU1:"+String(int(RealTcode_CPU_load))+"%");

    

    
    JoyC_widget_X += 14;
    JoyC_widget_Y += 14;
    // const int JoyC_widget_R = 10;
    // const int JoyCircle_IR = 3;
    // const int JoyCircle_OC = BLACK;

    int JoyCircle_IC = TFT_LIGHTGREY;

    if (JoyC_btn){
        JoyCircle_IC = RED;
    }
    else if (JoyC_In_X_DeadZone && JoyC_In_y_DeadZone){
        JoyCircle_IC = TFT_LIGHTGREY;
    }
    else if (JoyC_r > 40){
        JoyCircle_IC = BLACK;
    }
    else{
        JoyCircle_IC = TFT_DARKGREY;
    }
    
    float inner_JoyCircle_X = JoyC_widget_X + ((JoyC_X+1) / 10) -(JoyC_widget_R/2);
    float inner_JoyCircle_Y = JoyC_widget_Y - ((JoyC_Y+1) / 10) +(JoyC_widget_R/2); // y is inverted

    canvas.drawCircle(JoyC_widget_X, JoyC_widget_Y, JoyC_widget_R, JoyCircle_OC);
    canvas.fillCircle(inner_JoyCircle_X, inner_JoyCircle_Y, JoyCircle_IR, JoyCircle_IC);

    canvas.setCursor(JoyC_widget_X-11, JoyC_widget_Y+16);
    if (JoyC_In_X_DeadZone){
        canvas.setTextColor(TFT_LIGHTGREY);
    }
    else if (5 >= JoyC_X || JoyC_X >= 95){
        canvas.setTextColor(BLACK);
    }
    else{
        canvas.setTextColor(TFT_DARKGREY);
    }
    if (JoyC_X < 10){
        canvas.print("0"+String(JoyC_X));
    }
    else{
        canvas.print(JoyC_X);
    }


    canvas.setCursor(JoyC_widget_X+2, JoyC_widget_Y+16);
    if (JoyC_In_y_DeadZone){
        canvas.setTextColor(TFT_LIGHTGREY);
    }
    else if (5 >= JoyC_Y || JoyC_Y >= 95){
        canvas.setTextColor(BLACK);
    }
    else{
        canvas.setTextColor(TFT_DARKGREY);
    }

    if (JoyC_Y < 10){
        canvas.print("0"+String(JoyC_Y));
    }
    else{
        canvas.print(JoyC_Y);
    }
    

    
    byte Xinput_rect_x = 22;
    byte Xinput_rect_y = 4;

    if(JoyC_X_left_right == -1){ // left

        canvas.fillRect(JoyC_widget_X-14, JoyC_widget_Y-11, Xinput_rect_y, Xinput_rect_x, BLACK);
        //canvas.drawRect(JoyC_widget_X-12, JoyC_widget_Y-10, JoyC_widget_X-6, JoyC_widget_Y+10, BLACK);
        //canvas.drawLine(JoyC_widget_X-10, JoyC_widget_Y-10, JoyC_widget_X-10, JoyC_widget_Y+10, BLACK);
    }

    else if(JoyC_X_left_right == 1){ // right
        canvas.fillRect(JoyC_widget_X+11, JoyC_widget_Y-10, Xinput_rect_y, Xinput_rect_x, BLACK);
        //canvas.drawRect(JoyC_widget_X+6, JoyC_widget_Y-10, JoyC_widget_X+12, JoyC_widget_Y+10, BLACK);
        //canvas.drawLine(JoyC_widget_X+10, JoyC_widget_Y-10, JoyC_widget_X+10, JoyC_widget_Y+10, BLACK);
    }


    if(JoyC_Y_up_down == -1){   // down
        canvas.fillRect(JoyC_widget_X-10, JoyC_widget_Y+11, Xinput_rect_x, Xinput_rect_y, BLACK);
        //canvas.drawRect(JoyC_widget_X-10, JoyC_widget_Y-12, JoyC_widget_X+10, JoyC_widget_Y-6, BLACK);
        //canvas.drawLine(JoyC_widget_X-10, JoyC_widget_Y-10, JoyC_widget_X+10, JoyC_widget_Y-10, BLACK);
    }

    else if(JoyC_Y_up_down == 1){   // up
        canvas.fillRect(JoyC_widget_X-10, JoyC_widget_Y-14, Xinput_rect_x, Xinput_rect_y, BLACK);
        //canvas.drawRect(JoyC_widget_X-10, JoyC_widget_Y+6, JoyC_widget_X+10, JoyC_widget_Y+12, BLACK);
        //canvas.drawLine(JoyC_widget_X-10, JoyC_widget_Y+10, JoyC_widget_X+10, JoyC_widget_Y+10, BLACK);
    }




}



void LCD_CORE_Message(void){
    canvas.setTextFont(2);
    canvas.setTextSize(1);
    canvas.setCursor(CORE_SM_X, CORE_SM_Y);
    canvas.print("Core");
    canvas.setTextFont(4);
    canvas.setTextSize(2);
    canvas.setCursor(CORE_SM_X, CORE_SM_Y + 15);
    canvas.print(xPortGetCoreID());
}

void LCD_IMU_Message(void){
    canvas.setTextFont(4);
    canvas.setTextSize(1);
    canvas.setCursor(LCD_IMU_SM_X, LCD_IMU_SM_Y);
    canvas.print("Please Lay");
    canvas.setCursor(LCD_IMU_SM_X+3, LCD_IMU_SM_Y + 35);
    canvas.print("Robot Flat");
}

void LCD_Felg_Message(void){
    canvas.setTextFont(2);
    canvas.setTextSize(1);

    canvas.setCursor(LCD_FELG_SM_X+5, LCD_FELG_SM_Y);
    canvas.print("Designed By");

    canvas.setCursor(LCD_FELG_SM_X, LCD_FELG_SM_Y + 15);
    canvas.print("Felipe Galindo");

    canvas.setCursor(LCD_FELG_SM_X+3, LCD_FELG_SM_Y + 30);
    canvas.print("in Minnesota");
}

void LCD_calib1_Message(void){
    canvas.setTextFont(2);
    canvas.setTextSize(1);
    canvas.setTextColor(RED);

    canvas.setCursor(Middle_M_X, Middle_M_Y);
    canvas.print("Executing");

    canvas.setCursor(Middle_M_X, Middle_M_Y + 15);
    canvas.print("Stationary");

    canvas.setCursor(Middle_M_X, Middle_M_Y + 30);
    canvas.print("Calibration");

    canvas.setTextColor(WHITE);
}

void LCD_calib1_complete_Message(void){
    canvas.setTextFont(2);
    canvas.setTextSize(1);
    canvas.setTextColor(RED);

    canvas.setCursor(Middle_M_X, Middle_M_Y);
    canvas.print("Stationary");


    canvas.setCursor(Middle_M_X, Middle_M_Y + 15);
    canvas.print("Calibration");

    canvas.setCursor(Middle_M_X, Middle_M_Y + 30);
    canvas.print("Complete");

    canvas.setTextColor(WHITE);
}

void LCD_calib2_Message(void){
    canvas.setTextFont(2);
    canvas.setTextSize(1);
    canvas.setTextColor(RED);

    canvas.setCursor(Middle_M_X, Middle_M_Y);
    canvas.print("Executing");

    canvas.setCursor(Middle_M_X, Middle_M_Y + 15);
    canvas.print("Balance");

    canvas.setCursor(Middle_M_X, Middle_M_Y + 30);
    canvas.print("Calibration");

    canvas.setTextColor(WHITE);
}

void LCD_UI_Setup(){
    M5.Axp.ScreenBreath(11);
    //canvas.setRotation(2); // landscape inverted
    M5.lcd.setRotation(0);
    M5.lcd.fillScreen(BLACK);

    canvas.createSprite(135, 240);
    canvas.fillRect(0, 0, 135, 240, BLACK);

    

    // set TFT_eSPI LCD Brightness
    M5.Axp.ScreenBreath(lcd_brightness);


    canvas.setTextFont(2);
    canvas.setTextSize(1);
    canvas.setTextColor(WHITE);

    canvas.setCursor(SM_X, SM_Y);
    canvas.print("Starting Systems");

    LCD_CORE_Message();

    LCD_IMU_Message();

    LCD_Felg_Message();

    LCD_flush();
}





void LCD_DispBatVolt(int LCD_BTv_X = 0, int LCD_BTv_Y = 217, int bt_percent = -1, boolean bt_charging = false){

    const int LCD_BTv_W = 35;
    const int LCD_BTv_H = 13;
    const int LCD_BTv_R = 5;
    
    int LCD_BTv_C = TFT_LIGHTGREY;

    if(bt_percent > 80){
        LCD_BTv_C = GREEN;
    }
    else if(bt_charging){
        LCD_BTv_C = BLUE;
    }
    else if (bt_percent == -1){
        LCD_BTv_C = TFT_LIGHTGREY;
    }
    else if (bt_percent < 20){
        LCD_BTv_C = RED;
    }
    else if(bt_percent < 40){
        LCD_BTv_C = YELLOW;
    }

    canvas.setTextFont(1);
    canvas.setTextSize(1);

    canvas.setCursor(LCD_BTv_X+3, LCD_BTv_Y+3);

    canvas.fillRoundRect(LCD_BTv_X, LCD_BTv_Y,
     LCD_BTv_W, LCD_BTv_H, LCD_BTv_R, LCD_BTv_C);
    canvas.fillRoundRect(LCD_BTv_X+LCD_BTv_W, LCD_BTv_Y+2,
     4, 8, 2, LCD_BTv_C);

    String text = "";

    if (bt_percent <= -1){
        text = " ... ";
    }
    else if (bt_percent == 100){
        text = String(bt_percent) + "%";
    }
        
    else{
        text = " " + String(bt_percent) + "%";
    }


        

    //canvas.printf("%4.2fv ", vBatt);
    canvas.setTextColor(BLACK);
    canvas.print(text);
    //canvas.printf("%1.2fv ", vBatt);
    //canvas.printf("%1.0fv ", bt_percent);

}




void LCD_DispAngle() {
    canvas.setTextFont(6);
    canvas.setTextSize(1);
    canvas.setCursor(LCD_ANGLE_X-15, LCD_ANGLE_Y);

    canvas.fillRoundRect(LCD_ANGLE_X, LCD_ANGLE_Y-10,
     LCD_ANGLE_W, LCD_ANGLE_H, LCD_ANGLE_R, GREEN);

    canvas.printf("%5.0f   ", (-aveAccZ)*90.0);
}

void LCD_Update_Mode(){
    canvas.setTextFont(4);
    canvas.setTextSize(1);

    canvas.fillScreen(BLACK);

    canvas.setCursor(30, 5);
    
    if (demoMode == 0)
        canvas.print("Stand ");

    else if (demoMode == 1)
        canvas.print("Demo ");

}

void LCD_Abort_Message(){
    canvas.setTextFont(2);
    canvas.setTextSize(1);
    canvas.setTextColor(BLACK);

    canvas.fillRoundRect(Middle_M_X-30, Middle_M_Y-3,
     ABORT_M_W, ABORT_M_H, ABORT_M_R, RED);

    canvas.setCursor(Middle_M_X-25, Middle_M_Y);
    canvas.print("Robot Deactivated");

    canvas.setCursor(Middle_M_X-25, Middle_M_Y + 15);
    canvas.print("Reason:");

}

void LCD_Abort_DEG_Message(){

    canvas.setCursor(Middle_M_X-25, Middle_M_Y + 30);
    canvas.print("Max DEG:");
    canvas.println(maxAngle);

    canvas.setCursor(Middle_M_X+25, Middle_M_Y + 15);
    canvas.print("DEG:");
    canvas.print(varAng);

    canvas.setTextColor(WHITE);
}

void LCD_Abort_PWR_Message(){

    canvas.setCursor(Middle_M_X-25, Middle_M_Y + 30);
    canvas.print("Max PWR:");
    canvas.println(maxOvp);

    canvas.setCursor(Middle_M_X+25, Middle_M_Y + 15);
    canvas.print("PWR:");
    canvas.print(counterOverPwr);

    canvas.setTextColor(WHITE);
}

void LCD_Resume_from_Abort_Message(){
    LCD_IMU_Message();
}



void LCD_WiFi_Initializing_Message(){
    canvas.setTextFont(2);
    canvas.setTextSize(1);
    canvas.setTextColor(BLACK);

    const int WARN_WiFi_INIT_M_X = 5;
    const int WARN_WiFi_INIT_M_Y = 5;
    const int WARN_WiFi_INIT_M_W = 125;
    const int WARN_WiFi_INIT_M_H = 55;
    const int WARN_WiFi_INIT_M_R = 5;

    const int WARN_WiFi_INIT_M_S_H = 20;
    const int WARN_WiFi_INIT_M_S_W = 60;

    const int WARN_WiFi_INIT_M_S = WARN_WiFi_INIT_M_X+5 + (Warn_User_WiFi_Will_Be_Init_Selector_Abort * (WARN_WiFi_INIT_M_S_W-5));


    canvas.fillRoundRect(WARN_WiFi_INIT_M_X, WARN_WiFi_INIT_M_Y,
        WARN_WiFi_INIT_M_W, WARN_WiFi_INIT_M_H, WARN_WiFi_INIT_M_R, WHITE);

    canvas.setCursor(WARN_WiFi_INIT_M_X+5, WARN_WiFi_INIT_M_Y);
    canvas.print("Initializing");

    canvas.setCursor(WARN_WiFi_INIT_M_X+5, WARN_WiFi_INIT_M_Y + 15);
    canvas.print("WiFi");
    //canvas.print(" " + String(10-(Warn_User_WiFi_Will_Be_Init/10)) + "s");
    


    // canvas.fillRoundRect(WARN_WiFi_INIT_M_S, WARN_WiFi_INIT_M_Y+30,
    //     WARN_WiFi_INIT_M_S_W, WARN_WiFi_INIT_M_S_H, WARN_WiFi_INIT_M_R, TFT_LIGHTGREY);


    // canvas.setCursor(WARN_WiFi_INIT_M_X+10, WARN_WiFi_INIT_M_Y+32);
    // canvas.print("Start      Abort");



    // canvas.setTextColor(WHITE);

    //canvas.fillTriangle
}


void LCD_Warn_WiFi_Message(){
    canvas.setTextFont(2);
    canvas.setTextSize(1);
    canvas.setTextColor(BLACK);

    const int WARN_WiFi_INIT_M_X = 5;
    const int WARN_WiFi_INIT_M_Y = 5;
    const int WARN_WiFi_INIT_M_W = 125;
    const int WARN_WiFi_INIT_M_H = 55;
    const int WARN_WiFi_INIT_M_R = 5;

    const int WARN_WiFi_INIT_M_S_H = 20;
    const int WARN_WiFi_INIT_M_S_W = 60;

    const int WARN_WiFi_INIT_M_S = WARN_WiFi_INIT_M_X+5 + (Warn_User_WiFi_Will_Be_Init_Selector_Abort * (WARN_WiFi_INIT_M_S_W-5));


    canvas.fillRoundRect(WARN_WiFi_INIT_M_X, WARN_WiFi_INIT_M_Y,
        WARN_WiFi_INIT_M_W, WARN_WiFi_INIT_M_H, WARN_WiFi_INIT_M_R, WHITE);

    canvas.setCursor(WARN_WiFi_INIT_M_X+5, WARN_WiFi_INIT_M_Y);
    canvas.print("Scanning WiFi");

    canvas.setCursor(WARN_WiFi_INIT_M_X+5, WARN_WiFi_INIT_M_Y + 15);
    canvas.print("Networks in " + String(10-(Warn_User_WiFi_Will_Be_Init/10)) + "s");
    


    canvas.fillRoundRect(WARN_WiFi_INIT_M_S, WARN_WiFi_INIT_M_Y+30,
        WARN_WiFi_INIT_M_S_W, WARN_WiFi_INIT_M_S_H, WARN_WiFi_INIT_M_R, TFT_LIGHTGREY);


    canvas.setCursor(WARN_WiFi_INIT_M_X+10, WARN_WiFi_INIT_M_Y+32);
    canvas.print("Start      Abort");



    //canvas.setTextColor(WHITE);

    //canvas.fillTriangle
}

void LCD_Pairing_Message(){
    canvas.setTextFont(2);
    canvas.setTextSize(1);
    canvas.setTextColor(BLACK);

    const int PAIR_M_W = 125;
    const int PAIR_M_H = 40;
    const int PAIR_M_R = 5;

    canvas.fillRoundRect(Middle_M_X-30, Middle_M_Y-3,
     PAIR_M_W, PAIR_M_H, PAIR_M_R, RED);

    canvas.setCursor(Middle_M_X-25, Middle_M_Y);
    canvas.print("Robot Not Paired");

    canvas.setCursor(Middle_M_X-25, Middle_M_Y + 15);
    canvas.print("Please Pair");

    canvas.setTextColor(WHITE);

    //canvas.fillTriangle
}

// void LCD_Status_Message(){

//     String status_message = exec_status;

//     // get the length of status_message
//     //int status_message_length = status_message.length();

//     canvas.setTextFont(1);
//     canvas.setTextSize(1);
//     //canvas.setTextColor(BLACK);
    
//     //define the status box UI variables
//     const int STATUS_M_X = 0;
//     const int STATUS_M_Y = 220;
//     const int STATUS_M_W = 135;
//     const int STATUS_M_H = 20;
//     const int STATUS_M_R = 5;

//     const int STATUS_M_COLOR = WHITE;
//     int STATUS_M_TEXT_COLOR = invertColor16(STATUS_M_COLOR);
//     //int STATUS_M_TEXT_COLOR = BLACK;



//     // define color for the status box as white
    

//     canvas.fillRoundRect(STATUS_M_X, STATUS_M_Y,
//      STATUS_M_W, STATUS_M_H, STATUS_M_R, STATUS_M_COLOR);
    
//     canvas.setTextColor(STATUS_M_TEXT_COLOR);
    
//     canvas.setCursor(STATUS_M_X, STATUS_M_Y+2);
//     canvas.print(status_message);
// }

void LCD_Status_Message(){

    String status_message = exec_status;

    // get the length of status_message
    //int status_message_length = status_message.length();

    canvas.setTextFont(1);
    canvas.setTextSize(1);
    //canvas.setTextColor(BLACK);
    
    //define the status box UI variables
    const int STATUS_M_X = 0;
    const int STATUS_M_Y = 230;
    const int STATUS_M_W = 135;
    const int STATUS_M_H = 10;
    const int STATUS_M_R = 5;

    const int STATUS_M_COLOR = WHITE;
    int STATUS_M_TEXT_COLOR = invertColor16(STATUS_M_COLOR);
    //int STATUS_M_TEXT_COLOR = BLACK;



    // define color for the status box as white
    

    canvas.fillRoundRect(STATUS_M_X, STATUS_M_Y,
     STATUS_M_W, STATUS_M_H, STATUS_M_R, STATUS_M_COLOR);
    
    canvas.setTextColor(STATUS_M_TEXT_COLOR);
    
    canvas.setCursor(STATUS_M_X+2, STATUS_M_Y+1);
    canvas.print(status_message);
}


void LCD_print_JoyC_Debug(){
    
    //print the center values, and raw values

    canvas.setTextFont(1);
    canvas.setTextSize(1);
    canvas.setTextColor(BLACK);

    const int JOYC_DEBUG_M_X = 0;
    const int JOYC_DEBUG_M_Y = 150;
    const int JOYC_DEBUG_M_W = 135;
    const int JOYC_DEBUG_M_H = 70;
    const int JOYC_DEBUG_M_R = 5;

    const int JOYC_DEBUG_M_COLOR = WHITE;
    int JOYC_DEBUG_M_TEXT_COLOR = invertColor16(JOYC_DEBUG_M_COLOR);


    canvas.fillRoundRect(JOYC_DEBUG_M_X, JOYC_DEBUG_M_Y,
     JOYC_DEBUG_M_W, JOYC_DEBUG_M_H, JOYC_DEBUG_M_R, JOYC_DEBUG_M_COLOR);

    canvas.setTextColor(JOYC_DEBUG_M_TEXT_COLOR);

    canvas.setCursor(JOYC_DEBUG_M_X+5, JOYC_DEBUG_M_Y+5);
    canvas.print("X_C:");

    canvas.setCursor(JOYC_DEBUG_M_X+30, JOYC_DEBUG_M_Y+5);
    canvas.print(Joyc_X_center);

    
    canvas.setCursor(JOYC_DEBUG_M_X+65, JOYC_DEBUG_M_Y+5);
    canvas.print("Y_C:");

    canvas.setCursor(JOYC_DEBUG_M_X+90, JOYC_DEBUG_M_Y+5);
    canvas.print(Joyc_Y_center);


    canvas.setCursor(JOYC_DEBUG_M_X+5, JOYC_DEBUG_M_Y+20);
    canvas.print("X_R:");

    canvas.setCursor(JOYC_DEBUG_M_X+30, JOYC_DEBUG_M_Y+20);
    canvas.print(JoyC_X_raw);

    canvas.setCursor(JOYC_DEBUG_M_X+65, JOYC_DEBUG_M_Y+20);
    canvas.print("Y_R:");

    canvas.setCursor(JOYC_DEBUG_M_X+90, JOYC_DEBUG_M_Y+20);
    canvas.print(JoyC_Y_raw);


    
}


// void LCD_CPU_Widget(int COUNTER_M_X = 35, int COUNTER_M_Y = 220,){
//     canvas.setTextFont(1);
//     canvas.setTextSize(1);
    
//     //define the status box UI variables
//     const 
//     const int COUNTER_M_W = 100;
//     const int COUNTER_M_H = 10;
//     const int COUNTER_M_R = 5;

//     const int COUNTER_M_COLOR = WHITE;
//     int COUNTER_M_TEXT_COLOR = invertColor16(COUNTER_M_COLOR);

//     canvas.fillRoundRect(COUNTER_M_X, COUNTER_M_Y, COUNTER_M_W, COUNTER_M_H, COUNTER_M_R, COUNTER_M_COLOR);
    
//     canvas.setTextColor(COUNTER_M_TEXT_COLOR);

//     canvas.setCursor(COUNTER_M_X+5, COUNTER_M_Y+2);
//     canvas.print("CPU");


//     canvas.setCursor(COUNTER_M_X+30, COUNTER_M_Y+2);

//     String S_CPU_load = "0:"+String(int(BackgroundTask_CPU_load))+"%"+ " 1:";

//     if (RealTcode_CPU_load < 10){
//         S_CPU_load += "0";
//     }
    
    
//     S_CPU_load += String(int(RealTcode_CPU_load))+"%";
    
//     canvas.print(S_CPU_load);

// }




void LCD_CPU_Widget(int COUNTER_M_X, int COUNTER_M_Y, double bk_cpu_percent, double rt_cpu_load){
    canvas.setTextFont(1);
    canvas.setTextSize(1);
    
    //define the status box UI variables
    const int COUNTER_M_W = 100;
    const int COUNTER_M_H = 10;
    const int COUNTER_M_R = 5;

    const int COUNTER_M_COLOR = WHITE;
    int COUNTER_M_TEXT_COLOR = invertColor16(COUNTER_M_COLOR);

    canvas.fillRoundRect(COUNTER_M_X, COUNTER_M_Y, COUNTER_M_W, COUNTER_M_H, COUNTER_M_R, COUNTER_M_COLOR);
    
    canvas.setTextColor(COUNTER_M_TEXT_COLOR);

    canvas.setCursor(COUNTER_M_X+7, COUNTER_M_Y+2);
    canvas.print("CPU");


    canvas.setCursor(COUNTER_M_X+29, COUNTER_M_Y+2);

    String S_CPU_load = "0:"+String(int(bk_cpu_percent))+"%"+ " 1:";

    if (rt_cpu_load < 10){
        S_CPU_load += "0";
    }
    
    
    S_CPU_load += String(int(rt_cpu_load))+"%";
    
    canvas.print(S_CPU_load);

}















// Extract RGB components from RGB565
auto extractRGB565 = [](uint16_t color, uint8_t &r, uint8_t &g, uint8_t &b) {
    r = (color >> 11) & 0x1F;
    g = (color >> 5) & 0x3F;
    b = color & 0x1F;
};

// Function to mix two RGB565 colors based on a weight
auto mixRGB565 = [](uint16_t color1, uint16_t color2, float weight, uint16_t &result) {
    uint8_t r1, g1, b1, r2, g2, b2;
    extractRGB565(color1, r1, g1, b1);
    extractRGB565(color2, r2, g2, b2);

    // Corrected: Cast to uint8_t after computing the blend, before applying the mask
    uint8_t r = static_cast<uint8_t>(r1 * (1 - weight) + r2 * weight) & 0x1F;
    uint8_t g = static_cast<uint8_t>(g1 * (1 - weight) + g2 * weight) & 0x3F;
    uint8_t b = static_cast<uint8_t>(b1 * (1 - weight) + b2 * weight) & 0x1F;

    result = (r << 11) | (g << 5) | b;
};




uint16_t interpolateColor(int value, int minRange, int midRange, int maxRange, uint16_t startColor, uint16_t midColor, uint16_t endColor) {

    // make sure value is within range

    if (value < minRange){
        value = minRange;
    }
    else if (value > maxRange){
        value = maxRange;
    }


    // Normalize value to 0-1
    float normalized = 0;
    uint16_t color = 0;
    


    if (value <= midRange) {
        normalized = (float)(value - minRange) / (midRange - minRange);
        mixRGB565(startColor, midColor, normalized, color);
    } else {
        normalized = (float)(value - midRange) / (maxRange - midRange);
        mixRGB565(midColor, endColor, normalized, color);
    }

    return color;
}





void LCD_Western_Artificial_Horizon(){

    int AH_X = 18;
    int AH_Y = 20;
    int AH_W = 100;
    int AH_H = 100;

    const int JoyCircle_X = AH_X + AH_W/2;
    const int JoyCircle_Y = AH_Y + AH_H/2;
    const int JoyCircle_R = 10;
    const int JoyCircle_IR = 8;
    
    int JoyCircle_OC = BLACK;

    // // Define colors for interpolation
    // uint32_t red = tft.color565(255, 0, 0);
    // uint32_t yellow = tft.color565(255, 255, 0);
    // uint32_t blue = tft.color565(0, 0, 255);
    // uint32_t lightBlue = tft.color565(0, 128, 255);
    // uint32_t green = tft.color565(0, 255, 0);




    int Sky_C = 0x871F;
    //int Sky_C = 0x111F;
    //int Sky_C = TFT_BLUE;

    int Ground_C = 0x87CE;
    //int Ground_C = TFT_GREEN;


    // RGB565(255, 255, 150) = 0xFF9F

    int LIGHT_YELLOW = 0xFFFF69; // ;)







    // canvas.fillRoundRect(AH_X-10, AH_Y-10, AH_W+20, AH_H+20, 10, TFT_LIGHTGREY);


    // Calculate point offsets

    // int AH_Y_Offset = robot_Y_deg;
    // int AH_dif_offset = Avg_IMU_Z_deg_per_sec/4;
    
    //int AH_Y_Offset = -20 * robot_Y_deg;
    int AH_Y_Offset = -8 * robot_Y_deg;
    // int AH_Y_Offset = pitch;
    //int AH_Y_Offset = robot_Y_deg;
    int AH_dif_offset = Avg_IMU_Z_deg_per_sec;

    //Serial.println("AH_Y_Offset: " + String(AH_Y_Offset) + " AH_dif_offset: " + String(AH_dif_offset));

    // Calculate the horizon line


    int AH_Y_center = AH_Y + AH_H/2;

    int AH_Horizon_x0 = AH_X;
    int AH_Horizon_y0 = AH_Y_center - AH_Y_Offset + AH_dif_offset;

    int AH_Horizon_x1 = AH_X + AH_W;
    int AH_Horizon_y1 = AH_Y_center - AH_Y_Offset - AH_dif_offset;


    // make sure the horizon line is within the screen

    if (AH_Horizon_y0 < AH_Y){
        AH_Horizon_y0 = AH_Y;
    }
    else if (AH_Horizon_y0 > (AH_Y + AH_H)){
        AH_Horizon_y0 = AH_Y + AH_H;
    }


    if (AH_Horizon_y1 < AH_Y){
        AH_Horizon_y1 = AH_Y;
    }
    else if (AH_Horizon_y1 > (AH_Y + AH_H)){
        AH_Horizon_y1 = AH_Y + AH_H;
    }




    // draw the 4 triangles

    // draw the sky

    canvas.fillTriangle(AH_X         , AH_Y,    (AH_X + AH_W),          AH_Y,   AH_Horizon_x0, AH_Horizon_y0, Sky_C);
    canvas.fillTriangle((AH_X + AH_W), AH_Y,    AH_Horizon_x1, AH_Horizon_y1,   AH_Horizon_x0, AH_Horizon_y0, Sky_C);

    // draw the ground
    canvas.fillTriangle(AH_X, (AH_Y + AH_H), AH_Horizon_x1, AH_Horizon_y1, AH_Horizon_x0, AH_Horizon_y0, Ground_C);
    canvas.fillTriangle(AH_X, (AH_Y + AH_H),    (AH_X + AH_W), (AH_Y + AH_H),   AH_Horizon_x1, AH_Horizon_y1, Ground_C);


        
    int JoyCircle_IC = TFT_LIGHTGREY;

    if (JoyC_btn){
        JoyCircle_IC = RED;
    }
    else if (Robot_JoyC_r < 5){
        JoyCircle_IC = TFT_LIGHTGREY;
    }
    else if (Robot_JoyC_r > 40){
        JoyCircle_IC = BLACK;
    }
    else{
        JoyCircle_IC = TFT_DARKGREY;
    }
    
    float inner_JoyCircle_X = JoyCircle_X + ((Robot_JoyC_X)) - (AH_W/2);
    float inner_JoyCircle_Y = JoyCircle_Y - ((Robot_JoyC_Y)) + (AH_H/2); // y is inverted


    canvas.fillCircle(inner_JoyCircle_X, inner_JoyCircle_Y, JoyCircle_IR, JoyCircle_IC);





    // draw the horizon line

    canvas.drawLine(AH_X, (AH_Y + (AH_H/2)), (AH_X + AH_W), (AH_Y + (AH_H/2)), TFT_LIGHTGREY);


    int AH_Motors_Y_Offset = (Rmotor + Lmotor)/2;
    int AH_Motors_dif_offset = 2 * (Rmotor - Lmotor);

    // make sure that offsets are not more than .5 height

    if (AH_Motors_Y_Offset > AH_H/2){
        AH_Motors_Y_Offset = AH_H/2;
    }
    else if (AH_Motors_Y_Offset < -AH_H/2){
        AH_Motors_Y_Offset = -AH_H/2;
    }

    if (AH_Motors_dif_offset > AH_H/2){
        AH_Motors_dif_offset = AH_H/2;
    }
    else if (AH_Motors_dif_offset < -AH_H/2){
        AH_Motors_dif_offset = -AH_H/2;
    }


    // draw the motor indicator lines

    // canvas.drawLine(AH_X, (AH_Y + (AH_H/2) + Rmotor)-1, (AH_X + AH_W), (AH_Y + (AH_H/2) + Lmotor)-1, BLACK); 
    // canvas.drawLine(AH_X, (AH_Y + (AH_H/2) + Rmotor)+1, (AH_X + AH_W), (AH_Y + (AH_H/2) + Lmotor)+1, BLACK); 

    canvas.drawLine(AH_X, (AH_Y + (AH_H/2) + AH_Motors_Y_Offset + AH_Motors_dif_offset)-1, (AH_X + AH_W), (AH_Y + (AH_H/2) + AH_Motors_Y_Offset - AH_Motors_dif_offset)-1, BLACK);
    canvas.drawLine(AH_X, (AH_Y + (AH_H/2) + AH_Motors_Y_Offset + AH_Motors_dif_offset)+1, (AH_X + AH_W), (AH_Y + (AH_H/2) + AH_Motors_Y_Offset - AH_Motors_dif_offset)+1, BLACK);


    // draw the aircraft

    // canvas.setTextColor(BLACK);

    // canvas.setCursor(AH_X+AH_W/2-10, AH_Y+AH_H/2-5);
    // canvas.print("--w--"); 

    float indicator_JoyCircle_X = JoyCircle_X + ((JoyC_X)) - (AH_W/2);
    float indicator_JoyCircle_Y = JoyCircle_Y - ((JoyC_Y)) + (AH_H/2); // y is inverted

    if (JoyC_X == 50 && JoyC_Y == 50){
        // JoyCircle_OC = BLACK;
        JoyCircle_OC = TFT_LIGHTGREY;
    }
    else{
        // JoyCircle_OC = TFT_LIGHTGREY;
        JoyCircle_OC = BLACK;
    }


    canvas.drawCircle(indicator_JoyCircle_X, indicator_JoyCircle_Y, JoyCircle_R, JoyCircle_OC);


    // print the joystick values

    byte AH_JoyC_X = (AH_X)-18;
    byte AH_JoyC_Y = (AH_Y)-20;
    byte AH_JoyC_W = 30;
    byte AH_JoyC_H = 20;
    int AH_JoyC_C = TFT_LIGHTGREY;

    canvas.setTextFont(1);
    canvas.setTextSize(2);

    //canvas.fillRect(AH_JoyC_X, AH_JoyC_Y, AH_JoyC_W, AH_JoyC_H, AH_JoyC_C);

    

    /// IMU Y DEG RECT
    int Rect_color = interpolateColor(int(Avg_IMU_Z_deg_per_sec), -100, 0, 100, RED, LIGHT_YELLOW, BLUE);
    //canvas.setTextColor(invertColor16(Rect_color));

    canvas.fillRoundRect((AH_JoyC_X + AH_JoyC_W), AH_JoyC_Y, AH_JoyC_W+10, AH_JoyC_H, 5, Rect_color);
    canvas.setCursor(AH_JoyC_X-4 + AH_JoyC_W, AH_JoyC_Y + 3);
    // if (IMU_Z_deg_per_sec == 50){
    //     canvas.setTextColor(TFT_LIGHTGREY);
    // }
    // else if (5 >= IMU_Z_deg_per_sec || IMU_Z_deg_per_sec >= 95){
    //     canvas.setTextColor(BLACK);
    // }
    // else{
    //     canvas.setTextColor(TFT_DARKGREY);
    // }

    if (Avg_IMU_Z_deg_per_sec >= 0){
        canvas.print(" "+String(int(Avg_IMU_Z_deg_per_sec)));
    }
    else{
        canvas.print(int(Avg_IMU_Z_deg_per_sec));
    }




    Rect_color = interpolateColor(JoyC_X, 0, 50, 100, BLUE, LIGHT_YELLOW, RED);
    canvas.setTextColor(BLACK);

    

    canvas.fillRoundRect(AH_JoyC_X, AH_JoyC_Y, AH_JoyC_W, AH_JoyC_H, 5, Rect_color);
    canvas.setCursor(AH_JoyC_X+2, AH_JoyC_Y+2);

    // if (JoyC_X == 50){
    //     canvas.setTextColor(TFT_LIGHTGREY);
    // }
    // else if (5 >= JoyC_X || JoyC_X >= 95){
    //     canvas.setTextColor(BLACK);
    // }
    // else{
    //     canvas.setTextColor(TFT_DARKGREY);
    // }

    if (JoyC_X < 10){
        canvas.print("0"+String(JoyC_X));
    }
    else{
        canvas.print(JoyC_X);
    }





    Rect_color = interpolateColor(JoyC_Y, 0, 50, 100, GREEN, Sky_C, BLUE);
    //canvas.setTextColor(invertColor16(Rect_color));
    
    canvas.fillRoundRect((AH_JoyC_X + 2 * AH_JoyC_W+10), AH_JoyC_Y, AH_JoyC_W, AH_JoyC_H, 5, Rect_color);

    canvas.setCursor((AH_JoyC_X + 2 * AH_JoyC_W + 13), AH_JoyC_Y + 3);
    // if (JoyC_Y == 50){
    //     canvas.setTextColor(TFT_LIGHTGREY);
    // }
    // else if (5 >= JoyC_Y || JoyC_Y >= 95){
    //     canvas.setTextColor(BLACK);
    // }
    // else{
    //     canvas.setTextColor(TFT_DARKGREY);
    // }


    if (JoyC_Y < 10){
        canvas.print("0"+String(JoyC_Y));
    }
    else{
        canvas.print(JoyC_Y);
    }

    Rect_color = interpolateColor(int(robot_Y_deg), -100, 0, 100, RED, BLUE, RED);
    //canvas.setTextColor(invertColor16(Rect_color));

    canvas.fillRoundRect((AH_JoyC_X + 3 * AH_JoyC_W) + 10, AH_JoyC_Y , AH_JoyC_W + 5, AH_JoyC_H, 5, Rect_color);
    canvas.setCursor((AH_JoyC_X + 3 * AH_JoyC_W + 5), AH_JoyC_Y + 3);
    // if (JoyC_Y == 50){
    //     canvas.setTextColor(TFT_LIGHTGREY);
    // }
    // else if (5 >= JoyC_Y || JoyC_Y >= 95){
    //     canvas.setTextColor(BLACK);
    // }
    // else{
    //     canvas.setTextColor(TFT_DARKGREY);
    // }

    if (robot_Y_deg > 0){
        canvas.print(" "+String(int(robot_Y_deg)));
    }
    else{
        canvas.print(int(robot_Y_deg));
    }


    canvas.setTextSize(1);
    canvas.setTextFont(1);



    // if (isArmed){
    if (robot_state == ROBOT_ARMED){

        canvas.fillRoundRect(AH_X+2, AH_Y+3, 18, 10, 4, BLACK);

        canvas.setCursor(AH_X+4, AH_Y+5);
        canvas.setTextColor(RED);
        canvas.print("P");

        canvas.setCursor(AH_X+8, AH_Y+5);
        canvas.setTextColor(GREEN);
        canvas.print("I");

        canvas.setCursor(AH_X+13, AH_Y+5);
        canvas.setTextColor(BLUE);
        canvas.print("D");

        if(!standing){
            canvas.setTextColor(BLACK);
            canvas.setCursor(AH_X+35, AH_Y+5);
            canvas.print("ARMED");
        }

    }

    else{
        canvas.setTextColor(BLACK);
        canvas.setCursor(AH_X+27, AH_Y+5);
        canvas.print("DISARMED");
    }


}
