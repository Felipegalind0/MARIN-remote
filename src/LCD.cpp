// --- LCD.cpp ---
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

#include "utility/qrcode.h"
void print_qr_code(const char *string, uint16_t x, uint16_t y, uint8_t width, uint8_t version){
     
  // Create the QR code
  QRCode qrcode;
  uint8_t qrcodeData[qrcode_getBufferSize(version)];
  qrcode_initText(&qrcode, qrcodeData, version, 0, string);
  
  // Top quiet zone
  uint8_t thickness = width / qrcode.size;
  uint16_t lineLength = qrcode.size * thickness;
  uint8_t xOffset = x + (width-lineLength)/2;
  uint8_t yOffset = y + (width-lineLength)/2;
  canvas.fillRect(x, y, width, width, TFT_WHITE);

  for (uint8_t y = 0; y < qrcode.size; y++) {
    for (uint8_t x = 0; x < qrcode.size; x++) {
      uint8_t q = qrcode_getModule(&qrcode, x, y);
      if (q) canvas.fillRect(x * thickness + xOffset, y * thickness + yOffset, thickness, thickness, TFT_BLACK);
    }
  }
}

void LCD_Felg_Message(void){
    canvas.setTextFont(1);
    canvas.setTextSize(1);

    canvas.setCursor(LCD_FELG_SM_X, LCD_FELG_SM_Y-3);
    canvas.print(Remote_Name);

    canvas.setCursor(LCD_FELG_SM_X+5, LCD_FELG_SM_Y+9);
    canvas.print("Designed By");

    canvas.setCursor(LCD_FELG_SM_X, LCD_FELG_SM_Y +18);
    canvas.print("Felipe Galindo");

    canvas.setCursor(LCD_FELG_SM_X+3, LCD_FELG_SM_Y +27);
    canvas.print("in Minnesota");

    //print QR code

    print_qr_code("z.umn.edu/MARIN_remote", 17, 65, 100, 6);
    

    //bad, uses M5.Lcd, which is not the canvas
    //M5.Lcd.qrcode("www.github.com/felipegalindo", 50, 50, 50, 6);


    //draw white round rect and then print link
    canvas.fillRoundRect(0, 160, 135, 10, 3, TFT_WHITE);
    // canvas.fillRoundRect(0, 0, 135, 240, 10, TFT_WHITE);
    canvas.setCursor(2, 160);
    canvas.setTextFont(1);
    canvas.setTextSize(1);
    canvas.setTextColor(TFT_BLACK);
    canvas.print("z.umn.edu/MARIN_remote");


    // push the sprite to the screen
    canvas.pushSprite(0, 0);
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

    // canvas.setCursor(SM_X, SM_Y);
    // canvas.print("Starting Systems");

    //LCD_CORE_Message();


    LCD_Felg_Message();

    //LCD_flush();

    // wait for 2 seconds
    //delay(2000);
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

void LCD_WiFi_Scanning_Message(){
    initMSG_has_been_flushed = false;
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
    canvas.print("Scanning");

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

    //int STATUS_M_COLOR = WHITE;
    int STATUS_M_TEXT_COLOR = invertColor16(exec_status_color);
    //int STATUS_M_TEXT_COLOR = BLACK;



    // define color for the status box as white
    

    canvas.fillRoundRect(STATUS_M_X, STATUS_M_Y,
     STATUS_M_W, STATUS_M_H, STATUS_M_R, exec_status_color);
    
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







// --- LCD.cpp ---