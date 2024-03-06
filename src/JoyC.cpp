#include <M5StickCPlus.h>

#include <Wire.h>
#include "Arduino.h"
#include "JoyC.h"
#include "variables.h"

#include <cmath> // Include the cmath library for sqrt and atan2 functions

void cartesianToPolar(byte *x, byte *y, float *r, float *phi) {
    // // Normalize x and y to have 0,0 at the center
    // float x_normalized = JoyC_X - 50.0;
    // float y_normalized = JoyC_Y - 50.0;

    // // Calculate the radius
    // JoyC_r = sqrt(x_normalized * x_normalized + y_normalized * y_normalized);

    // // Calculate the angle in radians
    // JoyC_Phi = atan2(y_normalized, x_normalized);

    // // for angle in degrees, uncomment the following line
    // JoyC_Phi = JoyC_Phi * (180.0 / M_PI);

    // //Serial.println("r: " + String(JoyC_r) + " φ: " + String(JoyC_Phi));



    // UPDATED TO USE POINTERS

    // Normalize x and y to have 0,0 at the center
    float x_normalized = *x - 50.0;
    float y_normalized = *y - 50.0;

    // Calculate the radius
    *r = sqrt(x_normalized * x_normalized + y_normalized * y_normalized);

    // Calculate the angle in radians
    *phi = atan2(y_normalized, x_normalized);

    // for angle in degrees, uncomment the following line
    *phi = *phi * (180.0 / M_PI);

    //Serial.println("r: " + String(JoyC_r) + " φ: " + String(JoyC_Phi));

}


void UNIT_JOYC::writeBytes(uint8_t addr, uint8_t reg, uint8_t *buffer,
                           uint8_t length) {
    _wire->beginTransmission(addr);
    _wire->write(reg);
    for (int i = 0; i < length; i++) {
        _wire->write(*(buffer + i));
    }
    _wire->endTransmission();
}

void UNIT_JOYC::readBytes(uint8_t addr, uint8_t reg, uint8_t *buffer,
                          uint8_t length) {
    uint8_t index = 0;
    _wire->beginTransmission(addr);
    _wire->write(reg);
    _wire->endTransmission();
    _wire->requestFrom(addr, length);
    for (int i = 0; i < length; i++) {
        buffer[index++] = _wire->read();
    }
}

bool UNIT_JOYC::begin(TwoWire *wire, uint8_t addr, uint8_t sda, uint8_t scl,
                      uint32_t speed) {
    _wire  = wire;
    _addr  = addr;
    _sda   = sda;
    _scl   = scl;
    _speed = speed;
    _wire->begin(_sda, _scl, _speed);
    delay(10);
    _wire->beginTransmission(_addr);
    uint8_t error = _wire->endTransmission();
    if (error == 0) {
        return true;
    } else {
        return false;
    }
}

uint16_t UNIT_JOYC::getADCValue(uint8_t index) {

    uint8_t data[4]; // setup a buffer of 4 uint8_t's to store the data

    // In Landscape; with the joystick on top of the screen, the X-axis (left to right) is index 0, and the Y-axis (up and down) is index 1
    if (index > 2) return 0;    // if the index is greater than 2, return 0

    // define the register to read from
    uint8_t reg = index * 2 + ADC_VALUE_REG; // the register is the index * 2 + the ADC_VALUE_REG

    // read the data from the register
    readBytes(_addr, reg, data, 2); // _addr is the I2C address, reg is the register, data is the buffer to store the data, and 2 is the length of the data

    uint32_t value = data[0] | (data[1] << 8);  // combine the first 2 bytes of data into a uint32_t

    return value; // return the values
}

uint16_t UNIT_JOYC::getPOSValue(uint8_t index, minijoyc_pos_read_mode_t bit) {
    uint8_t data[4];
    uint32_t value;

    if (index > 2) return 0;
    if (bit == _10bit) {
        uint8_t reg = index * 2 + POS_VALUE_REG_10_BIT;
        readBytes(_addr, reg, data, 2);
        value = data[0] | (data[1] << 8);
    } else if (bit == _8bit) {
        uint8_t reg = index + POS_VALUE_REG_8_BIT;
        readBytes(_addr, reg, data, 1);
        value = data[0];
    }

    return value;
}

bool UNIT_JOYC::getButtonStatus() {
    uint8_t data;
    readBytes(_addr, BUTTON_REG, &data, 1);
    return data;
}

void UNIT_JOYC::setLEDColor(uint8_t index, uint32_t color) {
    uint8_t data[4];
    data[2] = color & 0xff;
    data[1] = (color >> 8) & 0xff;
    data[0] = (color >> 16) & 0xff;
    writeBytes(_addr, RGB_LED_REG, data, 3);
}

void UNIT_JOYC::setOneCalValue(uint8_t index, uint16_t data) {
    uint8_t reg;
    uint8_t buf[4];

    reg    = index * 2 + CAL_REG;
    buf[0] = data & 0xff;
    buf[1] = (data >> 8) & 0xff;
    writeBytes(_addr, reg, buf, 2);
    delay(1000);
}

void UNIT_JOYC::setAllCalValue(uint16_t *data) {
    writeBytes(_addr, CAL_REG, (uint8_t *)data, 12);
    delay(1000);
}

