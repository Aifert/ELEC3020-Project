#ifndef SNAKE_H
#define SNAKE_H

#include <TFT_eSPI.h>

// Function declarationsa
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
extern bool gameEnded;


extern bool lastBigButtonState1;
extern bool currentBigButtonState1;
extern bool lastYButtonState1;
extern bool currentYButtonState1;
extern bool lastAButtonState1;
extern bool currentAButtonState1;

extern bool lastBigButtonState2;
extern bool currentBigButtonState2;
extern bool lastYButtonState2;
extern bool currentYButtonState2;
extern bool lastAButtonState2;
extern bool currentAButtonState2;
// TFT_eSprite object

extern TFT_eSprite sprite;

#endif // SNAKE_H
