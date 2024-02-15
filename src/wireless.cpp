
#include <Arduino.h>

#include "creds.h"

#include "variables.h"

#include <esp_now.h>

#include <ESPmDNS.h>

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
    Serial.println(Remote_Name);

    Serial.print(" psswd: ");
    Serial.println(AP_password);

    //WiFi.softAP(AP_ssid);
    WiFi.softAP(Remote_Name, AP_password);


    if (!MDNS.begin(Remote_Name)) { // Start the mDNS responder for 
      Serial.println("Error setting up MDNS responder!");
    } else {
      Serial.println("mDNS responder started, device accessable at: " + String(Remote_Name) + ".local");
      // You can now reach your device using "marin.local" in the browser
    }


    Serial.println("Starting WebSerial...");  
    WebSerial.begin(&server);
    /* Attach Message Callback */
    WebSerial.msgCallback(recvMsg);

    server.begin();

    Serial.print("WebSerial accessable at: http://"+ String(Remote_Name) + ".local/webserial\n");


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




void sendData() {
  // Structure and data to send as before
  struct_message myData;

  // Create a string formatted as (+/-)XX(+/-)YY

  String message = "c";

  if (JoyC_X < 10) {
    message += "+0";
  }
  else {
    message += "+";
  }

  message += String(JoyC_X);

  if (JoyC_Y < 10) {
    message += "+0";
  }
  else {
    message += "+";
  }

  message += String(JoyC_Y);

  //strcpy(myData.a, (if (x < 0) ? "" : "+") + String(x) + (if (y < 0) ? "" : "+") + String(y));

  // Move message to myData
  message.toCharArray(myData.a, 8);

  // Send message via ESP-NOW
  esp_err_t result = esp_now_send(broadcastAddress, (uint8_t *) &myData, sizeof(myData));
  
  if (result == ESP_OK) {
    Serial.println("Sent with success, message: " + String(myData.a));
  }
  else {
    Serial.println("Error sending the data");
  }
}





void convertMacAddress(const String &macStr, uint8_t *macAddr) {
    // Assumes macStr is in the format "XX:XX:XX:XX:XX:XX"
    sscanf(macStr.c_str(), "%hhx:%hhx:%hhx:%hhx:%hhx:%hhx", 
           &macAddr[0], &macAddr[1], &macAddr[2], &macAddr[3], &macAddr[4], &macAddr[5]);
}


// returns dict(map) of SSIDs with ssid, MAC, and RSSI
void get_ssids(){

  robot_wifi_in_range = false;
  
  n_WiFi_Networks = WiFi.scanNetworks(); // Get the number of networks found

  Serial.println("\n-------------------------------------------------");

  Serial.println("Found " + String(n_WiFi_Networks) + " networks\n");


  for (int i = 0; i < n_WiFi_Networks; i++) { // Loop through each network

    

    // Network network;
    // network.ssid = WiFi.SSID(i);
    // network.mac = WiFi.BSSIDstr(i);
    // network.rssi = WiFi.RSSI(i);
    // WiFi_Networks.push_back(network);
    // Serial.println(network.ssid + " " + network.mac + " " + String(network.rssi));

    if (WiFi.SSID(i) == ssid) {
      Serial.print("\n(Main WiFi)  ");
      //Serial.println(WiFi.SSID(i) + " " + WiFi.BSSIDstr(i) + " " + String(WiFi.RSSI(i)) + "\n");
      Serial.println(String(WiFi.RSSI(i)) + " " + WiFi.BSSIDstr(i) + " " + WiFi.SSID(i) + "\n");

    }
    else if (WiFi.SSID(i) == Remote_Name) {
      Serial.print("\n(Remote_Name)");
      //Serial.println(WiFi.SSID(i) + " " + WiFi.BSSIDstr(i) + " " + String(WiFi.RSSI(i))+ "\n");
      Serial.println(String(WiFi.RSSI(i)) + " " + WiFi.BSSIDstr(i) + " " + WiFi.SSID(i) + "\n");


      WiFi_With_Remote_Name_Found = true;
    }
    // if WiFi.SSID contains starts with 'MARIN' and ends with '-robot'
    else if (WiFi.SSID(i).startsWith("MARIN") && WiFi.SSID(i).endsWith("-robot")) {
      if (Robot_MAC == "-") {
        Robot_MAC = WiFi.BSSIDstr(i);
      }
      if (Robot_ssid == "-") {
        Robot_ssid = WiFi.SSID(i);
      }
      robot_wifi_in_range = true;
      Serial.print("\n(Robot WiFi) ");
      //Serial.println(WiFi.SSID(i) + " " + WiFi.BSSIDstr(i) + " " + String(WiFi.RSSI(i))+ "\n");
      Serial.println(String(WiFi.RSSI(i)) + " " + WiFi.BSSIDstr(i) + " " + WiFi.SSID(i) + "\n");
    }
    else {
      Serial.print(  "(Unknown)    ");
      //Serial.println(WiFi.SSID(i) + " " + WiFi.BSSIDstr(i) + " " + String(WiFi.RSSI(i)));
      Serial.println(String(WiFi.RSSI(i)) + " " + WiFi.BSSIDstr(i) + " " + WiFi.SSID(i));
    }

  }

  Serial.println("-------------------------------------------------\n");

  if (robot_wifi_in_range) {
    Serial.println("SUCCESS: Robot WiFi found :D Robot_ssid: " + Robot_ssid + " Robot_MAC: " + Robot_MAC + "\n");
    // Convert Robot_MAC String to byte array
    convertMacAddress(Robot_MAC, broadcastAddress);

    
    // Setup ESPNOW peer
    esp_now_peer_info_t peerInfo;
    memcpy(peerInfo.peer_addr, broadcastAddress, 6);
    peerInfo.channel = 0;
    peerInfo.encrypt = false;

    // Add peer
    if (esp_now_add_peer(&peerInfo) != ESP_OK){
      Serial.println("Failed to add peer");
      return;
    }

    Serial.println("Peer added");

    robot_connected = true;

  }


  if (WiFi_With_Remote_Name_Found) {
    Serial.println("ERROR: WiFi With same name as remote name found, please change the remote name in creds.h\n");
  }
  else {
   
  }



}