#include "ESP32S3VGA.h"
#include <GfxWrapper.h>
#include <esp_now.h>
#include <WiFi.h>
#include "main.h"  // Assuming this contains the controller1 and controller2 struct definitions

// VGA pin configuration
const PinConfig pins(-1, -1, -1, -1, 43, -1, -1, -1, -1, -1, 44, -1, -1, -1, -1, 18, 1, 2);
extern VGA vga;
extern Mode mode;
extern GfxWrapper<VGA> gfx;

// Game display settings
const int screenWidth = 320;
const int screenHeight = 240;

// Paddle settings
const int paddleWidth = 10;
const int paddleHeight = 40;
int paddle1X = 10;
int paddle1Y = (screenHeight - paddleHeight) / 2;
int paddle2X = screenWidth - 20;
int paddle2Y = (screenHeight - paddleHeight) / 2;

// Ball settings
int ballX = screenWidth / 2;
int ballY = screenHeight / 2;
const int ballSize = 10;
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
    paddle1Y = constrain(paddle1Y, 0, screenHeight - paddleHeight);
    paddle2Y = constrain(paddle2Y, 0, screenHeight - paddleHeight);
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
    vga.clear(vga.rgb(0x00, 0x00, 0x00));  // Clear screen (black background)

    // Draw paddles
    gfx.fillRect(paddle1X, paddle1Y, paddleWidth, paddleHeight, vga.rgb(255, 255, 255));  // White paddle 1
    gfx.fillRect(paddle2X, paddle2Y, paddleWidth, paddleHeight, vga.rgb(255, 255, 255));  // White paddle 2

    // Draw ball
    gfx.fillRect(ballX, ballY, ballSize, ballSize, vga.rgb(255, 255, 255));  // White ball

    // Draw scores
    gfx.setCursor(30, 20);
    gfx.setTextColor(vga.rgb(255, 255, 255));
    gfx.print("P1: " + String(score1));  // Display player 1 score

    gfx.setCursor(screenWidth - 100, 20);
    gfx.setTextColor(vga.rgb(255, 255, 255));
    gfx.print("P2: " + String(score2));  // Display player 2 score

    vga.show();  // Display everything on screen
}

void drawWinScreen() {
    vga.clear(vga.rgb(0x00, 0x00, 0x00));  // Clear screen

    gfx.setCursor(80, screenHeight / 2 - 10);
    gfx.setTextColor(vga.rgb(255, 255, 255));
    gfx.print("Game Over!");

    gfx.setCursor(50, screenHeight / 2 + 20);
    gfx.setTextColor(vga.rgb(255, 255, 255));
    gfx.print("Press Button to Restart");

    vga.show();
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
    vga.bufferCount = 2;
    if (!vga.init(pins, mode, 16)) {
        while (1) delay(1); // Halt if VGA initialization fails
    }
    vga.start();

    resetGame();

    while(true) {
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
