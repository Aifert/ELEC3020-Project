// #include <TFT_eSPI.h>
// #include "../main.h"
// #include "back.h"
// #include "gameOver.h"
// #include "newGame.h"

// TFT_eSprite sprite = TFT_eSprite(&tft);

// // Define button pins
// #define leftButtonPin 0
// #define rightButtonPin 14

// const int EEPROM_SNAKE_ADDR = 1;

// // Joystick control pins
// const int joystickXPin = 2; // Adjust to your hardware setup (e.g., A0, A1)

// // Joystick smoothing parameters
// const int numReadings = 10;
// const int deadZone = 200;  // Adjust based on calibration for sensitivity

// int readingsX[numReadings];    // Array to store analog readings
// int readIndexJoystick = 0;      // Current index in the readings array
// long totalXJoystick = 0;        // Total of the readings for averaging
// int averageXJoystick = 0;       // Averaged X value

// // Define direction codes
// const int NO_VALUE = 0;
// const int LEFT_DIR = 1;
// const int RIGHT_DIR = 2;

// // Variable to track the last processed direction code
// int lastDirectionCode = NO_VALUE;

// // Define calibrated center for joystick's X-axis
// const int centerXJoystick = 3016; // Adjust based on your joystick's neutral position

// int size = 1;
// int y[120] = {0};
// int x[120] = {0};

// unsigned long currentTime = 0;
// int period = 200;
// int deb, deb2 = 0;
// int dirX = 1;
// int dirY = 0;
// bool taken = 0;
// unsigned short colors[2] = {0x48ED, 0x590F}; // Terrain colors
// unsigned short snakeColor[2] = {0x9FD3, 0x38C9};
// bool chosen = 0;
// bool gOver = 0;
// int moves = 0;
// int playTime = 0;
// int foodX = 0;
// int foodY = 0;
// int howHard = 0;
// String diff[3] = {"EASY","NORMAL","HARD"};
// bool ready = 1;
// long readyTime = 0;

// void checkGameOver() // Check if the game is over
// {
//     if(x[0] < 0 || x[0] >= 17 || y[0] < 0 || y[0] >= 17)
//         gOver = true;
//     for(int i = 1; i < size; i++)
//         if(x[i] == x[0] && y[i] == y[0])
//             gOver = true;
// }

// void getFood() // Get new position of food
// {
//     foodX = random(0,17);
//     foodY = random(0,17);
//     taken = 0;
//     for(int i = 0; i < size; i++)
//         if(foodX == x[i] && foodY == y[i])
//             taken = 1;
//     if(taken == 1)
//         getFood();
// }

// void run() // Run function to update snake's position
// {
//     for(int i = size; i > 0; i--)
//     {
//         x[i] = x[i-1];
//         y[i] = y[i-1];
//     }

//     x[0] = x[0] + dirX;
//     y[0] = y[0] + dirY;

//     if(x[0] == foodX && y[0] == foodY)
//     {
//         size++;
//         getFood();
//         tft.drawString(String(size),44,250);
//         period = period - 1;
//         tft.drawString(String(500 - period),124,250);
//     }

//     sprite.fillSprite(TFT_BLACK);
//     /*
//     for(int i=0;i<17;i++)
//       for(int j=0;j<17;j++)
//         {
//         sprite.fillRect(j*10,i*10,10,10,colors[chosen]);
//         chosen=!chosen;
//         }
//      chosen=0;*/
//     checkGameOver();
//     if(gOver == 0){
//         sprite.drawRect(0,0,170,170,0x02F3);
//         for(int i = 0; i < size; i++){
//             sprite.fillRoundRect(x[i]*10, y[i]*10, 10, 10, 2, snakeColor[0]);
//             sprite.fillRoundRect(2 + x[i]*10, 2 + y[i]*10, 6, 6, 2, snakeColor[1]);
//         }
//         sprite.fillRoundRect(foodX*10 + 1, foodY*10 + 1, 8, 8, 1, TFT_RED);
//         sprite.fillRoundRect(foodX*10 + 3, foodY*10 + 3, 4, 4, 1, 0xFE18);

//     }
//     else
//     {
//         sprite.pushImage(0,0,170,170,gameOver);
//     }
//     sprite.pushSprite(0,30);
// }

// int change = 0;
// void runSnakeGame() { //...............................................................loop

//     // Handle snake movement based on the period
//     if(millis() > currentTime + period) {
//         run();
//         currentTime = millis();
//     }

//     // Reset readiness after a short delay to prevent rapid direction changes
//     if (millis() > readyTime + 100 && ready == 0) {
//         ready = 1;
//         change = 0; // Reset change to allow new direction changes
//     }

//     if(ready == 1){
//         // Read joystick's X-axis analog value
//         int joystickValue = analogRead(joystickXPin); // Renamed variable to avoid shadowing 'x'

//         // Apply smoothing using a moving average
//         totalXJoystick = totalXJoystick - readingsX[readIndexJoystick];
//         readingsX[readIndexJoystick] = joystickValue;
//         totalXJoystick += readingsX[readIndexJoystick];
//         readIndexJoystick = (readIndexJoystick + 1) % numReadings;

//         averageXJoystick = totalXJoystick / numReadings;

