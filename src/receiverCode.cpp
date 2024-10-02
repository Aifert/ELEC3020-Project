#include <esp_now.h>
#include <WiFi.h>
#include <TFT_eSPI.h>

TFT_eSPI tft = TFT_eSPI();

// Structure example to receive data
// Must match the sender structure
typedef struct struct_message {
    int id; // must be unique for each sender board
    int big;
    int a;
    int b;
    int x;
    int y;
} struct_message;

// Create a struct_message called myData
struct_message myData;

// Create a structure to hold the readings from each board
struct_message controller1;

struct_message controller2;

// Function to print MAC address on the TFT screen
void printMacAddress() {
    uint8_t mac[6];
    WiFi.macAddress(mac);
    char macStr[18];
    snprintf(macStr, sizeof(macStr), "%02X:%02X:%02X:%02X:%02X:%02X",
             mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);

    tft.fillScreen(TFT_BLACK);
    tft.setCursor(0, 0);
    tft.println("MAC Address:");
    tft.println(macStr);
}

// callback function that will be executed when data is received
void OnDataRecv(const uint8_t * mac_addr, const uint8_t *incomingData, int len) {
  char macStr[18];
  Serial.print("Packet received from: ");
  snprintf(macStr, sizeof(macStr), "%02x:%02x:%02x:%02x:%02x:%02x",
           mac_addr[0], mac_addr[1], mac_addr[2], mac_addr[3], mac_addr[4], mac_addr[5]);
  Serial.println(macStr);
  Serial.println("\n");
  memcpy(&myData, incomingData, sizeof(myData));
  Serial.printf("Board ID %u: %u bytes\n", myData.id, len);

  // Print the received myData values
  Serial.println("Received myData values:");
  Serial.printf("id: %d\n", myData.id);
  Serial.printf("big: %d\n", myData.big);
  Serial.printf("a: %d\n", myData.a);
  Serial.printf("b: %d\n", myData.b);
  Serial.printf("x: %d\n", myData.x);
  Serial.printf("y: %d\n", myData.y);
  Serial.println();

  // Update controller1 with the new incoming data
  if (myData.id == 1) {
    controller1.id = myData.id;
    controller1.big = myData.big;
    controller1.a = myData.a;
    controller1.b = myData.b;
    controller1.x = myData.x;
    controller1.y = myData.y;

    // Print button states when they change
    if (controller1.big == 0) Serial.println("Controller 1 pressed BIG\n");
    if (controller1.a == 0) Serial.println("Controller 1 pressed A\n");
    if (controller1.b == 0) Serial.println("Controller 1 pressed B\n");
    if (controller1.x == 0) Serial.println("Controller 1 pressed X\n");
    if (controller1.y == 0) Serial.println("Controller 1 pressed Y\n");
  } else if (myData.id == 2) {
    controller2.id = myData.id;
    controller2.big = myData.big;
    controller2.a = myData.a;
    controller2.b = myData.b;
    controller2.x = myData.x;
    controller2.y = myData.y;

    // Print button states when they change
    if (controller2.big == 0) Serial.println("Controller 2 pressed BIG\n");
    if (controller2.a == 0) Serial.println("Controller 2 pressed A\n");
    if (controller2.b == 0) Serial.println("Controller 2 pressed B\n");
    if (controller2.x == 0) Serial.println("Controller 2 pressed X\n");
    if (controller2.y == 0) Serial.println("Controller 2 pressed Y\n");
  }
}

void setup() {
  //Initialise screen to draw
  tft.init();
  tft.setTextSize(2);
  tft.setTextDatum(MC_DATUM);
  tft.fillScreen(TFT_BLACK);
  tft.setTextColor(TFT_WHITE);

  //Initialize Serial Monitor
  Serial.begin(115200);

  //Set device as a Wi-Fi Station
  WiFi.mode(WIFI_STA);

  //Init ESP-NOW
  if (esp_now_init() != ESP_OK) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }

  // Once ESPNow is successfully Init, we will register for recv CB to
  // get recv packer info
  esp_now_register_recv_cb(esp_now_recv_cb_t(OnDataRecv));

  // Print MAC address on the TFT screen
  printMacAddress();
}

void loop() {
  // The loop is now empty as we're handling button presses in the callback
}
