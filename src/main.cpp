// main.cpp
#include <TFT_eSPI.h>
#include <EEPROM.h>
#include <WiFi.h>
#include <esp_now.h>
#include "snakeGame/Snake.h"
#include "reactionGame.h"
#include "main.h"
#include "audioFile.h"
#include "PongGame.h"
#include "ESP32S3VGA.h"
#include <GfxWrapper.h>

#define EEPROM_SIZE 4096
#define HIGH_SCORE_ADDRESS 0

struct_message myData;
struct_message controller1 = {1, 1, 1, 1, 1, 1};
struct_message controller2 = {2, 1, 1, 1, 1, 1};

// Define Buttons and Switch Pins
const int switchPin = 1;
const int powerEnable = 15;
int notSelected = 0;
int highScore = 0;

// VGA setup
const PinConfig pins(-1, -1, -1, -1, 43, -1, -1, -1, -1, -1, 44, -1, -1, -1, -1, 18, 1, 2);
TFT_eSPI tft = TFT_eSPI();
VGA vga;
Mode mode = Mode::MODE_320x240x60;
GfxWrapper<VGA>* gfx = nullptr;

// Function to print MAC address
void printMacAddress() {
    uint8_t mac[6];
    WiFi.macAddress(mac);
    char macStr[18];
    snprintf(macStr, sizeof(macStr), "%02X:%02X:%02X:%02X:%02X:%02X",
             mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);

    Serial.print("MAC Address: ");
    Serial.println(macStr);

    vga.clear(vga.rgb(0x00, 0x00, 0x00));  // Clear screen (black background)
    gfx->setCursor(0, 0);
    gfx->setTextColor(vga.rgb(255, 255, 255));
    gfx->println("MAC Address:");
    gfx->println(macStr);
    vga.show();
    delay(5000);  // Display for 5 seconds
}

void displayGameOptions() {
    gfx->setTextSize(1);
    vga.clear(vga.rgb(0x00, 0x00, 0x00));  // Clear screen (black background)
    String option1 = "Press A for Pong Game";
    String option2 = "Press B for Reaction Game";
    String option3 = "Press Y for Snake Game";

    int screenWidth = mode.hRes;
    int option1Y = 50;
    int option2Y = option1Y + 30;
    int option3Y = option2Y + 30;

    gfx->setTextColor(vga.rgb(255, 255, 255));

    // Calculate text width manually
    int option1Width = option1.length() * 8; // Assuming 8 pixels per character
    int option2Width = option2.length() * 8;
    int option3Width = option3.length() * 8;

    gfx->setCursor((screenWidth - option1Width) / 2, option1Y);
    gfx->print(option1);

    gfx->setCursor((screenWidth - option2Width) / 2, option2Y);
    gfx->print(option2);

    gfx->setCursor((screenWidth - option3Width) / 2, option3Y);
    gfx->print(option3);

    vga.show();
    notSelected = 0;

    // Display on LilyGo TFT
    tft.fillScreen(TFT_BLACK);
    tft.setTextColor(TFT_WHITE);
    tft.setTextSize(2);
    tft.setCursor(0, 0);
    tft.println(option1);
    tft.println(option2);
    tft.println(option3);
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
    pinMode(switchPin, INPUT_PULLUP);
    pinMode(powerEnable, OUTPUT);

    digitalWrite(powerEnable, HIGH);

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

    // Initialize VGA
    vga.bufferCount = 2;
    if (!vga.init(pins, mode, 16)) {
        while (1) delay(1); // Halt if VGA initialization fails
    }
    vga.start();
    gfx = new GfxWrapper<VGA>(vga, mode.hRes, mode.vRes);  // GfxWrapper initialization
    Serial.println("VGA initialized");

    // Initialize LilyGo TFT
    tft.init();
    tft.setRotation(1);

    // Print MAC address
    printMacAddress();

    // Display initial game options
    displayGameOptions();
}