uint16_t UNIT_JOYC::getCalValue(uint8_t index) {
    if (index > 5) return 0;
    uint8_t data[4];
    uint8_t reg = index * 2 + CAL_REG;
    readBytes(_addr, reg, data, 2);
    uint16_t value = data[0] | (data[1] << 8);
    return value;
}

uint8_t UNIT_JOYC::setI2CAddress(uint8_t addr) {
    _wire->beginTransmission(_addr);
    _wire->write(I2C_ADDRESS_REG);
    _wire->write(addr);
    _wire->endTransmission();
    _addr = addr;
    return _addr;
}

uint8_t UNIT_JOYC::getI2CAddress(void) {
    _wire->beginTransmission(_addr);
    _wire->write(I2C_ADDRESS_REG);
    _wire->endTransmission();

    uint8_t RegValue;

    _wire->requestFrom(_addr, 1);
    RegValue = Wire.read();
    return RegValue;
}

uint8_t UNIT_JOYC::getFirmwareVersion(void) {
    _wire->beginTransmission(_addr);
    _wire->write(FIRMWARE_VERSION_REG);
    _wire->endTransmission();

    uint8_t RegValue;

    _wire->requestFrom(_addr, 1);
    RegValue = Wire.read();
    return RegValue;
}








UNIT_JOYC joyc;

void JoyC_setup(){
    Serial.print("@JoyC_setup");
    joyc.begin();
}


void print_JoyC_raw_values(){
    Serial.print("JoyC_X_raw: ");
    Serial.print(JoyC_X_raw);
    Serial.print(" JoyC_Y_raw: ");
    Serial.print(JoyC_Y_raw);
}

void print_JoyC_center_values(){
    Serial.print("JoyC_X_center: ");
    Serial.print(Joyc_X_center);
    Serial.print(" JoyC_Y_center: ");
    Serial.print(Joyc_Y_center);
}

void print_JoyC_min_max_values(){
    Serial.print("Joyc_X_raw_min: ");
    Serial.print(Joyc_X_raw_min);
    Serial.print(" Joyc_X_max: ");
    Serial.print(Joyc_X_max);
    Serial.print(" Joyc_Y_raw_min: ");
    Serial.print(Joyc_Y_raw_min);
    Serial.print(" Joyc_Y_raw_max: ");
    Serial.println(Joyc_Y_raw_max);
}

void print_JoyC_mapped_values(){
    Serial.print("JoyC_X: ");
    Serial.print(JoyC_X);
    Serial.print(" JoyC_Y: ");
    Serial.println(JoyC_Y);
}

void recalculate_X_center(){
    Joyc_X_center = (JoyC_X_raw + Joyc_X_center + Joyc_X_raw_min + Joyc_X_max) / 4;
}

void recalculate_Y_center(){
    Joyc_Y_center = (JoyC_Y_raw + Joyc_Y_center + Joyc_Y_raw_min + Joyc_Y_raw_max) / 4;
}

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


        if (Warn_User_WiFi_Will_Be_Init && !WiFi_Is_Initialized &&
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

        //Serial.println("menu_active: " + String(menu_active) + " JoyC_needs_to_return_to_center: " + String(JoyC_needs_to_return_to_center) + " JoyC_X_left_right: " + String(JoyC_X_left_right) + " JoyC_Y_up_down: " + String(JoyC_Y_up_down) + " robot_menu_X_selector: " + String(robot_menu_X_selector) + " robot_menu_Y_selector: " + String(robot_menu_Y_selector) + " robot_state: " + String(robot_state) + " robot_msg: " + robot_msg);

        else if(menu_active && !JoyC_needs_to_return_to_center){
            
            if ( JoyC_Y_up_down == JoyC_selector_UP){
                JoyC_Y_up_down = JoyC_selector_CENTER;
                JoyC_needs_to_return_to_center = true;
            //if (JoyC_up){
                if (robot_menu_Y_selector < Robot_menu_max_Y){
                    robot_menu_Y_selector++;

                }
                else {
                    Serial.println("JoyC_up, but robot_menu_Y_selector is already at max");
                }
            }
            else if (JoyC_Y_up_down == JoyC_selector_DOWN){
                JoyC_Y_up_down = JoyC_selector_CENTER;
                JoyC_needs_to_return_to_center = true;
                if (robot_menu_Y_selector > Robot_menu_min_Y){
                    robot_menu_Y_selector--;
                }
                else {
                    Serial.println("JoyC_down, but robot_menu_Y_selector is already at min");
                }
            }
            else if (JoyC_X_left_right == JoyC_selector_LEFT){
                JoyC_X_left_right = JoyC_selector_CENTER;
                JoyC_needs_to_return_to_center = true;
                if (robot_menu_X_selector > Robot_menu_min_X){
                    robot_menu_X_selector--;
                }
            }
            else if (JoyC_X_left_right == JoyC_selector_RIGHT){
                JoyC_X_left_right = JoyC_selector_CENTER;
                JoyC_needs_to_return_to_center = true;
                if (robot_menu_X_selector < Robot_menu_max_X){
                    robot_menu_X_selector++;
                }
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

