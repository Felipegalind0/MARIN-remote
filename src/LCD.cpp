#include <M5StickCPlus.h>
#include "IO.h"
#include "variables.h"
#include "LCD.h"
#include "COMS.h"

uint16_t invertColor16(uint16_t color) {
    // Invert the color: 0xFFFF is max value for 16-bit, color is the original color
    return 0xFFFF - color;
}

TFT_eSprite canvas = TFT_eSprite(&M5.Lcd);

void LCD_flush(){
    canvas.pushSprite(0, 0);
    canvas.fillSprite(BLACK);
}

void LCD_loop(){
    if ((counter % 1000) == 0) {
            Serial.print("Updating Battery Voltage: ");
            updateBatVolt();
            Serial.print("LCD_loop() running on core ");
            Serial.println(xPortGetCoreID());
    }
    if (!(is_paired)) {
        Serial.print("Showing Pairing Message");
        LCD_Pairing_Message();
    }
    //LCD_DispAngle();

    //Serial.println("LCD_Status_Message()");
    LCD_Status_Message();

    //Serial.println("LCD_Print_Counter()");
    LCD_Print_Counter();

    LCD_print_JoyC_Debug();

    //Serial.println("pushSprite()");
    canvas.pushSprite(0, 0);
}

void LCD_Print_Counter(){
    canvas.setTextFont(1);
    canvas.setTextSize(1);
    
    //define the status box UI variables
    const int COUNTER_M_X = 0;
    const int COUNTER_M_Y = 0;
    const int COUNTER_M_W = 140;
    const int COUNTER_M_H = 40;
    const int COUNTER_M_R = 10;


    const int COUNTER_M_COLOR = WHITE;
    int COUNTER_M_TEXT_COLOR = invertColor16(COUNTER_M_COLOR);


    canvas.fillRoundRect(COUNTER_M_X, COUNTER_M_Y,
     COUNTER_M_W, COUNTER_M_H, COUNTER_M_R, COUNTER_M_COLOR);
    
    canvas.setTextColor(COUNTER_M_TEXT_COLOR);

    canvas.setCursor(COUNTER_M_X+100, COUNTER_M_Y+2);
    canvas.print(counter);
    
    
    canvas.setCursor(COUNTER_M_X+40, COUNTER_M_Y+2);
    canvas.print("us");
    canvas.setCursor(COUNTER_M_X+55, COUNTER_M_Y+2);
    canvas.print(RealTcode_execution_time);

    
    const int JoyCircle_X = 12;
    const int JoyCircle_Y = 12;
    const int JoyCircle_R = 10;
    const int JoyCircle_IR = 3;
    const int JoyCircle_OC = BLACK;

    int JoyCircle_IC = BLACK;

    if (JoyC_In_X_DeadZone && JoyC_In_y_DeadZone){
        JoyCircle_IC = RED;
    }
    
    float inner_JoyCircle_X = JoyCircle_X + (JoyC_X / 10) -5;
    float inner_JoyCircle_Y = JoyCircle_Y - (JoyC_Y / 10) +5; // y is inverted

    canvas.drawCircle(JoyCircle_X, JoyCircle_Y, JoyCircle_R, JoyCircle_OC);
    canvas.fillCircle(inner_JoyCircle_X, inner_JoyCircle_Y, JoyCircle_IR, JoyCircle_IC);

    canvas.setCursor(JoyCircle_X-8, JoyCircle_Y+18);
    if (JoyC_In_X_DeadZone){
        canvas.setTextColor(RED);
    }
    else{
        canvas.setTextColor(BLACK);
    }
    canvas.print(JoyC_X);

    canvas.setCursor(JoyCircle_X+6, JoyCircle_Y+18);
    if (JoyC_In_y_DeadZone){
        canvas.setTextColor(RED);
    }
    else{
        canvas.setTextColor(BLACK);
    }
    canvas.print(JoyC_Y);


    if(JoyC_X_left_right == -1){ // left

        canvas.fillRect(JoyCircle_X-12, JoyCircle_Y-10, 4, 20, BLACK);
        //canvas.drawRect(JoyCircle_X-12, JoyCircle_Y-10, JoyCircle_X-6, JoyCircle_Y+10, BLACK);
        //canvas.drawLine(JoyCircle_X-10, JoyCircle_Y-10, JoyCircle_X-10, JoyCircle_Y+10, BLACK);
    }

    else if(JoyC_X_left_right == 1){ // right
        canvas.fillRect(JoyCircle_X+9, JoyCircle_Y-10, 4, 20, BLACK);
        //canvas.drawRect(JoyCircle_X+6, JoyCircle_Y-10, JoyCircle_X+12, JoyCircle_Y+10, BLACK);
        //canvas.drawLine(JoyCircle_X+10, JoyCircle_Y-10, JoyCircle_X+10, JoyCircle_Y+10, BLACK);
    }


    if(JoyC_Y_up_down == -1){   // down
        canvas.fillRect(JoyCircle_X-10, JoyCircle_Y+10, 20, 4, BLACK);
        //canvas.drawRect(JoyCircle_X-10, JoyCircle_Y-12, JoyCircle_X+10, JoyCircle_Y-6, BLACK);
        //canvas.drawLine(JoyCircle_X-10, JoyCircle_Y-10, JoyCircle_X+10, JoyCircle_Y-10, BLACK);
    }

    else if(JoyC_Y_up_down == 1){   // up
        canvas.fillRect(JoyCircle_X-9, JoyCircle_Y-12, 20, 4, BLACK);
        //canvas.drawRect(JoyCircle_X-10, JoyCircle_Y+6, JoyCircle_X+10, JoyCircle_Y+12, BLACK);
        //canvas.drawLine(JoyCircle_X-10, JoyCircle_Y+10, JoyCircle_X+10, JoyCircle_Y+10, BLACK);
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

void LCD_Setup(){
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

void LCD_DispBatVolt() {

    const int LCD_BTv_X = 5;
    const int LCD_BTv_Y = 40; //220
    const int LCD_BTv_W = 50;
    const int LCD_BTv_H = 20;
    const int LCD_BTv_R = 5;

    canvas.setTextFont(2);
    canvas.setTextSize(1);

    canvas.setCursor(LCD_BTv_X, LCD_BTv_Y);

    canvas.fillRoundRect(LCD_BTv_X-5, LCD_BTv_Y-2,
     LCD_BTv_W, LCD_BTv_H, LCD_BTv_R, BLUE);

    canvas.printf("%4.2fv ", vBatt);
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

void LCD_Status_Message(){

    String status_message = exec_status;

    // get the length of status_message
    //int status_message_length = status_message.length();

    canvas.setTextFont(1);
    canvas.setTextSize(1);
    //canvas.setTextColor(BLACK);
    
    //define the status box UI variables
    const int STATUS_M_X = 0;
    const int STATUS_M_Y = 220;
    const int STATUS_M_W = 135;
    const int STATUS_M_H = 20;
    const int STATUS_M_R = 5;

    const int STATUS_M_COLOR = WHITE;
    int STATUS_M_TEXT_COLOR = invertColor16(STATUS_M_COLOR);
    //int STATUS_M_TEXT_COLOR = BLACK;



    // define color for the status box as white
    

    canvas.fillRoundRect(STATUS_M_X, STATUS_M_Y,
     STATUS_M_W, STATUS_M_H, STATUS_M_R, STATUS_M_COLOR);
    
    canvas.setTextColor(STATUS_M_TEXT_COLOR);
    
    canvas.setCursor(STATUS_M_X, STATUS_M_Y+2);
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