void loop() {
    if (controller2.y == 1 || controller1.y == 1) {
        displayGameOptions();
    }

    while (!notSelected) {
        if (controller2.y == 0 || controller1.y == 0) {
            vga.clear(vga.rgb(0x00, 0x00, 0x00));
            gfx->setCursor(10, 10);
            gfx->setTextColor(vga.rgb(255, 255, 255));
            gfx->print("Snake Game Selected");
            vga.show();

            tft.fillScreen(TFT_BLACK);
            tft.setCursor(0, 0);
            tft.println("Snake Game Selected");

            playCantina();
            setupSnakeGame();

            vga.clear(vga.rgb(0x00, 0x00, 0x00));
            gfx->setTextColor(vga.rgb(255, 255, 255));
            gfx->setCursor(mode.hRes / 2 - 90, mode.vRes / 2 - 20);
            gfx->print("Press any button");
            gfx->setCursor(mode.hRes / 2 - 75, mode.vRes / 2 + 10);
            gfx->print("to continue");
            vga.show();

            tft.fillScreen(TFT_BLACK);
            tft.setCursor(0, 0);
            tft.println("Press any button");
            tft.println("to continue");

            // Wait for button release
            while (controller2.y == 0 || controller1.y == 0) {
                delay(1500);
            }

            // Wait for a new button press
            bool buttonPressed = false;
            while (!buttonPressed) {
                if (controller2.a == 0 || controller2.b == 0 || controller2.x == 0 || controller2.y == 0 || controller2.big == 0 ||
                    controller1.a == 0 || controller1.b == 0 || controller1.x == 0 || controller1.y == 0 || controller1.big == 0) {
                    buttonPressed = true;
                }
                delay(50);
            }

            notSelected = 1;
            break;
        } else if (controller2.b == 0 || controller1.b == 0) {
            vga.clear(vga.rgb(0x00, 0x00, 0x00));
            gfx->setCursor(10, 10);
            gfx->setTextColor(vga.rgb(255, 255, 255));
            gfx->print("Reaction Game Selected");
            vga.show();

            tft.fillScreen(TFT_BLACK);
            tft.setCursor(0, 0);
            tft.println("Reaction Game Selected");

            playCantina();
            runReactionGame();

            vga.clear(vga.rgb(0x00, 0x00, 0x00));
            gfx->setCursor(mode.hRes / 2 - 90, mode.vRes / 2 - 20);
            gfx->print("Press any button");
            gfx->setCursor(mode.hRes / 2 - 75, mode.vRes / 2 + 10);
            gfx->print("to continue");
            vga.show();

            tft.fillScreen(TFT_BLACK);
            tft.setCursor(0, 0);
            tft.println("Press any button");
            tft.println("to continue");

            // Wait for button release
            while (controller2.b == 0 || controller1.b == 0) {
                delay(1500);
            }

            // Wait for a new button press
            bool buttonPressed = false;
            while (!buttonPressed) {
                if (controller2.a == 0 || controller2.b == 0 || controller2.x == 0 || controller2.y == 0 || controller2.big == 0 ||
                    controller1.a == 0 || controller1.b == 0 || controller1.x == 0 || controller1.y == 0 || controller1.big == 0) {
                    playCantina();
                    buttonPressed = true;
                }
                delay(50);
            }

            notSelected = 1;
            break;
        } else if (controller2.a == 0 || controller1.a == 0) {
            vga.clear(vga.rgb(0x00, 0x00, 0x00));
            gfx->setCursor(10, 10);
            gfx->setTextColor(vga.rgb(255, 255, 255));
            gfx->print("Pong Game Selected");
            vga.show();

            tft.fillScreen(TFT_BLACK);
            tft.setCursor(0, 0);
            tft.println("Pong Game Selected");

            playCantina();
            setupPongGame();

            vga.clear(vga.rgb(0x00, 0x00, 0x00));
            gfx->setCursor(mode.hRes / 2 - 90, mode.vRes / 2 - 20);
            gfx->print("Press any button");
            gfx->setCursor(mode.hRes / 2 - 75, mode.vRes / 2 + 10);
            gfx->print("to continue");
            vga.show();

            tft.fillScreen(TFT_BLACK);
            tft.setCursor(0, 0);
            tft.println("Press any button");
            tft.println("to continue");

            // Wait for button release
            while (controller2.a == 0 || controller1.a == 0) {
                delay(1500);
            }

            // Wait for a new button press
            bool buttonPressed = false;
            while (!buttonPressed) {
                if (controller2.a == 0 || controller2.b == 0 || controller2.x == 0 || controller2.y == 0 || controller2.big == 0 ||
                    controller1.a == 0 || controller1.b == 0 || controller1.x == 0 || controller1.y == 0 || controller1.big == 0) {
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

