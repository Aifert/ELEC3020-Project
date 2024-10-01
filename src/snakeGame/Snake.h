#ifndef SNAKE_H
#define SNAKE_H

#include <TFT_eSPI.h>

// Function declarations
void setupSnakeGame();
void runSnakeGame();
void checkGameOver();
void run();
void getFood();

// Constants
extern const int joystickXPin;
extern const int numReadings;
extern const int deadZone;
extern const int centerXJoystick;

// Variables
extern int size;
extern int y[120];
extern int x[120];
extern unsigned long currentTime;
extern int period;
extern int dirX;
extern int dirY;
extern int foodX;
extern int foodY;
extern int howHard;
extern bool gOver;

// TFT_eSprite object
extern TFT_eSprite sprite;

#endif // SNAKE_H
