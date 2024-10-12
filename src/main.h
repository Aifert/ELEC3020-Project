#ifndef MAIN_H
#define MAIN_H

#include <TFT_eSPI.h>
#include "ESP32S3VGA.h"
#include <GfxWrapper.h>
#include <EEPROM.h>
#include <esp_now.h>
#include <WiFi.h>
#include "toast.h"

extern TFT_eSPI tft;
extern VGA vga;
extern Mode mode;
extern GfxWrapper<VGA>* gfx;
// Structure definition
typedef struct struct_message {
    int id;
    int big;
    int a;
    int b;
    int x;
    int y;
} struct_message;

extern struct_message myData;
extern struct_message controller1;
extern struct_message controller2;

#define EEPROM_SIZE 4096
#define REACTION_HIGH_SCORE_ADDRESS 0
#define SNAKE_HIGH_SCORE_ADDRESS 12
#define PONG_HIGH_SCORE_ADDRESS 36

// Function prototypes
void setup();
void loop();

typedef struct snakeHighScores {
    int size;
    int speed;
} snakeHighScores;

extern snakeHighScores highScores[3];

extern int reactionHighScores[3];



#endif // MAIN_H

