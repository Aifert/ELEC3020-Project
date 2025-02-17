// main.cpp
#include "snakeGame/Snake.h"
#include "reactionGame.h"
#include "main.h"
#include "audioFile.h"
#include "PongGame.h"
#include "leaderboard.h"
#include <Arduino.h>

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

snakeHighScores highScores[3];
int reactionHighScores[3];

// Replace the Star struct with a Toast struct
const int TOAST_COUNT = 15;
struct Toast {
    int x, y;
    int speed;
    int size;
    uint16_t color;
};
Toast toasts[TOAST_COUNT];

// Define an array of colors
const uint16_t COLORS[] = {
    static_cast<uint16_t>(vga.rgb(255, 0, 0)),    // Red
    static_cast<uint16_t>(vga.rgb(0, 255, 0)),    // Green
    static_cast<uint16_t>(vga.rgb(0, 0, 255)),    // Blue
    static_cast<uint16_t>(vga.rgb(255, 255, 0)),  // Yellow
    static_cast<uint16_t>(vga.rgb(255, 0, 255)),  // Magenta
    static_cast<uint16_t>(vga.rgb(0, 255, 255)),  // Cyan
    static_cast<uint16_t>(vga.rgb(255, 128, 0)),  // Orange
    static_cast<uint16_t>(vga.rgb(128, 0, 255))   // Purple
};

const int COLOR_COUNT = sizeof(COLORS) / sizeof(COLORS[0]);

// Modify this function to initialize the toasts with predefined colors
void initToasts() {
    for (int i = 0; i < TOAST_COUNT; i++) {
        toasts[i].x = random(mode.hRes);
        toasts[i].y = random(mode.vRes);
        toasts[i].speed = random(1, 3);
        toasts[i].size = random(10, 20);
        toasts[i].color = COLORS[random(COLOR_COUNT)];
    }
}

// Add this function to draw a simple toast shape
void drawToast(int x, int y, int size, uint16_t color) {
    int half_size = size / 2;
    gfx->fillRect(x - half_size, y - half_size, size, size, color);
    gfx->fillCircle(x - half_size, y - half_size, half_size / 2, color);
    gfx->fillCircle(x + half_size, y - half_size, half_size / 2, color);
}

// Function to print MAC address
void printMacAddress() {
    uint8_t mac[6];
    WiFi.macAddress(mac);
    char macStr[18];
    snprintf(macStr, sizeof(macStr), "%02X:%02X:%02X:%02X:%02X:%02X",
             mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);

    Serial.print("MAC Address: ");
    Serial.println(macStr);

}

void waitForButtonRelease(){
    while (controller2.a == 0 || controller2.b == 0 || controller2.x == 0 || controller2.y == 0 || controller2.big == 0 ||
           controller1.a == 0 || controller1.b == 0 || controller1.x == 0 || controller1.y == 0 || controller1.big == 0) {
        delay(1500);
    }
}

