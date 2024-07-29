#include "LCD_Western_Artificial_Horizon.h"

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
