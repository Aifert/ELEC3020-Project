#include <esp_now.h>
#include <WiFi.h>
#include <TFT_eSPI.h>
#include <EEPROM.h>
#include "main.h"
#include "reactionGame.h"

const int SwitchPin = 1;     // Button pin
const int EEPROM_ADDR = 0;   // EEPROM address to store high score

unsigned long startTime = 0;
bool gameStarted = false;
bool gameEnded = false;
unsigned long lastButtonPress = 0;  // For debouncing

const int debounceDelay = 200;  // 200 ms debounce time

void setHighScore(int highScore) {
    EEPROM.put(EEPROM_ADDR, highScore);  // Store high score in EEPROM
    EEPROM.commit();  // Commit the EEPROM changes
}

void processButtonClick(int& highScore) {
    bool controller1Pressed = controller1.big == 0;
    bool controller2Pressed = controller2.big == 0;
    bool wirelessButtonState = controller1Pressed || controller2Pressed;

    bool buttonPressed = wirelessButtonState;

    // Debounce check
    if (buttonPressed && millis() - lastButtonPress > debounceDelay) {
        lastButtonPress = millis();

        if (!gameStarted) {
            // Start the game
            gameStarted = true;
            startTime = millis();
            tft.fillScreen(TFT_BLACK);
            tft.setTextColor(TFT_WHITE, TFT_BLACK);
            tft.setTextSize(2);
            tft.setCursor(20, 100);
            tft.println("Wait for GREEN");

            // Random delay between 2 and 5 seconds
            delay(random(2000, 5000));

            tft.fillScreen(TFT_GREEN);
            tft.setTextColor(TFT_BLACK, TFT_GREEN);
            tft.setTextSize(3);
            tft.setCursor(60, 100);
            tft.println("PRESS!");

            startTime = millis();  // Reset start time
        } else {
            // End the game and calculate reaction time
            unsigned long reactionTime = millis() - startTime;

            tft.fillScreen(TFT_BLACK);
            tft.setTextColor(TFT_WHITE, TFT_BLACK);
            tft.setTextSize(2);
            tft.setCursor(20, 60);
            tft.print("Reaction time: ");
            tft.print(reactionTime);
            tft.println(" ms");

            // Display which controller won
            tft.setCursor(20, 100);
            if (controller1Pressed) {
                tft.println("Controller 1 won!");
            } else if (controller2Pressed) {
                tft.println("Controller 2 won!");
            }

            if (reactionTime < highScore || highScore == 0) {
                highScore = reactionTime;
                setHighScore(highScore);
                tft.setCursor(20, 140);
                tft.println("New High Score!");

                delay(1000);
            }

            tft.setCursor(20, 180);
            tft.print("High Score: ");
            tft.print(highScore);
            tft.println(" ms");

            delay(1000);

            gameEnded = true;
        }
    }

    // Reset wireless button states
    controller1.big = 1;
    controller2.big = 1;
}

void runReactionGame() {
    // Set up ESP-NOW
    WiFi.mode(WIFI_STA);
    if (esp_now_init() != ESP_OK) {
        Serial.println("Error initializing ESP-NOW");
        return;
    }

    // Register callback function
    // esp_now_register_recv_cb(OnDataRecv);

    int highScore = 0;
    EEPROM.get(EEPROM_ADDR, highScore);  // Load the high score from EEPROM

    tft.fillScreen(TFT_BLACK);
    tft.setTextColor(TFT_WHITE, TFT_BLACK);
    tft.setTextSize(2);
    tft.setCursor(10, 100);
    tft.println("Press to start");

    // Main game loop
    while (!gameEnded) {
        processButtonClick(highScore);
        delay(10);  // Small delay to prevent excessive CPU usage
    }

    // Reset game state
    gameStarted = false;
    gameEnded = false;
}
