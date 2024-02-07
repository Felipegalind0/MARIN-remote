
#include <Arduino.h>

#include "creds.h"

#include "variables.h"

#include <esp_now.h>

#if defined(ESP8266)
  #include <ESP8266WiFi.h>
  #include <ESPAsyncTCP.h>
#elif defined(ESP32)
  #include <WiFi.h>
  #include <AsyncTCP.h>
#endif
#include <ESPAsyncWebServer.h>
#include <WebSerial.h>
#include <Vector.h>

// Structure example to receive data
// Must match the sender structure
typedef struct struct_message {
    char a[7];
} struct_message;

AsyncWebServer server(80);

// Create a struct_message called myData
struct_message myData;

/* Message callback of WebSerial */
void recvMsg(uint8_t *data, size_t len){
  //WebSerial.println("Received Data...");

  String d = "";
  for(int i=0; i < len; i++){
    d += char(data[i]);
  }
  
  //WebSerial.println(d);
}

// void processCharArray() {
//   if (myData.a[1] == '-' || myData.a[1] == '+') {
//     if (myData.a[2] == '0' || myData.a[2] == '1') {
//       if (myData.a[3] >= '0' && myData.a[3] <= '9') {
//         int signX = (myData.a[1] == '-') ? -1 : 1;
//         int digit1X = myData.a[2] - '0';
//         int digit2X = myData.a[3] - '0';
//         x = signX * (digit1X * 10 + digit2X);
//       }
//     }
//   }
//   if (myData.a[4] == '-' || myData.a[4] == '+') {
//     if (myData.a[5] == '0' || myData.a[5] == '1') {
//       if (myData.a[6] >= '0' && myData.a[6] <= '9') {
//         int signY = (myData.a[4] == '-') ? -1 : 1;
//         int digit1Y = myData.a[5] - '0';
//         int digit2Y = myData.a[6] - '0';
//         y = signY * -(digit1Y * 10 + digit2Y);
//       }
//     }
//   }

//   // Update the rotation and movement of the robot based on the X and Y values
//   Movement_UpdateRotation(x);
//   Movement_UpdateMovement(y);
// }

void print_msg_from_mac_to_serial(const uint8_t * mac, const uint8_t *incomingData, int *len) {
  Serial.print("\n\n'");
  // print the incoming mac address
  Serial.print(mac[0], HEX);
  Serial.print(":");
  Serial.print(mac[1], HEX);
  Serial.print(":");
  Serial.print(mac[2], HEX);
  Serial.print(":");
  Serial.print(mac[3], HEX);
  Serial.print(":");
  Serial.print(mac[4], HEX);
  Serial.print(":");
  Serial.print(mac[5], HEX);
  Serial.print("' sent(");
  Serial.print(*len); // dereference the 'len' ptr to get the lenght of the incoming data
  Serial.println("): ");
  for (int i = 0; i < *len; i++) { // loop through the incoming data
    Serial.print(char(incomingData[i]));
    Serial.print(" ");
  }

}

// callback function that will be executed when data is received
void OnDataRecv(const uint8_t * mac, const uint8_t *incomingData, int len) {

  //memcpy(&myData, incomingData, sizeof(myData));

  // Serial.print("Bytes received: ");
  // Serial.println(len);
  // Serial.print("Char: ");
  // Serial.println(myData.a);

  //processCharArray(); // Call the function to process the received data

  // Serial.print("X: ");
  // Serial.print(x);
  // Serial.print("    Y: ");
  // Serial.println(y);


  //print_msg_from_mac_to_serial(mac, incomingData, &len);

  
}

void Wireless_Setup(){

    WiFi.mode(WIFI_AP_STA); 
    WiFi.begin(ssid, password);
    if (WiFi.waitForConnectResult() != WL_CONNECTED) {
        Serial.printf("WiFi Failed!\n");
        WiFi_connected = false;
    }
    else {

      WiFi_connected = true;
  
      Serial.print("Connected to WiFi '");
      Serial.print(ssid);
      Serial.print("' with IP: ");
      Serial.println(WiFi.localIP());
      // WebSerial is accessible at "<IP Address>/webserial" in browser

    }

    
    Serial.print("Starting WiFI AP: ");
    Serial.println(AP_ssid);

    Serial.print(" psswd: ");
    Serial.println(AP_password);

    //WiFi.softAP(AP_ssid);
    WiFi.softAP(AP_ssid, AP_password);


    Serial.println("Starting WebSerial...");
    WebSerial.begin(&server);
    /* Attach Message Callback */
    WebSerial.msgCallback(recvMsg);
    server.begin();


    Serial.print("Initializing ESP-NOW");
    // Init ESP-NOW
    if (esp_now_init() != ESP_OK) {
      Serial.println("Error initializing ESP-NOW");
      return;
    }

    // Once ESPNow is successfully Init, we will register for recv CB to
    // get recv packer info
    Serial.print("Registering Data Receive Callback");
    esp_now_register_recv_cb(OnDataRecv);

}
// returns dict(map) of SSIDs with ssid, MAC, and RSSI
Vector<Network> get_ssids(){
  int n = WiFi.scanNetworks();

  Vector<Network> networks;

  for (int i = 0; i < n; ++i) {
    Network network;
    network.ssid = WiFi.SSID(i);
    network.mac = WiFi.BSSIDstr(i);
    network.rssi = WiFi.RSSI(i);
    networks.push_back(network); // Add the network to the vector
    //Serial.println(network.ssid + " " + network.mac + " " + String(network.rssi));
  }

  // return networks;
  return networks;
}