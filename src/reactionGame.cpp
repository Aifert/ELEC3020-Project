#include <esp_now.h>
#include <WiFi.h>
#include "main.h"
#include "reactionGame.h"
#include "audioFile.h"
#include "ESP32S3VGA.h"
#include <GfxWrapper.h>

const int SwitchPin = 1;     // Button pin

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
    EEPROM.put(REACTION_HIGH_SCORE_ADDRESS, highScore);  // Store high score in EEPROM
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

            // Random delay between 2 and 5 seconds
            delay(random(2000, 5000));

            // Set the screen to green for both VGA and TFT
            vga.clear(vga.rgb(0, 255, 0));  // Green screen for VGA
            gfx->setTextColor(vga.rgb(0, 0, 0));  // Black text on green
            gfx->setTextSize(3);
            gfx->setCursor(60, 100);  // Set text position
            gfx->println("PRESS!");
            vga.show();  // Only show after everything is drawn

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

            // Clear the screen again before showing the winner
            vga.clear(vga.rgb(0, 0, 0));  // Clear VGA before showing winner

            // Display which controller won
            gfx->setCursor(20, 100);  // Set text position for VGA
            if (controller1Pressed) {
                gfx->println("Controller 1 won!");
                playCantina();
            } else if (controller2Pressed) {
                gfx->println("Controller 2 won!");
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

                delay(1000);
            }

            // Display high score on both VGA and TFT
            vga.clear(vga.rgb(0, 0, 0));  // Clear VGA screen before showing high score
            gfx->setCursor(20, 180);  // Set text position
            gfx->print("High Score: ");
            gfx->print(highScore);
            gfx->println(" ms");
            vga.show();  // Only show after everything is drawn

            delay(3000);  // Allow time for display to stabilize

            gameEnded = true;
        }
    }

    // Reset wireless button states
    controller1.big = 1;
    controller2.big = 1;
}

void runReactionGame() {
    int highScore = 0;
    EEPROM.get(REACTION_HIGH_SCORE_ADDRESS, highScore);  // Load the high score from EEPROM

    // Initial instructions on VGA and TFT
    vga.clear(vga.rgb(0, 0, 0));  // Black screen for VGA
    gfx->setTextColor(vga.rgb(255, 255, 255));  // White text
    gfx->setTextSize(2);
    gfx->setCursor(10, 100);
    gfx->println("Press to start");
    vga.show();

    // Main game loop
    while (!gameEnded) {
        processButtonClick(highScore);
        delay(10);  // Small delay to prevent excessive CPU usage
    }

    // Reset game state
    gameStarted = false;
    gameEnded = false;
}
