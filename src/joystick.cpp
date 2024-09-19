// #include <ESP32Servo.h>
// #include "joystick.h"

// const int SwitchPin = 1;
// const int xPin = 2;
// const int yPin = 3;
// const int servoPinX = 43;  // The pin for the SG90 servo

// Servo servoX;  // Create a servo object for X movement (controlling SG90)

// const int deadZone = 10;  // Set the dead zone threshold
// const int noiseThreshold = 5;  // Minimum difference to register movement
// const int numReadings = 10;  // Number of samples to average

// int readingsX[numReadings];   // Array to store readings for X
// int readIndex = 0;            // Index of the current reading
// int totalX = 0;               // Total of X readings
// int averageX = 0;             // The smoothed X value

// int neutralX = 512;           // Neutral X (centered position)

// void setupJoystick() {
//     // Initialize all the readings to 0
//     for (int i = 0; i < numReadings; i++) {
//         readingsX[i] = 0;
//     }
// }

// void detectPosition() {
//     static int previousX = 0; // Start relative to neutral at 0

//     int x_position = analogRead(xPin);

//     // Apply smoothing using a moving average for X
//     totalX = totalX - readingsX[readIndex];  // Subtract the last reading from total
//     readingsX[readIndex] = x_position;       // Read the new X value
//     totalX = totalX + readingsX[readIndex];  // Add the new reading to the total
//     averageX = totalX / numReadings;         // Calculate the average X

//     // Advance to the next position in the array
//     readIndex++;
//     if (readIndex >= numReadings) {
//         readIndex = 0;  // Wrap around if we've reached the end of the array
//     }

//     // Apply dead zone logic for X
//     if (abs(averageX - neutralX) < deadZone) {
//         averageX = neutralX;  // Neutral X position
//     }

//     // Calculate the relative position from the neutral point
//     int relativeX = averageX - neutralX;

//     tft.fillScreen(TFT_BLACK);
//     tft.setCursor(0,0);
//     tft.print("X : \n");
//     tft.print(relativeX);

//     // Only update the servo if the change exceeds the noise threshold
//     if (abs(relativeX - previousX) > noiseThreshold) {
//         // Map the relative joystick values to servo positions (0-180 degrees)
//         int servoXPos = map(relativeX + neutralX, 0, 1023, 0, 180);  // Map X to 180-degree SG90 range

//         // Move the servo to the mapped position
//         servoX.write(servoXPos);

//         // Update the previous position to the new relative value


//     }


//     delay(50);  // Short delay for smooth servo movement
// }

// void runJoystickControl() {
//     pinMode(SwitchPin, INPUT);
//     pinMode(xPin, INPUT);
//     pinMode(yPin, INPUT);  // Even though you're using only 1 servo, keep the Y pin for future use if needed

//     // Initialize the servo pin
//     servoX.attach(servoPinX);  // Attach the X servo (SG90) to the servoPinX

//     // Initialize smoothing arrays
//     setupJoystick();

//     tft.init();
//     tft.setRotation(1);

//     tft.fillScreen(TFT_BLACK);
//     tft.setTextColor(TFT_WHITE, TFT_BLACK);
//     tft.setTextSize(2);

//     while (true) {
//         detectPosition();
//     }
// }
