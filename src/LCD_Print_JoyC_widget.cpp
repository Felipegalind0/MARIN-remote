#include "LCD_Print_JoyC_widget.h"
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