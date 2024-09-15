#include "joystick.h"

const int SwitchPin = 1;
const int xPin = 2;
const int yPin = 3;

const int deadZone = 10;  // Set the dead zone threshold
const int noiseThreshold = 5;  // Minimum difference to register movement
const int numReadings = 10;  // Number of samples to average

int readingsX[numReadings];   // Array to store readings for X
int readingsY[numReadings];   // Array to store readings for Y
int readIndex = 0;            // Index of the current reading
int totalX = 0;               // Total of X readings
int totalY = 0;               // Total of Y readings
int averageX = 0;             // The smoothed X value
int averageY = 0;             // The smoothed Y value

int neutralX = 512;           // Neutral X (centered position)
int neutralY = 512;           // Neutral Y (centered position)

void setupJoystick() {
    // Initialize all the readings to 0
    for (int i = 0; i < numReadings; i++) {
        readingsX[i] = 0;
        readingsY[i] = 0;
    }
}

void detectPosition() {
    static int previousX = 0; // Start relative to neutral at 0
    static int previousY = 0;
    static int clicked;

    int x_position = analogRead(xPin);
    int y_position = analogRead(yPin);

    // Apply smoothing using a moving average for X
    totalX = totalX - readingsX[readIndex];  // Subtract the last reading from total
    readingsX[readIndex] = x_position;       // Read the new X value
    totalX = totalX + readingsX[readIndex];  // Add the new reading to the total
    averageX = totalX / numReadings;         // Calculate the average X

    // Apply smoothing using a moving average for Y
    totalY = totalY - readingsY[readIndex];  // Subtract the last reading from total
    readingsY[readIndex] = y_position;       // Read the new Y value
    totalY = totalY + readingsY[readIndex];  // Add the new reading to the total
    averageY = totalY / numReadings;         // Calculate the average Y

    // Advance to the next position in the array
    readIndex++;
    if (readIndex >= numReadings) {
        readIndex = 0;  // Wrap around if we've reached the end of the array
    }

    // Apply dead zone logic for X
    if (abs(averageX - neutralX) < deadZone) {
        averageX = neutralX;  // Neutral X position
    }

    // Apply dead zone logic for Y
    if (abs(averageY - neutralY) < deadZone) {
        averageY = neutralY;  // Neutral Y position
    }

    // Calculate the relative position from the neutral point
    int relativeX = averageX - neutralX;
    int relativeY = averageY - neutralY;

    // Only update the screen if the change exceeds the noise threshold
    if (abs(relativeX - previousX) > noiseThreshold || abs(relativeY - previousY) > noiseThreshold) {
        tft.fillScreen(TFT_BLACK);
        tft.setCursor(0, 0);

        tft.print("Relative X: ");
        tft.println(relativeX);  // Display relative X

        tft.print("Relative Y: ");
        tft.println(relativeY);  // Display relative Y

        // Update the previous positions to the new relative values
        previousX = relativeX;
        previousY = relativeY;
    }

    // Check if the switch is clicked
    if (digitalRead(SwitchPin) != clicked) {
        tft.print("Clicked");
        clicked = digitalRead(SwitchPin);
    }
}

void runJoystickControl() {
    pinMode(SwitchPin, INPUT);
    pinMode(xPin, INPUT);
    pinMode(yPin, INPUT);

    // Initialize smoothing arrays
    setupJoystick();

    tft.init();
    tft.setRotation(1);

    tft.fillScreen(TFT_BLACK);
    tft.setTextColor(TFT_WHITE, TFT_BLACK);
    tft.setTextSize(2);

    while (true) {
        detectPosition();
        delay(100);  // Add a short delay to prevent overwhelming the display with too many updates
    }
}
