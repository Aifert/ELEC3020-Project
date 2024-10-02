// main.cpp
#include <TFT_eSPI.h>
#include <EEPROM.h>
#include <WiFi.h>
#include <esp_now.h>
#include "snakeGame/Snake.h"
#include "reactionGame.h"
#include "main.h"
#include "audioFile.h"


#define EEPROM_SIZE 4096
#define HIGH_SCORE_ADDRESS 0

struct_message myData;
struct_message controller1 = {1, 1, 1, 1, 1, 1};
struct_message controller2 = {2, 1, 1, 1, 1, 1};

// Define Buttons and Switch Pins
// const int buttonA = 0;
// const int buttonB = 14;
const int buttonC = 15;
const int switchPin = 1;
int notSelected = 0;
int highScore = 0;

// Create a struct_message called myData

TFT_eSPI tft = TFT_eSPI();

// Function to print MAC address
void printMacAddress() {
    uint8_t mac[6];
    WiFi.macAddress(mac);
    char macStr[18];
    snprintf(macStr, sizeof(macStr), "%02X:%02X:%02X:%02X:%02X:%02X",
             mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);

    Serial.print("MAC Address: ");
    Serial.println(macStr);

    tft.fillScreen(TFT_BLACK);
    tft.setCursor(0, 0);
    tft.println("MAC Address:");
    tft.println(macStr);
    delay(5000);  // Display for 5 seconds
}

void displayGameOptions() {
    tft.fillScreen(TFT_BLACK);
    String option1 = "Press A for Snake Game";
    String option2 = "Press B for Reaction Game";

    int screenWidth = tft.width();
    int option1Width = tft.textWidth(option1);
    int option2Width = tft.textWidth(option2);

    int option1X = (screenWidth - option1Width) / 2;
    int option2X = (screenWidth - option2Width) / 2;

    int option1Y = 50;
    int option2Y = option1Y + 30;

    tft.setCursor(option1X, option1Y);
    tft.print(option1);

    tft.setCursor(option2X, option2Y);
    tft.print(option2);

    notSelected = 0;
}


void OnDataRecv(const uint8_t * mac_addr, const uint8_t *incomingData, int len) {
    memcpy(&myData, incomingData, sizeof(myData));

    if (myData.id == 1) {
        controller1 = myData;
    } else if (myData.id == 2) {
        controller2 = myData;
    }
    Serial.println();
}

void setup() {
    Serial.begin(115200);
    // pinMode(buttonA, INPUT_PULLUP);
    // pinMode(buttonB, INPUT_PULLUP);
    pinMode(buttonC, INPUT_PULLUP);
    pinMode(switchPin, INPUT_PULLUP);

    WiFi.mode(WIFI_STA);
    if (esp_now_init() != ESP_OK) {
        Serial.println("Error initializing ESP-NOW");
        return;
    }

    esp_now_register_recv_cb(OnDataRecv);

    // Initialize EEPROM
    if (!EEPROM.begin(EEPROM_SIZE)) {
        Serial.println("Failed to initialize EEPROM");
    } else {
        Serial.println("EEPROM initialized");
        EEPROM.get(HIGH_SCORE_ADDRESS, highScore);
        Serial.print("Loaded High Score: ");
        Serial.println(highScore);
    }

    // Initialize TFT Display
    tft.init();
    tft.setRotation(1);
    tft.fillScreen(TFT_BLACK);
    tft.setTextColor(TFT_WHITE, TFT_BLACK);
    tft.setTextSize(2);

    // Print MAC address
    printMacAddress();

    // Display initial game options
    displayGameOptions();
}

void loop() {
    if (controller2.y == 1) {
        displayGameOptions();
    }

    while (!notSelected) {
        if (controller2.y == 0) {
            tft.fillScreen(TFT_BLACK);
            tft.setCursor(10, 10);
            tft.print("Snake Game Selected");

            playCantina();
            setupSnakeGame();

            tft.fillScreen(TFT_BLACK);
            tft.setTextColor(TFT_WHITE, TFT_BLACK);
            tft.setTextSize(2);
            tft.setRotation(1);
            tft.setCursor(10, 40);
            tft.print("Press any button");
            tft.print("to continue");

            // Wait for button release
            while (controller2.y == 0) {
                delay(1500);
            }

            // Wait for a new button press
            bool buttonPressed = false;
            while (!buttonPressed) {
                if (controller2.a == 0 || controller2.b == 0 || controller2.x == 0 || controller2.y == 0 || controller2.big == 0) {
                    buttonPressed = true;
                }
                delay(50);
            }

            notSelected = 1;

            notSelected = 1;
            break;
        } else if (controller2.b == 0) {
            tft.fillScreen(TFT_BLACK);
            tft.setCursor(10, 10);
            tft.print("Reaction Game Selected");

            playCantina();
            runReactionGame();

            // Display message and wait for button press
            tft.fillScreen(TFT_BLACK);
            tft.setCursor(10, 40);
            tft.print("Press any button to continue");

            // Wait for button release
            while (controller2.b == 0) {
                delay(1500);
            }

            // Wait for a new button press
            bool buttonPressed = false;
            while (!buttonPressed) {
                if (controller2.a == 0 || controller2.b == 0 || controller2.x == 0 || controller2.y == 0 || controller2.big == 0) {
                    playCantina();
                    buttonPressed = true;
                }
                delay(50);
            }

            notSelected = 1;
            break;
        }

        delay(100); // Debounce delay
    }

    // Return to main menu
    if (notSelected) {
        displayGameOptions();
    }
}
