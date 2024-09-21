// // joystick.cpp
// #include "shared.h"
// #include "joystick.h"

// // Define Joystick Pins
// const int xPin = 2; // Joystick X-axis
// const int yPin = 3; // Joystick Y-axis

// // Deadzone and Thresholds
// const int deadZone = 10;
// const int noiseThreshold = 5;
// const int numReadings = 10;

// // Variables for Smoothing
// int readingsX[numReadings];
// int readIndexX = 0;
// int totalX = 0;
// int averageX = 0;

// int readingsY[numReadings];
// int readIndexY = 0;
// int totalY = 0;
// int averageY = 0;

// int neutralX = 512;
// int neutralY = 512;

// // Function to Initialize Joystick Readings
// void setupJoystick() {
//     for (int i = 0; i < numReadings; i++) {
//         readingsX[i] = neutralX;
//         readingsY[i] = neutralY;
//     }
// }

// // **Function to Detect Joystick Direction (Only Defined Here)**
// Direction readJoystickDirection() {
//     // Read X-axis
//     int x_position = analogRead(xPin);
//     totalX = totalX - readingsX[readIndexX];
//     readingsX[readIndexX] = x_position;
//     totalX = totalX + readingsX[readIndexX];
//     averageX = totalX / numReadings;
//     readIndexX = (readIndexX + 1) % numReadings;

//     // Read Y-axis
//     int y_position = analogRead(yPin);
//     totalY = totalY - readingsY[readIndexY];
//     readingsY[readIndexY] = y_position;
//     totalY = totalY + readingsY[readIndexY];
//     averageY = totalY / numReadings;
//     readIndexY = (readIndexY + 1) % numReadings;

//     // Apply Deadzone
//     if (abs(averageX - neutralX) < deadZone) {
//         averageX = neutralX;
//     }
//     if (abs(averageY - neutralY) < deadZone) {
//         averageY = neutralY;
//     }

//     // Determine Direction
//     const int threshold = 600; // Adjust based on calibration

//     if (averageY < threshold && averageY > (1023 - threshold)) {
//         // Neutral Y
//     } else if (averageY < threshold) {
//         return UP;
//     } else if (averageY > (1023 - threshold)) {
//         return DOWN;
//     }

//     if (averageX < threshold && averageX > (1023 - threshold)) {
//         // Neutral X
//     } else if (averageX < threshold) {
//         return LEFT;
//     } else if (averageX > (1023 - threshold)) {
//         return RIGHT;
//     }

//     return NONE;
// }

// // **Function to Initialize Snake Game via Joystick**
// void initialiseSnakeGame() {
//     setupSnakeGame(); // Defined in snake.cpp

//     while (true) {
//         runSnakeGame(); // Run the game loop defined in snake.cpp
//     }
// }
