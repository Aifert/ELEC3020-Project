// main.cpp
#include <TFT_eSPI.h>
#include <EEPROM.h>
#include "snakeGame/Snake.h"
#include "reactionGame.h"
#include "main.h"

#define EEPROM_SIZE 4096
#define HIGH_SCORE_ADDRESS 0

// Define Buttons and Switch Pins
const int buttonA = 0;
const int buttonB = 14;
const int switchPin = 1;
int notSelected = 0;
int highScore = 0;

TFT_eSPI tft = TFT_eSPI();

void setup() {
    Serial.begin(115200);
    pinMode(buttonA, INPUT_PULLUP);
    pinMode(buttonB, INPUT_PULLUP);
    pinMode(switchPin, INPUT_PULLUP);

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

    // Display Game Options
    String option1 = "Press 0 for Snake Game";
    String option2 = "Press 14 for Reaction Game";

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
}

void loop() {
    while(!notSelected) {
        int buttonAState = digitalRead(buttonA);
        int buttonBState = digitalRead(buttonB);

        if (buttonAState == LOW) {
            tft.fillScreen(TFT_BLACK);
            tft.setCursor(10, 10);
            tft.print("Snake Game Selected");

            setupSnakeGame();

            notSelected = 1;
            break;
        } else if (buttonBState == LOW) {
            tft.fillScreen(TFT_BLACK);
            tft.setCursor(10, 10);
            tft.print("Reaction Game Selected");

            // Run Reaction Game
            runReactionGame();

            notSelected = 1;
            break;
        }

        delay(100); // Debounce delay
    }
}
