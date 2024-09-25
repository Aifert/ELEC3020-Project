#ifndef MAIN_H
#define MAIN_H

#include <TFT_eSPI.h>

extern TFT_eSPI tft;

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

// Function prototypes
void setup();
void loop();

#endif // MAIN_H
