// #include <TFT_eSPI.h>

// // Initialize TFT
// TFT_eSPI tft = TFT_eSPI();

// // Define joystick pins
// const int xPin = 2;
// const int yPin = 3;  // Not used in direction detection but kept for potential future use

// // Joystick smoothing parameters
// const int numReadings = 10;
// const int deadZone = 200;  // Adjusted for 12-bit ADC; tweak as necessary

// int readingsX[numReadings];
// int readIndex = 0;
// long totalX = 0;  // Use long to prevent overflow
// int averageX = 0;

// // Define calibrated center for 12-bit ADC based on your observations
// const int centerX = 3016;

// // Direction thresholds
// const int leftThreshold = centerX - deadZone;
// const int rightThreshold = centerX + deadZone;

// void detectPosition() {
//   int x = analogRead(xPin);

//   // Apply smoothing using a moving average
//   totalX = totalX - readingsX[readIndex];
//   readingsX[readIndex] = x;
//   totalX += readingsX[readIndex];
//   readIndex = (readIndex + 1) % numReadings;

//   averageX = totalX / numReadings;

//   // Determine direction based on X-axis
//   String direction = "";

//   if (averageX < leftThreshold) {
//     direction = "LEFT";
//   }
//   else if (averageX > rightThreshold) {
//     direction = "RIGHT";
//   }
//   else {
//     direction = "NO_VALUE";
//   }

//   // Display on TFT
//   tft.fillScreen(TFT_BLACK);
//   tft.setCursor(10, 10);
//   tft.setTextSize(3);
//   tft.setTextColor(TFT_WHITE, TFT_BLACK);
//   tft.println(direction);

//   // Debug output
//   Serial.print("X: ");
//   Serial.print(averageX);
//   Serial.print(" Direction: ");
//   Serial.println(direction);
// }

// void setup() {
//   Serial.begin(115200);

//   // Initialize the TFT display
//   tft.init();
//   tft.setRotation(1);
//   tft.fillScreen(TFT_BLACK);
//   tft.setTextColor(TFT_WHITE, TFT_BLACK);
//   tft.setTextSize(3);

//   // Initialize joystick readings to center
//   for (int i = 0; i < numReadings; i++) {
//     readingsX[i] = centerX;
//   }
//   totalX = centerX * numReadings;
// }

// void loop() {
//   detectPosition();
//   delay(50);  // Short delay for stability
// }
