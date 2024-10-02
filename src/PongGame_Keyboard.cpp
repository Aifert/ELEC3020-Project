// #include "ESP32S3VGA.h"
// #include <GfxWrapper.h>
// #include <Arduino.h> // This will provide delay() and Serial functions

// // VGA pin configuration
// const PinConfig pins(-1,-1,-1,-1,43,  -1,-1,-1,-1,-1,44,  -1,-1,-1,-1,18,  1,2);
// VGA vga;
// Mode mode = Mode::MODE_320x240x60;  // Set VGA mode to 320x240
// GfxWrapper<VGA> gfx(vga, mode.hRes, mode.vRes);

// // Game display settings
// int screenWidth = 320;
// int screenHeight = 240;

// // Paddle settings
// int paddleWidth = 10;
// int paddleHeight = 40;
// int paddle1X = 10;
// int paddle1Y = (screenHeight - paddleHeight) / 2;
// int paddle2X = screenWidth - 20;
// int paddle2Y = (screenHeight - paddleHeight) / 2;

// // Ball settings
// int ballX = screenWidth / 2;
// int ballY = screenHeight / 2;
// int ballSize = 10;
// int ballSpeedX = 2;
// int ballSpeedY = 2;

// // Game state
// bool gameEnded = false;
// int score1 = 0;
// int score2 = 0;

// // Function declarations
// void resetGame();
// void updatePaddles();
// void updateBall();
// void checkCollision();
// void drawGame();
// void drawWinScreen();
// void checkWinCondition();
// void resetBall();

// void setup() {
//   vga.bufferCount = 2;
//   if (!vga.init(pins, mode, 16)) {
//     while (1) delay(1); // Halt if VGA initialization fails
//   }
//   vga.start();
//   Serial.begin(9600);  // Initialize serial communication
//   resetGame();
// }

// void loop() {
//   if (!gameEnded) {
//     updatePaddles();
//     updateBall();
//     checkCollision();
//     drawGame();
//   } else {
//     drawWinScreen();
//     if (Serial.available()) {
//       char cmd = Serial.read();
//       if (cmd == 'r') { // 'r' for restart
//         resetGame();
//       }
//     }
//   }
//   delay(20); // Control game speed
// }

// void updatePaddles() {
//   char cmd = 0;
//   if (Serial.available()) {
//     cmd = Serial.read();
//   }

//   // Player 1 controls
//   if (cmd == 'w') {
//     paddle1Y -= 5; // Move Paddle 1 up
//   } else if (cmd == 's') {
//     paddle1Y += 5; // Move Paddle 1 down
//   }

//   // Player 2 controls
//   if (cmd == 'p') {
//     paddle2Y -= 5; // Move Paddle 2 up
//   } else if (cmd == 'l') {
//     paddle2Y += 5; // Move Paddle 2 down
//   }

//   // Keep paddles within screen bounds
//   if (paddle1Y < 0) paddle1Y = 0;
//   if (paddle1Y + paddleHeight > screenHeight) paddle1Y = screenHeight - paddleHeight;

//   if (paddle2Y < 0) paddle2Y = 0;
//   if (paddle2Y + paddleHeight > screenHeight) paddle2Y = screenHeight - paddleHeight;
// }

// void updateBall() {
//   ballX += ballSpeedX;
//   ballY += ballSpeedY;

//   // Ball bounces off top and bottom
//   if (ballY <= 0 || ballY >= screenHeight - ballSize) {
//     ballSpeedY = -ballSpeedY;
//   }

//   // Ball goes out of bounds (left or right)
//   if (ballX <= 0) {
//     score2++;
//     checkWinCondition();
//     resetBall();
//   }
//   if (ballX >= screenWidth - ballSize) {
//     score1++;
//     checkWinCondition();
//     resetBall();
//   }
// }

// void checkCollision() {
//   // Ball hits paddle 1
//   if (ballX <= paddle1X + paddleWidth && ballY >= paddle1Y && ballY <= paddle1Y + paddleHeight) {
//     ballSpeedX = -ballSpeedX;
//   }

//   // Ball hits paddle 2
//   if (ballX + ballSize >= paddle2X && ballY >= paddle2Y && ballY <= paddle2Y + paddleHeight) {
//     ballSpeedX = -ballSpeedX;
//   }
// }

// void drawGame() {
//   vga.clear(vga.rgb(0x00, 0x00, 0x00));  // Clear screen (black background)

//   // Draw paddles
//   gfx.fillRect(paddle1X, paddle1Y, paddleWidth, paddleHeight, vga.rgb(255, 255, 255));  // White paddle 1
//   gfx.fillRect(paddle2X, paddle2Y, paddleWidth, paddleHeight, vga.rgb(255, 255, 255));  // White paddle 2

//   // Draw ball
//   gfx.fillRect(ballX, ballY, ballSize, ballSize, vga.rgb(255, 255, 255));  // White ball

//   // Draw scores
//   gfx.setCursor(30, 20);
//   gfx.setTextColor(vga.rgb(255, 255, 255));
//   gfx.print("P1: " + String(score1));  // Display player 1 score

//   gfx.setCursor(screenWidth - 100, 20);
//   gfx.setTextColor(vga.rgb(255, 255, 255));
//   gfx.print("P2: " + String(score2));  // Display player 2 score

//   vga.show();  // Display everything on screen
// }

// void drawWinScreen() {
//   vga.clear(vga.rgb(0x00, 0x00, 0x00));  // Clear screen

//   gfx.setCursor(80, screenHeight / 2 - 10);
//   gfx.setTextColor(vga.rgb(255, 255, 255));
//   gfx.print("Game Over!");

//   gfx.setCursor(50, screenHeight / 2 + 20);
//   gfx.setTextColor(vga.rgb(255, 255, 255));
//   gfx.print("Press 'r' to Restart");

//   vga.show();
// }

// void checkWinCondition() {
//   if (score1 >= 5 || score2 >= 5) {
//     gameEnded = true;
//   }
// }

// void resetGame() {
//   score1 = 0;
//   score2 = 0;
//   gameEnded = false;
//   resetBall();
// }

// void resetBall() {
//   ballX = screenWidth / 2;
//   ballY = screenHeight / 2;
//   ballSpeedX = (ballSpeedX > 0) ? 2 : -2;
//   ballSpeedY = (ballSpeedY > 0) ? 2 : -2;
// }
