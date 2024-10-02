#include <TFT_eSPI.h>
#include <Arduino.h>
#include "main.h"  // Assuming this contains the controller1 and controller2 struct definitions


// Game display settings
const int screenWidth = 240;  // Adjust for LilyGo S3 screen width
const int screenHeight = 135; // Adjust for LilyGo S3 screen height

// Paddle settings
const int paddleWidth = 5;
const int paddleHeight = 30;
int paddle1X = 10;
int paddle1Y = (screenHeight - paddleHeight) / 2;
int paddle2X = screenWidth - 15;
int paddle2Y = (screenHeight - paddleHeight) / 2;

// Ball settings
int ballX = screenWidth / 2;
int ballY = screenHeight / 2;
const int ballSize = 5;
int ballSpeedX = 2;
int ballSpeedY = 2;

// Game state
bool PonggameEnded = false;
int score1 = 0;
int score2 = 0;

// Function declarations
void resetGame();
void updatePaddles();
void updateBall();
void checkCollision();
void drawGame();
void drawWinScreen();
void checkWinCondition();
void resetBall();

void updatePaddles() {
  // Player 1 controls (left paddle)
  if (controller1.x == 0) {
    paddle1Y -= 5; // Move Paddle 1 up
  } else if (controller1.b == 0) {
    paddle1Y += 5; // Move Paddle 1 down
  }

  // Player 2 controls (right paddle)
  if (controller2.x == 0) {
    paddle2Y -= 5; // Move Paddle 2 up
  } else if (controller2.b == 0) {
    paddle2Y += 5; // Move Paddle 2 down
  }

  // Keep paddles within screen bounds
  if (paddle1Y < 0) paddle1Y = 0;
  if (paddle1Y + paddleHeight > screenHeight) paddle1Y = screenHeight - paddleHeight;
  if (paddle2Y < 0) paddle2Y = 0;
  if (paddle2Y + paddleHeight > screenHeight) paddle2Y = screenHeight - paddleHeight;
}

void updateBall() {
  ballX += ballSpeedX;
  ballY += ballSpeedY;

  // Ball bounces off top and bottom
  if (ballY <= 0 || ballY >= screenHeight - ballSize) {
    ballSpeedY = -ballSpeedY;
  }

  // Ball goes out of bounds (left or right)
  if (ballX <= 0) {
    score2++;
    checkWinCondition();
    resetBall();
  }
  if (ballX >= screenWidth - ballSize) {
    score1++;
    checkWinCondition();
    resetBall();
  }
}

void checkCollision() {
  // Ball hits paddle 1
  if (ballX <= paddle1X + paddleWidth && ballY >= paddle1Y && ballY <= paddle1Y + paddleHeight) {
    ballSpeedX = -ballSpeedX;
  }

  // Ball hits paddle 2
  if (ballX + ballSize >= paddle2X && ballY >= paddle2Y && ballY <= paddle2Y + paddleHeight) {
    ballSpeedX = -ballSpeedX;
  }
}

void drawGame() {
  tft.fillScreen(TFT_BLACK);

  // Draw paddles
  tft.fillRect(paddle1X, paddle1Y, paddleWidth, paddleHeight, TFT_WHITE);
  tft.fillRect(paddle2X, paddle2Y, paddleWidth, paddleHeight, TFT_WHITE);

  // Draw ball
  tft.fillRect(ballX, ballY, ballSize, ballSize, TFT_WHITE);

  // Draw scores
  tft.setTextColor(TFT_WHITE);
  tft.setTextSize(2);
  tft.setCursor(30, 10);
  tft.print(score1);
  tft.setCursor(screenWidth - 50, 10);
  tft.print(score2);
}

void drawWinScreen() {
  tft.fillScreen(TFT_BLACK);
  tft.setTextColor(TFT_WHITE);
  tft.setTextSize(2);
  tft.setCursor(40, screenHeight / 2 - 20);
  tft.print("Game Over!");
  tft.setCursor(20, screenHeight / 2 + 10);
  tft.print("Press any button");
  tft.setCursor(50, screenHeight / 2 + 30);
  tft.print("to restart");
}

void checkWinCondition() {
  if (score1 >= 5 || score2 >= 5) {
    PonggameEnded = true;
  }
}

void resetGame() {
  score1 = 0;
  score2 = 0;
  PonggameEnded = false;
  resetBall();
}

void resetBall() {
  ballX = screenWidth / 2;
  ballY = screenHeight / 2;
  ballSpeedX = (random(0, 2) == 0) ? 2 : -2;  // Randomize initial direction
  ballSpeedY = (random(0, 2) == 0) ? 2 : -2;
}

void setupPongGame() {
  tft.init();
  tft.setRotation(1);  // Adjust rotation if needed
  tft.fillScreen(TFT_BLACK);
  resetGame();

  while(true){
    if (!PonggameEnded) {
    updatePaddles();
    updateBall();
    checkCollision();
    drawGame();
    } else {
    drawWinScreen();
    resetGame();

    break;
  }
  delay(20); // Control game speed

  }
}
