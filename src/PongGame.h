#ifndef PONG_GAME_H
#define PONG_GAME_H

#include <TFT_eSPI.h>

// Game display settings
extern const int screenWidth;
extern const int screenHeight;

// Paddle settings
extern const int paddleWidth;
extern const int paddleHeight;
extern int paddle1X;
extern int paddle1Y;
extern int paddle2X;
extern int paddle2Y;

// Ball settings
extern int ballX;
extern int ballY;
extern const int ballSize;
extern int ballSpeedX;
extern int ballSpeedY;

// Game state
extern bool PonggameEnded;
extern int score1;
extern int score2;

// Function declarations
void resetGame();
void updatePaddles();
void updateBall();
void checkCollision();
void drawGame();
void drawWinScreen();
void checkWinCondition();
void resetBall();
void setupPongGame();

#endif // PONG_GAME_H
