#include "LCD_menu.h"


String get_menu_title(int X_index){
    switch (X_index){
        case ROBOT_MENU:
            return "  < Robot Menu >";
        case WIFI_NETWORKS:
            return " < WiFi Networks ";

        case WIFI_MENU:
            return " < WiFi Menu ";
        default:
            return "";
    }
}

String get_menu_str(int X_index, int Y_index){


    switch (X_index){


        case ROBOT_MENU:

            switch (Y_index){
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
            


        case WIFI_NETWORKS:

            if (Y_index < 1){
                return String(WiFi.RSSI(-Y_index))+ "dB " + WiFi.SSID(-Y_index); 
            }
            else if (Y_index == 1){
                return "        ReScan";
            }


        case WIFI_MENU:
            switch (Y_index){
                case WIFI_SHOW_INFO:
                    return "      Info";
                case WIFI_MENU_CONNECT_DISCONNECT:
                    return "     Connect";
                case WIFI_MENU_SCAN:
                    return "      ReScan";
                case WIFI_SET_NUM:
                    return "     Set Num";


                default:
                    return "";
            }

        default: 
            return "get_menu_str(" + String(X_index) + ", " + String(Y_index) + ")";

    }
}


void LCD_Top_1_line_text(String text, byte text_size, int color, byte widget_x, byte widget_y, byte widget_w, byte widget_h, byte widget_r){
    canvas.setTextFont(text_size);
    canvas.setTextSize(1);

    // text will be something like " -#dB SSID", split by the first space and compare with ssid to see if it is the same
    String txt_ssid = text.substring(text.indexOf(" ")+1);
    // Serial.println("txt_ssid: " + txt_ssid + " ssid: " + ssid);
    if (txt_ssid.equals(ssid)){
        
        canvas.setTextColor(BLUE);
    }
    else{
        // canvas.setTextColor(color);
        canvas.setTextColor(BLACK);
    }



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

    //canvas.setTextColor(invertColor16(color));

    canvas.setCursor(widget_x+5, widget_y+2);



    canvas.print(text);



}





void LCD_Menu(){
    int menu_rect_text_size = 2;



    //LCD_Top_1_line_text("    Robot Menu", 1, WHITE, 5, 5, 127, 20, 5);
    String robot_menu_text_buf = get_menu_title(menu_X_selector);

    canvas.fillRoundRect(0, 45, 135, 82, 5, DARKGREY);

    LCD_Top_1_line_text(robot_menu_text_buf, menu_rect_text_size, WHITE, 2, 48, 131, 16, 3);
    //LCD_Top_1_line_text(robot_menu_text_buf, menu_rect_text_size, DARKGREY, 0, 30, 135, 100, 5);

    


    switch (menu_X_selector){
        case ROBOT_MENU:
            menu_rect_text_size = 2;
            break;
        case WIFI_NETWORKS:
            menu_rect_text_size = 1;
            break;
        default:
            menu_rect_text_size = 1;
    }



    # define upper_menu_rect_x 2
    # define upper_menu_rect_y 65
    # define upper_menu_rect_w 131
    # define upper_menu_rect_h 20
    # define upper_menu_rect_r 10



    if( (menu_X_selector == ROBOT_MENU && ((menu_Y_selector+1) <= Robot_menu_max_X)) ||
        // (menu_X_selector == WIFI_MENU && ((menu_Y_selector+1) <= n_WiFi_Networks-1)) ){
        (menu_X_selector == WIFI_NETWORKS && (menu_Y_selector < 1)) ||
        (menu_X_selector == WIFI_MENU && (menu_Y_selector < WIFI_MENU_Y_MAX)) ){


        robot_menu_text_buf = get_menu_str(menu_X_selector, menu_Y_selector+1);

        //LCD_Top_1_line_text(robot_menu_text_buf, 1, TFT_LIGHTGREY, 5, 50, 131, 20, 10);
        LCD_Top_1_line_text(robot_menu_text_buf, menu_rect_text_size, TFT_LIGHTGREY, upper_menu_rect_x, upper_menu_rect_y, upper_menu_rect_w, upper_menu_rect_h, upper_menu_rect_r);

    }
    else {

        //LCD_Top_1_line_text(" ", 1, BLACK, 5, 50, 127, 20, 10);
        LCD_Top_1_line_text(" ", menu_rect_text_size, DARKGREY, upper_menu_rect_x, upper_menu_rect_y, upper_menu_rect_w, upper_menu_rect_h, upper_menu_rect_r);

    }


    robot_menu_text_buf = get_menu_str(menu_X_selector, menu_Y_selector);

    LCD_Top_1_line_text(robot_menu_text_buf, menu_rect_text_size, WHITE, 2, 85, 131, 20, 7);

    
    # define lower_menu_rect_x 2
    # define lower_menu_rect_y 105
    # define lower_menu_rect_w 131
    # define lower_menu_rect_h 20
    # define lower_menu_rect_r 10


    if( (menu_X_selector == ROBOT_MENU && ((menu_Y_selector-1) >= Robot_menu_min_X)) ||
        // (menu_X_selector == WIFI_MENU && ((menu_Y_selector-1) >= 0)) ){
        (menu_X_selector == WIFI_NETWORKS && (menu_Y_selector > 1-n_WiFi_Networks)) ||
        (menu_X_selector == WIFI_MENU && (menu_Y_selector > WIFI_MENU_Y_MIN))     ){


        robot_menu_text_buf = get_menu_str(menu_X_selector, menu_Y_selector-1);

        //LCD_Top_1_line_text(robot_menu_text_buf, 1, TFT_LIGHTGREY, 2, 110, 131, 20, 10);
        LCD_Top_1_line_text(robot_menu_text_buf, menu_rect_text_size, TFT_LIGHTGREY, lower_menu_rect_x, lower_menu_rect_y, lower_menu_rect_w, lower_menu_rect_h, lower_menu_rect_r);
    
    }
    else {
        //LCD_Top_1_line_text(" ", 1, BLACK, 5, 110, 131, 20, 10);
        LCD_Top_1_line_text(" ", menu_rect_text_size, DARKGREY, lower_menu_rect_x, lower_menu_rect_y, lower_menu_rect_w, lower_menu_rect_h, lower_menu_rect_r);
    
    }

}