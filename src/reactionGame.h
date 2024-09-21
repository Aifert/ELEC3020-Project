#ifndef REACTION_GAME_H
#define REACTION_GAME_H

#include <TFT_eSPI.h>
#include <EEPROM.h>

// Function prototypes
void setHighScore(int highScore);
void processButtonClick(int& highScore);
void runReactionGame();

// Constants
extern const int SwitchPin;

// Global variables
extern TFT_eSPI tft;
extern unsigned long startTime;
extern bool gameStarted;

#endif // REACTION_GAME_H
