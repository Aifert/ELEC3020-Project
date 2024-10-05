#include <VGA.h>
#include "GfxWrapper.h"
#include "../main.h"
#include "back.h"
#include "gameOver.h"
#include "main.h"
#include "newGame.h"
#include "Snake.h"
#include "audioFile.h"

int size = 1;
int y[120] = {0};
int x[120] = {0};

unsigned long currentTime = 0;
int period = 200;
int deb, deb2 = 0;
int dirX = 1;
int dirY = 0;
bool taken = 0;
unsigned short colors[2] = {0x48ED, 0x590F};  // terrain colors
unsigned short snakeColor[2] = {0x9FD3, 0x38C9};
bool chosen = 0;
bool gOver = 0;
int moves = 0;
int playTime = 0;
int foodX = 0;
int foodY = 0;
int howHard = 0;
String diff[3] = {"EASY", "NORMAL", "HARD"};
bool ready = 1;
long readyTime = 0;
int previousHowHard = -1;


// Button state variables
bool lastBigButtonState1 = true;
bool currentBigButtonState1 = true;
bool lastYButtonState1 = true;
bool currentYButtonState1 = true;
bool lastBButtonState1 = true;
bool currentAButtonState1 = true;

bool lastBigButtonState2 = true;
bool currentBigButtonState2 = true;
bool lastYButtonState2 = true;
bool currentYButtonState2 = true;
bool lastBButtonState2 = true;
bool currentAButtonState2 = true;

void getFood() {
    foodX = random(0, 17);
    foodY = random(0, 17);
    taken = 0;
    for (int i = 0; i < size; i++)
        if (foodX == x[i] && foodY == y[i])
            taken = 1;
    if (taken == 1)
        getFood();
}

int change = 0;

void checkGameOver() {
    if (x[0] < 0 || x[0] >= 17 || y[0] < 0 || y[0] >= 17)
        gOver = true;
    for (int i = 1; i < size; i++)
        if (x[i] == x[0] && y[i] == y[0])
            gOver = true;
}

void run() {
    for (int i = size; i > 0; i--) {
        x[i] = x[i - 1];
        y[i] = y[i - 1];
    }

    x[0] = x[0] + dirX;
    y[0] = y[0] + dirY;

    if (x[0] == foodX && y[0] == foodY) {
        size++;
        getFood();
        period = period - 1;
    }

    gfx->fillScreen(vga.rgb(0, 0, 0));  // Clear the screen

    checkGameOver();
    if (!gOver) {
        gfx->drawRect(0, 0, 170, 170, vga.rgb(0, 255, 255));  // Draw the game border

        // Draw the snake
        for (int i = 0; i < size; i++) {
            gfx->fillRect(x[i] * 10, y[i] * 10, 10, 10, vga.rgb(100, 255, 0));
            gfx->fillRect(2 + x[i] * 10, 2 + y[i] * 10, 6, 6, vga.rgb(50, 150, 0));
        }

        // Draw the food
        gfx->fillRect(foodX * 10 + 1, foodY * 10 + 1, 8, 8, vga.rgb(255, 0, 0));
        gfx->fillRect(foodX * 10 + 3, foodY * 10 + 3, 4, 4, vga.rgb(255, 255, 0));
    } else {
        gfx->drawRGBBitmap(50, 50, gameOver, 170, 170);  // Draw game over image
    }

    vga.show();
}

