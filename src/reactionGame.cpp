#include <TFT_eSPI.h>
#include <EEPROM.h>  // Include EEPROM library
#include <reactionGame.h>

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
    bool switchState = !digitalRead(SwitchPin);  // Active LOW button state

    Serial.println("switchState");
    Serial.println(switchState);

    // Debounce check
    if (switchState && millis() - lastButtonPress > debounceDelay) {
        lastButtonPress = millis();

        if (!gameStarted) {
            // Start the game
            gameStarted = true;
            gameEnded = false;
            startTime = millis();
            tft.fillScreen(TFT_GREEN);
            tft.setTextColor(TFT_BLACK);
            tft.setTextSize(2);
            tft.setCursor(10, 110);
            tft.println("Click when red!");

            // Generate a random delay between 1 to 5 seconds
            unsigned long delayTime = random(1000, 5001);  // Random delay between 1000ms to 5000ms
            delay(delayTime);

            // Turn the screen red
            tft.fillScreen(TFT_RED);
            startTime = millis();  // Reset start time to reaction time point
        } else {
            // End the game and calculate reaction time
            unsigned long reactionTime = millis() - startTime;

            tft.fillScreen(TFT_BLACK);
            tft.setTextColor(TFT_WHITE);
            tft.setTextSize(2);
            tft.setCursor(10, 50);
            tft.print("Reaction time: ");
            tft.print(reactionTime);
            tft.println(" ms");

            // Update high score if applicable
            if (reactionTime < highScore || highScore == 0) {
                highScore = reactionTime;
                setHighScore(highScore);
                tft.setCursor(10, 90);
                tft.println("New high score!");
            } else {
                tft.setCursor(10, 90);
                tft.println("Your score is:");
                tft.print(reactionTime);
                tft.println(" ms");
            }

            gameEnded = true;
            delay(3000);  // Display result for 3 seconds
        }
    }

    Serial.println("nothing pressed I am not doing anything");
}

void runReactionGame() {
    int highScore = 0;
    EEPROM.get(EEPROM_ADDR, highScore);  // Load the high score from EEPROM

    Serial.println("Loading high score:");
    Serial.println(highScore);

    // Initial display
    tft.fillScreen(TFT_BLACK);
    tft.setTextColor(TFT_WHITE);
    tft.setTextSize(2);
    tft.setCursor(10, 50);
    tft.println("Reaction Game");
    tft.setCursor(10, 90);
    tft.print("High Score: ");
    if (highScore == 0) {
        tft.println("None");
    } else {
        tft.print(highScore);
        tft.println(" ms");
    }
    tft.setCursor(10, 130);
    tft.println("Click to start!");

    // Wait for button press to start the game
    while (digitalRead(SwitchPin) == HIGH) {
        delay(10);  // Small delay to prevent CPU hogging
    }

    delay(1000);  // Short delay before game starts

    // Start the game
    while (!gameEnded) {
        processButtonClick(highScore);
    }

    // Reset game state
    gameStarted = false;
    gameEnded = false;
}
