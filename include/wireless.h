#ifndef _WIRELESS_H_
#define _WIRELESS_H_

//#include <Vector.h>
#include <variables.h>
#include "JoyC.h"

#include <Arduino.h>

#include "creds.h"

#include "variables.h"

#include <esp_now.h>

#include <ESPmDNS.h>

#include "LCD.h"

#if defined(ESP8266)
  #include <ESP8266WiFi.h>
  #include <ESPAsyncTCP.h>
#elif defined(ESP32)
  #include <WiFi.h>
  #include <AsyncTCP.h>
#endif

#include <ESPAsyncWebServer.h>
#include <WebSerial.h>
//#include <Vector.h>
#include "IO.h"

// Function declarations
void Wireless_Setup();
//                                                                                                                                                                                                                                                                                                                                                                                                                                                  void Wireless_Setup( void * pvParameters );
void WebSerialrecvMsg(uint8_t *data, size_t len);
void processCharArray();
void OnDataRecv(const uint8_t * mac, const uint8_t *incomingData, int len);
void get_ssids();

void sendData();


#endif // _WIRELESS_H_
