#ifndef SPLIT_SCREEN_SNAKE_GAME_H
#define SPLIT_SCREEN_SNAKE_GAME_H

#include <VGA.h>
#include "GfxWrapper.h"

extern bool gameEnded;
extern bool gOver;

extern bool ready;
extern long readyTime;

// Function declarations
void getFood1();
void getFood2();
void checkGameOver1();
void checkGameOver2();
void runPlayer1();
void runPlayer2();
void runSplitScreenSnakeGame();
void setupSplitScreenSnakeGame();

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

#endif // SPLIT_SCREEN_SNAKE_GAME_H