void displayGameOptions() {
    gfx->setTextSize(1);

    // Draw animated toast background
    vga.clear(vga.rgb(0, 0, 0));  // Black background
    for (int i = 0; i < TOAST_COUNT; i++) {
        drawToast(toasts[i].x, toasts[i].y, toasts[i].size, toasts[i].color);
        toasts[i].y = (toasts[i].y + toasts[i].speed) % (mode.vRes + toasts[i].size);
        if (toasts[i].y < -toasts[i].size) {
            toasts[i].y = mode.vRes + toasts[i].size;
        }
    }

    String team = "Team 6";
    String option1 = "Press A for Pong Game";
    String option2 = "Press B for Reaction Game";
    String option3 = "Press Y for Snake Game";
    String option4 = "Press Big for Leaderboard";

    int screenWidth = mode.hRes;
    int teamY = 20;
    int option1Y = teamY + 30;
    int option2Y = option1Y + 30;
    int option3Y = option2Y + 30;
    int option4Y = option3Y + 30;

    gfx->setTextColor(vga.rgb(255, 255, 255));  // White text

    // Calculate text width manually
    int teamWidth = team.length() * 16; // Assuming 8 pixels per character
    int option1Width = option1.length() * 8; // Assuming 8 pixels per character
    int option2Width = option2.length() * 8;
    int option3Width = option3.length() * 8;
    int option4Width = option4.length() * 8;

    gfx->setCursor((screenWidth - teamWidth) / 2, teamY);
    gfx->print(team);

    gfx->setCursor((screenWidth - option1Width) / 2, option1Y);
    gfx->print(option1);

    gfx->setCursor((screenWidth - option2Width) / 2, option2Y);
    gfx->print(option2);

    gfx->setCursor((screenWidth - option3Width) / 2, option3Y);
    gfx->print(option3);

    gfx->setCursor((screenWidth - option4Width) / 2, option4Y);
    gfx->print(option4);

    vga.show();
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
        EEPROM.get(REACTION_HIGH_SCORE_ADDRESS, highScore);
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
    tft.fillScreen(TFT_BLACK);
    tft.setTextColor(TFT_WHITE, TFT_BLACK);
    tft.setTextSize(4);
    tft.setCursor(80, 50);
    tft.println("Team 6");

    // Print MAC address
    printMacAddress();

    // Initialize toasts instead of stars
    initToasts();

    // Display initial game options
    displayGameOptions();

    // // Reset high scores in EEPROM
    // memset(reactionHighScores, 0, sizeof(reactionHighScores));
    // EEPROM.put(REACTION_HIGH_SCORE_ADDRESS, reactionHighScores);

    // memset(highScores, 0, sizeof(highScores));
    // EEPROM.put(SNAKE_HIGH_SCORE_ADDRESS, highScores);

    // int pongHighScores[3] = {0};
    // EEPROM.put(PONG_HIGH_SCORE_ADDRESS, pongHighScores);

    // EEPROM.commit();
    // Serial.println("All high scores reset in EEPROM");
}

void loop() {
    if (controller2.y == 1 || controller1.y == 1) {
        displayGameOptions();
    }

    while (!notSelected) {
        // Update and redraw the background
        displayGameOptions();

        if (controller2.y == 0 || controller1.y == 0) {
            vga.clear(vga.rgb(0x00, 0x00, 0x00));
            gfx->setCursor(10, 10);
            gfx->setTextColor(vga.rgb(255, 255, 255));
            gfx->print("Snake Game Selected");
            vga.show();

            playCantina();
            setupSnakeGame();

            vga.clear(vga.rgb(0x00, 0x00, 0x00));
            gfx->setTextColor(vga.rgb(255, 255, 255));
            gfx->setCursor(mode.hRes / 2 - 90, mode.vRes / 2 - 20);
            gfx->print("Press any button");
            gfx->setCursor(mode.hRes / 2 - 75, mode.vRes / 2 + 10);
            gfx->print("to continue");
            vga.show();

            // Wait for button release
            waitForButtonRelease();

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

            playCantina();
            runReactionGame();

            vga.clear(vga.rgb(0x00, 0x00, 0x00));
            gfx->setCursor(mode.hRes / 2 - 90, mode.vRes / 2 - 20);
            gfx->print("Press any button");
            gfx->setCursor(mode.hRes / 2 - 75, mode.vRes / 2 + 10);
            gfx->print("to continue");
            vga.show();

            // Wait for button release
            waitForButtonRelease();

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

            playCantina();
            setupPongGame();

            vga.clear(vga.rgb(0x00, 0x00, 0x00));
            gfx->setCursor(mode.hRes / 2 - 90, mode.vRes / 2 - 20);
            gfx->print("Press any button");
            gfx->setCursor(mode.hRes / 2 - 75, mode.vRes / 2 + 10);
            gfx->print("to continue");
            vga.show();

            // Wait for button release
            waitForButtonRelease();

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
        }
        else if (controller2.big == 0 || controller1.big == 0) {
            delay(50);
            if (controller2.big == 0 || controller1.big == 0) {
                vga.clear(vga.rgb(0x00, 0x00, 0x00));
                gfx->setCursor(10, 10);
                gfx->setTextColor(vga.rgb(255, 255, 255));
                gfx->print("Game Leaderboard Selected");
                vga.show();

                showLeaderboard();

                waitForButtonRelease();

                vga.clear(vga.rgb(0x00, 0x00, 0x00));
                gfx->setCursor(mode.hRes / 2 - 90, mode.vRes / 2 - 20);
                gfx->print("Press any button");
                gfx->setCursor(mode.hRes / 2 - 75, mode.vRes / 2 + 10);
                gfx->print("to continue");
                vga.show();

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
            }
        }

        delay(50); // Adjust this delay to control animation speed
    }

    // Return to main menu
    if (notSelected) {
        displayGameOptions();
    }
}
