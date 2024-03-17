// --- wireless.cpp ---
#include "wireless.h" 

#define msg_str_len 64
// Structure example to receive data
// Must match the sender structure
typedef struct struct_message {
    byte i;
    char a[msg_str_len];
} struct_message;

typedef struct struct_message_s {
    byte i;
    char a[msg_str_len];
} struct_message_r;

AsyncWebServer server(80);

// Create a struct_message called myData
struct_message myData;
struct_message_r RmyData;

/* Message callback of WebSerial */
void WebSerialrecvMsg(uint8_t *data, size_t len){
  RED_LED(1);
  //WebSerial.println("Received Data...");

  String d = "";
  for(int i=0; i < len; i++){
    d += char(data[i]);
  }
  
  Serial.println(d);
  RED_LED(0);
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





# define command_index 0  // c


# define x_sign_index 1   // + or -

# define isArmed_sign_index 1  // 1 or 0
# define isArmed_index 2  // 1 or 0

# define perCentBatt_p10_index 3    // 0-9
# define perCentBatt_p1_index 4     // 0-9




# define x_p10_index 2    // 0-9
# define x_p1_index 3     // 0-9


# define y_sign_index 4   // + or -

# define y_p10_index 5    // 0-9
# define y_p1_index 6     // 0-9

# define roll_sign_index 7   // + or -

# define roll_p100_index 8    // 0-9
# define roll_p10_index 9    // 0-9
# define roll_p1_index 10     // 0-9

# define pitch_sign_index 11   // + or -

# define pitch_p100_index 12   // 0-9
# define pitch_p10_index 13    // 0-9
# define pitch_p1_index 14     // 0-9

# define Lmotor_sign_index 15   // + or -

# define Lmotor_p100_index 16   // 0-9
# define Lmotor_p10_index 17    // 0-9
# define Lmotor_p1_index 18     // 0-9

# define Rmotor_sign_index 19   // + or -

# define Rmotor_p100_index 20   // 0-9
# define Rmotor_p10_index 21    // 0-9
# define Rmotor_p1_index 22     // 0-9


# define Background_CPU_sign_index 23   // + or -

# define Background_CPU_p10_index 24   // 0-9
# define Background_CPU_p1_index 25    // 0-9


# define RealT_CPU_CPU_sign_index 26   // + or -

# define RealT_CPU_CPU_p10_index 27   // 0-9
# define RealT_CPU_CPU_p1_index 28    // 0-9



// # define RealT_CPU_CPU_sign_index 23   // + or -

// # define RealT_CPU_CPU_p10_index 24   // 0-9
// # define RealT_CPU_CPU_p1_index 25    // 0-9

// # define Background_CPU_sign_index 26   // + or -

// # define Background_CPU_p10_index 27   // 0-9
// # define Background_CPU_p1_index 28    // 0-9



String msg_str = "";

void processCharArray() {

  msg_str = robot_msg;

  if(robot_msg == "ARMED") {
      robot_state = ROBOT_ARMED;
      // isArmed = true;
      // robot_ARM_requested = false;
      return;
  }

  else if (robot_state == ROBOT_ARMING){
      Serial.println("ERROR: ARM request failed");
  }


  else if(robot_msg == "DISARMED") {
      robot_state = ROBOT_DISARMED;
      //isArmed = false;
      //robot_DISARM_requested = false;
      return;
  }

  else if (robot_state == ROBOT_DISARMING){
      Serial.println("ERROR: DISARM request failed");
  }

  else if(robot_msg == "TAKING_OFF") {
      robot_state = TAKING_OFF;
      return;

  }

  else if(robot_state == TAKEOFF_REQUESTED) {
      Serial.println("ERROR: Takeoff request failed");
  }


  /*EXAMPLE DATA:
  //SPACES ADDED FOR CLARITY

  'c' commands; includes info that needs to be updated quickly like joystick and IMU data
  c       +50+50+000+060+000+000+74+11
  c+50+50+007+029+000+000+73+11


  's' commands; includes info that needs to be updated slowly like armedStatus, batteryPerCent 
  only send every 100th msg
  s +0+07 +50+50+000+060+000+000+73+11 
  s+0+89+50+50+007+029+000+000+73+12


  s+0+89+50+50+007+029+000+000+73+12
  c9+50+50+007+029+000+000+73+12
  c+50+50+007+029+000+000+74+11
    
  */

  if (robot_msg[command_index] == 's') {

    //decode isArmed
    isArmed = (msg_str[isArmed_index] == '1') ? true : false;

    //decode perCentBatt
      perCentBatt = (msg_str[perCentBatt_p10_index] - '0') * 10 + (msg_str[perCentBatt_p1_index] - '0');

    //set msg_str to 'c' + the rest of the string so that the rest of the data can be processed
    msg_str = "c" + robot_msg.substring(perCentBatt_p1_index + 2, msg_str_len);

    // print the updated msg_str

    Serial.print("msg_str: " + msg_str);
  }
    
  if (msg_str[command_index] == 'c') {
    
    // Process the first 2 chars to get the sign of the X value
    Robot_JoyC_X = (msg_str[x_p10_index]- '0') * 10 + (msg_str[x_p1_index] - '0');  

    // Process the following 2 chars to get the sign of the Y value
    Robot_JoyC_Y = (msg_str[y_p10_index] - '0') * 10 + (msg_str[y_p1_index] - '0');

    //Avg_IMU_Z_deg_per_sec = (msg_str[8] - '0') * 100;
    Avg_IMU_Z_deg_per_sec = ((msg_str[roll_p100_index] - '0') * 100 + (msg_str[roll_p10_index] - '0') * 10 + (msg_str[roll_p1_index] - '0')) * ((msg_str[roll_sign_index] == '-') ? -1 : 1);

    robot_Y_deg = ((msg_str[pitch_p100_index] - '0') * 100 + (msg_str[pitch_p10_index] - '0') * 10 + (msg_str[pitch_p1_index] - '0')) * ((msg_str[pitch_sign_index] == '-') ? -1 : 1);

    Lmotor = ((msg_str[Lmotor_p100_index] - '0') * 100 + (msg_str[Lmotor_p10_index] - '0') * 10 + (msg_str[Lmotor_p1_index] - '0')) * ((msg_str[Lmotor_sign_index] == '-') ? -1 : 1);

    Rmotor = ((msg_str[Rmotor_p100_index] - '0') * 100 + (msg_str[Rmotor_p10_index] - '0') * 10 + (msg_str[Rmotor_p1_index] - '0')) * ((msg_str[Rmotor_sign_index] == '-') ? -1 : 1);

    Robot_RealTcode_CPU_load = ((msg_str[RealT_CPU_CPU_p10_index] - '0') * 10 + (msg_str[RealT_CPU_CPU_p1_index] - '0'));

    Robot_BackgroundTask_CPU_load = ((msg_str[Background_CPU_p10_index] - '0') * 10 + (msg_str[Background_CPU_p1_index] - '0'));
    
    
    cartesianToPolar(&Robot_JoyC_X, &Robot_JoyC_Y, &Robot_JoyC_r, &Robot_JoyC_Phi);

    // Serial.print("X: ");
    // Serial.print(Robot_JoyC_X); 
    // Serial.print("    Y: ");
    // Serial.print(Robot_JoyC_Y);
    // Serial.print("    R: ");
    // Serial.print(Avg_IMU_Z_deg_per_sec);
    // Serial.print("    P: ");
    // Serial.print(robot_Y_deg);
    // Serial.print("    L: ");
    // Serial.print(Lmotor);
    // Serial.print("    R: ");
    // Serial.print(Rmotor);

    Serial.println();

  }
  
}




void print_msg_from_mac_to_serial(const uint8_t * mac, String robot_msg, int *len) {


  byte str_len = robot_msg.substring(0, robot_msg.indexOf('\0')).length();




  //Serial.print("\n\n'");
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
  Serial.print(str_len);
  //Serial.print(*len); // dereference the 'len' ptr to get the lenght of the incoming data
  Serial.print("/"+String(msg_str_len));
  Serial.print("): ");


  // print the incoming data to the serial monitor
  Serial.print(robot_msg.substring(0, str_len));



  // for (int i = 0; i < *len; i++) { // loop through the incoming data
    
  //   //Serial.print(" ");
  //   Serial.print(char(incomingData[i])); // print each char of the incoming data

  //   //check if end of str 
  //   if (incomingData[i] == '\0') {
  //     break;
  //   }
  // }


  Serial.println();
  Serial.println("\n\n");

  
  

}

// callback function that will be executed when data is received
void OnDataRecv(const uint8_t * mac, const uint8_t *incomingData, int len) {

  RED_LED(1);

  // print the incoming data to the serial monitor
  //print_msg_from_mac_to_serial(mac, incomingData, &len);


  

  memcpy(&RmyData, incomingData, sizeof(RmyData));

  robot_msg = RmyData.a;

  // Serial.print("Bytes received: ");
  // Serial.println(len);
  // Serial.print("Char: ");
  // Serial.println(myData.a);

  // Serial.print("X: ");
  // Serial.print(x);
  // Serial.print("    Y: ");
  // Serial.println(y);


  Serial.print("#" + String(RmyData.i) + "  ");

  print_msg_from_mac_to_serial(mac, robot_msg, &len);

  processCharArray(); // Call the function to process the received data

  RED_LED(0);
}


void Wireless_Setup(){
//void Wireless_Setup( void * pvParameters ){
  RED_LED(1);

    //print the MAC address of the device
    Serial.print("MAC Address: ");
    Serial.println(WiFi.macAddress());

    WiFi.mode(WIFI_AP_STA); 
    WiFi.begin(ssid, password);

    if (WiFi.waitForConnectResult() != WL_CONNECTED) {
        Serial.printf("WiFi Failed!\n");
        //WiFi_connected = false;
        WiFi_State = WIFI_DISCONNECTED;
    }
    else {

      //WiFi_connected = true;
      WiFi_State = WIFI_CONNECTED;
  
      Serial.print("Connected to WiFi '");
      Serial.print(ssid);
      Serial.print("' with IP: ");
      Serial.println(WiFi.localIP());
      // WebSerial is accessible at "<IP Address>/webserial" in browser

    }

    
    Serial.print("Starting WiFI AP: ");
    Serial.println(Remote_Name);

    // Serial.print(" psswd: ");
    // Serial.println(AP_password);

    //WiFi.softAP(AP_ssid);
    //WiFi.softAP(Remote_Name, AP_password);
    WiFi.softAP(Remote_Name);


    if (!MDNS.begin(Remote_Name)) { // Start the mDNS responder for 
      Serial.println("Error setting up MDNS responder!");
    } else {
      Serial.println("mDNS responder started, device accessable at: " + String(Remote_Name) + ".local");
      // You can now reach your device using "marin.local" in the browser
    }


    Serial.println("Starting WebSerial...");  
    WebSerial.begin(&server);
    /* Attach Message Callback */
    WebSerial.msgCallback(WebSerialrecvMsg);

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


   

    //WiFi_Is_Initialized = true;
    //WiFi_Is_Initializing = false;
    WiFi_State = WIFI_INITIALIZED;

    RED_LED(0);

    

    return;
}


#define PRINT_SENT 1

byte i_msg = 0;

void sendData() {

  i_msg++;

  // Structure and data to send as before
  struct_message myData;

  String message = "";

  // Create a string formatted as (+/-)XX(+/-)YY

  if (menu_active){
    return;
  }
  else if (robot_state == ROBOT_ARMING) {
    message = "ARM";
  }
  else if (robot_state == ROBOT_DISARMING) {
    message = "DISARM";
  }
  else if (robot_state == TAKEOFF_REQUESTED) {
    message = "REQUEST_TAKEOFF";
  }
  else {
    message += "c";

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

  }

  

  
  //strcpy(myData.a, (if (x < 0) ? "" : "+") + String(x) + (if (y < 0) ? "" : "+") + String(y));

  // Move message to myData
  message.toCharArray(myData.a, msg_str_len);

  myData.i = i_msg;

  // Send message via ESP-NOW
  esp_err_t result = esp_now_send(broadcastAddress, (uint8_t *) &myData, sizeof(myData));
  
  if (result == ESP_OK) {
    #if PRINT_SENT
    Serial.println("Sent with success, message: " + String(myData.a));
    #endif
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

  menu_active = true;
  JoyC_Xinput = true;
  menu_X_selector = WIFI_MENU;
  LCD_flush();

}

// --- wireless.cpp ---