//         // Determine direction based on X-axis
//         int directionCode = NO_VALUE;

//         if (averageXJoystick > 4000) { // Threshold for "RIGHT" detection
//             directionCode = RIGHT_DIR;
//         }
//         else if (averageXJoystick < (centerXJoystick - deadZone)) { // Threshold for "LEFT" detection
//             directionCode = LEFT_DIR;
//         }
//         else {
//             directionCode = NO_VALUE;
//         }

//         // Debugging: Print joystick values (optional)
//         Serial.print("AverageXJoystick: ");
//         Serial.print(averageXJoystick);
//         Serial.print(" Direction Code: ");
//         Serial.println(directionCode);

//         // Implement the new direction change logic
//         if (directionCode != lastDirectionCode) {
//             Serial.print("Previous direction code");
//             Serial.println(lastDirectionCode);

//             Serial.print("New direction Code");
//             Serial.println(directionCode);
//             // Process direction change based on directionCode
//             if (directionCode == LEFT_DIR) {
//                 if(dirX == 1) {
//                     dirY = -1;
//                     dirX = 0;
//                 }
//                 else if(dirX == -1) {
//                     dirY = 1;
//                     dirX = 0;
//                 }
//                 else if(dirY == 1) {
//                     dirX = 1;
//                     dirY = 0;
//                 }
//                 else if(dirY == -1) {
//                     dirX = -1;
//                     dirY = 0;
//                 }
//             }
//             else if (directionCode == RIGHT_DIR) {
//                 if(dirX == 1) {
//                     dirY = 1;
//                     dirX = 0;
//                 }
//                 else if(dirX == -1) {
//                     dirY = -1;
//                     dirX = 0;
//                 }
//                 else if(dirY == 1) {
//                     dirX = -1;
//                     dirY = 0;
//                 }
//                 else if(dirY == -1) {
//                     dirX = 1;
//                     dirY = 0;
//                 }
//             }

//             // Update lastDirectionCode to prevent multiple changes from the same input
//             lastDirectionCode = directionCode;
//             Serial.print("updated");
//             Serial.print("Previous direction code");
//             Serial.println(lastDirectionCode);

//             Serial.print("New direction Code");
//             Serial.println(directionCode);


//             // Prevent immediate consecutive changes
//             ready = 0;
//             readyTime = millis();
//         }
//     }

//     /*
//     // Existing button handling code - Removed as joystick is now used for direction
//     if(ready==1){
//         if(digitalRead(leftButtonPin) == 0){
//             // Handle left button
//             // ... (existing code)
//         }
//         else{ deb=0; }
//     }

//     if(ready==1){
//         if(digitalRead(rightButtonPin) == 0){
//             // Handle right button
//             // ... (existing code)
//         }
//         else {deb2=0; }
//     }
//     */
// }

// void setupSnakeGame() {  //.......................setup
//     // Initialize serial communication for debugging (optional)
//     Serial.begin(115200);

//     // Initialize button pins
//     pinMode(leftButtonPin, INPUT_PULLUP);
//     pinMode(rightButtonPin, INPUT_PULLUP);

//     // Initialize TFT display
//     tft.init();
//     tft.fillScreen(TFT_BLACK);
//     tft.setRotation(0);
//     tft.setSwapBytes(true);
//     tft.pushImage(0,0,170,320,back);
//     tft.pushImage(0,30,170,170,newGame);

//     // Display difficulty indicator
//     tft.setTextColor(TFT_PURPLE, 0x7DFD);
//     tft.fillSmoothCircle(28, 102 + (howHard * 24), 5, TFT_RED, TFT_BLACK);
//     tft.drawString("DIFFICULTY:   " + diff[howHard] + "   ", 26, 267);

//     // Create sprite for drawing
//     sprite.createSprite(170,170);
//     sprite.setSwapBytes(true);

//     // Initialize joystick readings to center
//     for(int i = 0; i < numReadings; i++) {
//         readingsX[i] = centerXJoystick;
//     }
//     totalXJoystick = centerXJoystick * numReadings;

//     // Difficulty selection loop using left and right buttons
//     while(digitalRead(rightButtonPin) == 1)
//     {
//         if(digitalRead(leftButtonPin) == 0)
//         {
//             if(deb2 == 0)
//             {
//                 deb2 = 1;
//                 tft.fillCircle(28, 102 + (howHard * 24), 6, TFT_BLACK);
//                 howHard++;
//                 if(howHard == 3) howHard = 0;
//                 tft.fillSmoothCircle(28, 102 + (howHard * 24), 5, TFT_RED, TFT_BLACK);
//                 tft.drawString("DIFFICULTY:   " + diff[howHard] + "   ", 26, 267);
//                 period = 200 - howHard * 20;
//                 delay(200);
//             }
//         }
//         else
//         {
//             deb2 = 0;
//         }
//     }

//     // Initialize snake position and food
//     y[0] = random(5,15);
//     getFood();
//     tft.setTextSize(3);
//     tft.setTextDatum(4);
//     tft.drawString(String(size),44,250);
//     tft.drawString(String(500 - period),124,250);
//     delay(400);
//     dirX = 1;
//     dirY = 0;

//     while(true){
//         runSnakeGame();
//     }
// }
