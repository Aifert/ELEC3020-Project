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

void processButtonClick(int highScoreArray[]) {
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
                gameWinSound();
            } else if (controller2Pressed) {
                gfx->println("Controller 2 won!");
                gameWinSound();
            }
            vga.show();  // Show content after drawing
            delay(3000);  // Ensure enough time before resetting the screen

            // Check if new high score
            if (reactionTime < highScoreArray[0] || highScoreArray[0] == 0) {
                unsigned long newHighScoreArray[3] = {reactionTime, highScoreArray[0], highScoreArray[1]};

                EEPROM.put(REACTION_HIGH_SCORE_ADDRESS, newHighScoreArray);  // Store high score in EEPROM
                EEPROM.commit();  // Commit the EEPROM changes

                vga.clear(vga.rgb(0, 0, 0));  // Clear VGA before updating high score
                gfx->setCursor(20, 140);  // Set text position
                gfx->println("New High Score!");
                vga.show();  // Only show after everything is drawn

                delay(1000);
            }

            // Check if second fastest score
            else if (reactionTime < highScoreArray[1] || highScoreArray[1] == 0) {
                unsigned long newHighScoreArray[3] = {highScoreArray[0], reactionTime, highScoreArray[1]};

                EEPROM.put(REACTION_HIGH_SCORE_ADDRESS, newHighScoreArray);  // Store high score in EEPROM
                EEPROM.commit();  // Commit the EEPROM changes

                vga.clear(vga.rgb(0, 0, 0));  // Clear VGA before updating high score
                gfx->setCursor(20, 140);  // Set text position
                gfx->println("Second Fastest Time!");
                vga.show();  // Only show after everything is drawn

                delay(1000);
            }

            // Check if third fastest time
            else if (reactionTime < highScoreArray[2] || highScoreArray[2] == 0) {
                // Reaction time is the third fastest, shift scores below it down
                unsigned long newHighScoreArray[3] = {highScoreArray[0], highScoreArray[1], reactionTime};

                EEPROM.put(REACTION_HIGH_SCORE_ADDRESS, newHighScoreArray);  // Store high score in EEPROM
                EEPROM.commit();  // Commit the EEPROM changes

                vga.clear(vga.rgb(0, 0, 0));  // Clear VGA before updating high score
                gfx->setCursor(20, 140);  // Set text position
                gfx->println("Third Fastest Time!");
                vga.show();  // Only show after everything is drawn

                delay(1000);
            }

            // Display high score on both VGA and TFT
            // vga.clear(vga.rgb(0, 0, 0));  // Clear VGA screen before showing high score
            // gfx->setCursor(20, 180);  // Set text position
            // gfx->print("High Score: ");
            // gfx->print(highScoreArray[0]);
            // gfx->println(" ms");
            // vga.show();  // Only show after everything is drawn

            // delay(3000);  // Allow time for display to stabilize

            gameEnded = true;
        }
    }

    // Reset wireless button states
    controller1.big = 1;
    controller2.big = 1;
}

void runReactionGame() {
    EEPROM.get(REACTION_HIGH_SCORE_ADDRESS, reactionHighScores);  // Load the high score from EEPROM

    // Initial instructions on VGA and TFT
    vga.clear(vga.rgb(0x00, 0x00, 0x00));  // Clear screen (black background)


    gfx->setCursor(10, 10);
    gfx->setTextColor(vga.rgb(255, 255, 255));
    gfx->print("< Press Y to return to the main menu");


    gfx->setCursor(70, 100);
    gfx->setTextColor(vga.rgb(255, 255, 255));
    gfx->print("Press Big to start the game");

    vga.show();

    // Main game loop
    while(controller1.big == controller2.big || controller2.big == controller1.big){
        Serial.print(controller1.big);
        if (controller1.y == 0 || controller2.y == 0){
            return;
        }
    }
    while (!gameEnded) {
        processButtonClick(reactionHighScores);
        delay(10);  // Small delay to prevent excessive CPU usage
    }

    // Reset game state
    gameStarted = false;
    gameEnded = false;

    gfx->setTextSize(1);
}
