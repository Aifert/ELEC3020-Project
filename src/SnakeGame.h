#ifndef SNAKE_GAME_H
#define SNAKE_GAME_H

#include <TFT_eSPI.h>

// Game display settings
extern const int screenWidth;
extern const int screenHeight;
extern const int midScreen;

// Snake settings
extern const int snakeSize;
extern const int maxSnakeLength;
extern int snake1X[];
extern int snake1Y[];
extern int snake2X[];
extern int snake2Y[];
extern int snake1DX;
extern int snake1DY;
extern int snake2DX;
extern int snake2DY;
extern int snake1Length;
extern int snake2Length;
extern int snake1Score;
extern int snake2Score;
extern int player1Wins;
extern int player2Wins;
extern bool snakeGameEnded;
extern bool player1Lost;
extern bool player2Lost;

// Fruit settings
extern int fruit1X;
extern int fruit1Y;
extern int fruit2X;
extern int fruit2Y;

// Function declarations
void resetGame();
void updateSnakes();
void checkCollision();
void drawGame();
void drawWinScreen();
void placeFruit(int &fruitX, int &fruitY);
void setupSnakeGame();

#endif // SNAKE_GAME_H