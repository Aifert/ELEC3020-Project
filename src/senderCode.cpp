// #include <esp_now.h>
// #include <WiFi.h>
// #include <TFT_eSPI.h>
// #include <LittleFS.h>

// #define buttonBig 1
// #define buttonA 11
// #define buttonB 2
// #define buttonX 3
// #define buttonY 10
// #define powerEnable 15

// // REPLACE WITH THE RECEIVER'S MAC Address
// // uint8_t broadcastAddress[] = {0x30, 0x30, 0xf9, 0x59, 0xe2, 0x90};
// uint8_t broadcastAddress[] = {0x30, 0x30, 0xf9, 0x59, 0xe2, 0x90};
// // My Mac Address is 0x30, 0x30, 0xf9, 0x59, 0xe2, 0x90

// // Structure example to send data
// // Must match the receiver structure
// typedef struct struct_message {
//     int id; // must be unique for each sender board
//     int big;
//     int a;
//     int b;
//     int x;
//     int y;
// } struct_message;

// // Create a struct_message called myData
// struct_message myData = {2, 1, 1, 1, 1, 1};

// // Create peer interface
// esp_now_peer_info_t peerInfo;

// // TFT display object
// TFT_eSPI tft = TFT_eSPI();

// // callback when data is sent
// void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status) {
//   Serial.print("\r\nLast Packet Send Status:\t");
//   Serial.println(status == ESP_NOW_SEND_SUCCESS ? "Delivery Success" : "Delivery Fail");
// }

// void setup() {

//   pinMode(powerEnable, OUTPUT);
//   pinMode(buttonBig, INPUT);
//   pinMode(buttonA, INPUT);
//   pinMode(buttonB, INPUT);
//   pinMode(buttonX, INPUT);
//   pinMode(buttonY, INPUT);

//   digitalWrite(powerEnable, HIGH);
//   // Init Serial Monitor
//   Serial.begin(115200);

//   // Initialize TFT display
//   tft.init();
//   tft.setRotation(1);
//   tft.fillScreen(TFT_BLACK);
//   tft.setTextColor(TFT_WHITE, TFT_BLACK);
//   tft.setTextSize(2);

//   // Print player 1 on the screen
//   tft.setCursor(100, 50);  // Set cursor position
//   tft.println("Player 2");

//   // Set device as a Wi-Fi Station
//   WiFi.mode(WIFI_STA);

//   // Init ESP-NOW
//   if (esp_now_init() != ESP_OK) {
//     Serial.println("Error initializing ESP-NOW");
//     tft.println("Error initializing ESP-NOW");
//     return;
//   }

//   // Once ESPNow is successfully Init, we will register for Send CB to
//   // get the status of Trasnmitted packet
//   esp_now_register_send_cb(OnDataSent);

//   // Register peer
//   memcpy(peerInfo.peer_addr, broadcastAddress, 6);
//   peerInfo.channel = 0;
//   peerInfo.encrypt = false;

//   // Add peer
//   if (esp_now_add_peer(&peerInfo) != ESP_OK){
//     Serial.println("Failed to add peer");
//     tft.println("Failed to add peer");
//     return;
//   }

//   Serial.println("Setup complete");
// }

// void loop() {
//   esp_err_t result;
//   bool dataChanged = false;

//   if (digitalRead(buttonBig) != myData.big) {
//     myData.big = digitalRead(buttonBig);
//     dataChanged = true;
//   }
//   if (digitalRead(buttonA) != myData.a) {
//     myData.a = digitalRead(buttonA);
//     dataChanged = true;
//   }
//   if (digitalRead(buttonB) != myData.b) {
//     myData.b = digitalRead(buttonB);
//     dataChanged = true;
//   }
//   if (digitalRead(buttonX) != myData.x) {
//     myData.x = digitalRead(buttonX);
//     dataChanged = true;
//   }
//   if (digitalRead(buttonY) != myData.y) {
//     myData.y = digitalRead(buttonY);
//     dataChanged = true;
//   }

//   // Send message via ESP-NOW only if data has changed
//   if (dataChanged) {
//     Serial.println("big: " + String(myData.big));
//     Serial.println("a: " + String(myData.a));
//     Serial.println("b: " + String(myData.b));
//     Serial.println("x: " + String(myData.x));
//     Serial.println("y: " + String(myData.y));

//     result = esp_now_send(broadcastAddress, (uint8_t *) &myData, sizeof(myData));
//   }
// }
