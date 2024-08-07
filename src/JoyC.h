#ifndef _JOYC_H
#define _JOYC_H

#include "Arduino.h"
#include "Wire.h"
#include "variables.h"

#define JoyC_ADDR            0x54
#define ADC_VALUE_REG        0x00
#define POS_VALUE_REG_10_BIT 0x10
#define POS_VALUE_REG_8_BIT  0x20
#define BUTTON_REG           0x30
#define RGB_LED_REG          0x40
#define CAL_REG              0x50
#define FIRMWARE_VERSION_REG 0xFE
#define I2C_ADDRESS_REG      0xFF

#define LIGHT_BLUE    0x7D7C
#define SKY_BLUE      0x6DBE
#define DEEP_SKY_BLUE 0x05FF
#define DODGER_BLUE   0x1C9F
#define MEDIUM_BLUE   0x001B

#define CAL_MODE_STOP   0
#define CAL_MODE_AUTO   1
#define CAL_MODE_MANUAL 2

typedef enum { _8bit = 0, _10bit } minijoyc_pos_read_mode_t;

class UNIT_JOYC {
   private:
    uint8_t _addr;
    TwoWire* _wire;
    uint8_t _scl;
    uint8_t _sda;
    uint8_t _speed;
    void writeBytes(uint8_t addr, uint8_t reg, uint8_t* buffer, uint8_t length);
    void readBytes(uint8_t addr, uint8_t reg, uint8_t* buffer, uint8_t length);

   public:
    bool begin(TwoWire* wire = &Wire, uint8_t addr = JoyC_ADDR,
               uint8_t sda = 21, uint8_t scl = 22, uint32_t speed = 200000L);
    uint16_t getADCValue(uint8_t index);
    uint16_t getPOSValue(uint8_t index, minijoyc_pos_read_mode_t bit);
    void setOneCalValue(uint8_t index, uint16_t data);
    void setAllCalValue(uint16_t* data);
    uint16_t getCalValue(uint8_t index);
    uint8_t getCALMode(void);
    bool getButtonStatus();
    void setLEDColor(uint8_t index, uint32_t color);
    // void setStepValue(uint8_t step);
    // uint8_t getStepValue(void);
    uint8_t setI2CAddress(uint8_t addr);
    uint8_t getI2CAddress(void);
    uint8_t getFirmwareVersion(void);
    void resetCounter(void);
};

extern UNIT_JOYC joyc;

void cartesianToPolar(byte *x, byte *y, float *r, float *phi);                                       

void JoyC_setup();

void set_JoyC_LED_color(uint32_t color);

void print_JoyC_min_max_values();
void print_JoyC_center_values();
void print_JoyC_mapped_values();
void recalculate_X_center();
void recalculate_Y_center();

#endif