void runSnakeGame() {
    if (millis() > currentTime + period) {
        run();
        currentTime = millis();
    }

    // Button handling
    lastBigButtonState2 = currentBigButtonState2;
    currentBigButtonState2 = controller2.big;
    lastYButtonState2 = currentYButtonState2;
    currentYButtonState2 = controller2.y;
    lastBButtonState2 = currentAButtonState2;
    currentAButtonState2 = controller2.a;

    lastBigButtonState1 = currentBigButtonState1;
    currentBigButtonState1 = controller1.big;
    lastYButtonState1 = currentYButtonState1;
    currentYButtonState1 = controller1.y;
    lastBButtonState1 = currentAButtonState1;
    currentAButtonState1 = controller1.a;

    if ((currentYButtonState1 == 0 && lastYButtonState1 == 1 && ready == 1)
        || (currentYButtonState2 == 0 && lastYButtonState2 == 1 && ready == 1)) {
        playSnakeLeft();
        int temp = dirX;
        dirX = dirY;
        dirY = -temp;
        ready = 0;
        readyTime = millis();
    }

    if ((currentAButtonState1 == 0 && lastBButtonState1 == 1 && ready == 1)
        || (currentAButtonState2 == 0 && lastBButtonState2 == 1 && ready == 1)) {
        playSnakeRight();
        int temp = dirX;
        dirX = -dirY;
        dirY = temp;
        ready = 0;
        readyTime = millis();
    }

    if (millis() > readyTime + 100 && ready == 0) {
        ready = 1;
    }
}

// Add this helper function to draw the difficulty dot
void drawDifficultyDot(int howHard) {
    int yPos;

    // Redraw the newGame image before drawing the dot
    vga.clear(vga.rgb(0x00, 0x00, 0x00));
    gfx->drawRGBBitmap(50, 50, newGame, 170, 170);
    gfx->setCursor(10, 10);
    gfx->setTextColor(vga.rgb(255, 255, 255));
    gfx->print("Press Y to return the main menu");


    // Determine y-coordinate based on difficulty
    switch (howHard) {
        case 0:  // Easy
            yPos = 120;
            break;
        case 1:  // Normal
            yPos = 140;
            break;
        case 2:  // Hard
            yPos = 160;
            break;
    }

    // Only clear the previous dot
    if (previousHowHard != -1) {
        int prevYPos;
        switch (previousHowHard) {
            case 0: prevYPos = 120; break;
            case 1: prevYPos = 150; break;
            case 2: prevYPos = 160; break;
        }
        gfx->fillCircle(78, prevYPos, 5, vga.rgb(0, 0, 0));  // Clear the previous dot

        gfx->drawRGBBitmap(50, 50, newGame, 170, 170);  // Redraw the newGame screen
    }

    // Draw the current dot
    gfx->fillCircle(78, yPos, 5, vga.rgb(255, 0, 0));  // Draw the red dot at the new position

    // Store the current difficulty as the previous one
    previousHowHard = howHard;
}


void setupSnakeGame() {
    // Clear screen and draw the 'newGame' image
    vga.clear(vga.rgb(0x00, 0x00, 0x00));

    gfx->drawRGBBitmap(50, 50, newGame, 170, 170);  // Display the newGame screen
    vga.show();

    size = 1;
    period = 200;
    howHard = 0;
    gOver = false;

    drawDifficultyDot(howHard);
    vga.show();

    // Wait for user input to select difficulty
    while (controller1.big == controller2.big) {
        if(controller2.y == 0 || controller1.y == 0){
            return;
        }
        if (controller2.b == 0 || controller1.b == 0) {
            if (deb2 == 0) {
                deb2 = 1;

                // Change difficulty when 'B' is pressed
                howHard++;
                if (howHard == 3) howHard = 0;  // Wrap around to Easy after Hard

                // Redraw the dot based on the new difficulty
                drawDifficultyDot(howHard);
                vga.show();

                // Adjust the game period based on difficulty
                period = 200 - howHard * 20;

                delay(200);
            }
        } else {
            deb2 = 0;
        }
        delay(100);
    }



    y[0] = random(5, 15);
    getFood();
    delay(400);
    dirX = 1;
    dirY = 0;

    while (true) {
        runSnakeGame();

        if (gOver == 1) {
            gameOverSound();
            size = 1;
            x[0] = 0;
            y[0] = random(5, 15);
            dirX = 1;
            dirY = 0;
            gOver = 0;

            delay(3500);
            break;
        }
    }
}
