#include <esp_now.h>
#include <WiFi.h>
#include <TFT_eSPI.h>
#include <EEPROM.h>
#include "main.h"
#include "reactionGame.h"
#include "audioFile.h"
#include "ESP32S3VGA.h"
#include <GfxWrapper.h>

const int SwitchPin = 1;     // Button pin
const int EEPROM_ADDR = 0;   // EEPROM address to store high score

unsigned long startTime = 0;
bool gameStarted = false;
bool gameEnded = false;
unsigned long lastButtonPress = 0;  // For debouncing
const int debounceDelay = 200;  // 200 ms debounce time

// VGA setup
extern VGA vga;  // Make sure VGA is initialized in your main code
extern GfxWrapper<VGA>* gfx;  // Ensure the GfxWrapper for VGA is initialized
extern Mode mode;  // VGA mode (defined elsewhere)

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

            // Clear both VGA and TFT before drawing new content
            vga.clear(vga.rgb(0, 0, 0));  // Clear VGA screen
            gfx->setTextColor(vga.rgb(255, 255, 255));  // Set white text color for VGA
            gfx->setTextSize(2);
            gfx->setCursor(20, 100);  // Set text position
            gfx->println("Wait for GREEN");
            vga.show();  // Only show after everything is drawn

            tft.fillScreen(TFT_BLACK);  // Clear TFT screen
            tft.setTextColor(TFT_WHITE, TFT_BLACK);
            tft.setTextSize(2);
            tft.setCursor(20, 100);
            tft.println("Wait for GREEN");

            // Random delay between 2 and 5 seconds
            delay(random(2000, 5000));

            // Set the screen to green for both VGA and TFT
            vga.clear(vga.rgb(0, 255, 0));  // Green screen for VGA
            gfx->setTextColor(vga.rgb(0, 0, 0));  // Black text on green
            gfx->setTextSize(3);
            gfx->setCursor(60, 100);  // Set text position
            gfx->println("PRESS!");
            vga.show();  // Only show after everything is drawn

            tft.fillScreen(TFT_GREEN);  // Green screen for TFT
            tft.setTextColor(TFT_BLACK, TFT_GREEN);
            tft.setTextSize(3);
            tft.setCursor(60, 100);
            tft.println("PRESS!");

            startTime = millis();  // Reset start time
        } else {
            // End the game and calculate reaction time
            unsigned long reactionTime = millis() - startTime;

            // Clear both VGA and TFT before showing reaction time and winner
            vga.clear(vga.rgb(0, 0, 0));  // Clear VGA screen
            gfx->setTextColor(vga.rgb(255, 255, 255));  // Set white text color for VGA
            gfx->setTextSize(2);
            gfx->setCursor(20, 60);  // Set text position
            gfx->print("Reaction time: ");
            gfx->print(reactionTime);
            gfx->println(" ms");
            vga.show();  // Only show after everything is drawn

            tft.fillScreen(TFT_BLACK);  // Clear TFT screen
            tft.setTextColor(TFT_WHITE, TFT_BLACK);
            tft.setTextSize(2);
            tft.setCursor(20, 60);
            tft.print("Reaction time: ");
            tft.print(reactionTime);
            tft.println(" ms");

            // Clear the screen again before showing the winner
            vga.clear(vga.rgb(0, 0, 0));  // Clear VGA before showing winner
            tft.fillScreen(TFT_BLACK);  // Clear TFT before showing winner

            // Display which controller won
            gfx->setCursor(20, 100);  // Set text position for VGA
            tft.setCursor(20, 100);   // Set text position for TFT
            if (controller1Pressed) {
                gfx->println("Controller 1 won!");
                tft.println("Controller 1 won!");
                playCantina();
            } else if (controller2Pressed) {
                gfx->println("Controller 2 won!");
                tft.println("Controller 2 won!");
                playCantina();
            }
            vga.show();  // Show content after drawing
            delay(3000);  // Ensure enough time before resetting the screen

            // Check if new high score
            if (reactionTime < highScore || highScore == 0) {
                highScore = reactionTime;
                setHighScore(highScore);

                vga.clear(vga.rgb(0, 0, 0));  // Clear VGA before updating high score
                gfx->setCursor(20, 140);  // Set text position
                gfx->println("New High Score!");
                vga.show();  // Only show after everything is drawn

                tft.fillScreen(TFT_BLACK);  // Clear TFT before updating high score
                tft.setCursor(20, 140);
                tft.println("New High Score!");

                delay(1000);
            }

            // Display high score on both VGA and TFT
            vga.clear(vga.rgb(0, 0, 0));  // Clear VGA screen before showing high score
            gfx->setCursor(20, 180);  // Set text position
            gfx->print("High Score: ");
            gfx->print(highScore);
            gfx->println(" ms");
            vga.show();  // Only show after everything is drawn

            tft.fillScreen(TFT_BLACK);  // Clear TFT screen before showing high score
            tft.setCursor(20, 180);
            tft.print("High Score: ");
            tft.print(highScore);
            tft.println(" ms");

            delay(3000);  // Allow time for display to stabilize

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

    int highScore = 0;
    EEPROM.get(EEPROM_ADDR, highScore);  // Load the high score from EEPROM

    // Initial instructions on VGA and TFT
    vga.clear(vga.rgb(0, 0, 0));  // Black screen for VGA
    gfx->setTextColor(vga.rgb(255, 255, 255));  // White text
    gfx->setTextSize(2);
    gfx->setCursor(10, 100);
    gfx->println("Press to start");
    vga.show();

    tft.fillScreen(TFT_BLACK);  // Black screen for TFT
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
