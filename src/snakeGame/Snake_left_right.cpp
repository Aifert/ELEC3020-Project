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

typedef struct snakehHighScores {
    int size;
    int speed;
} snakeHighScores;


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
        // Draw the game border starting at (0, 50), matching the newGame image location
        gfx->drawRect(0, 50, 170, 170, vga.rgb(0, 255, 255));  // Adjusted game border

        // Display food collected next to the rectangle (right side)
        gfx->setCursor(180, 60);  // Set cursor position for the food collection message
        gfx->setTextColor(vga.rgb(255, 255, 255));  // Set text color to white
        gfx->print("Food collected: ");  // Print the food collection message
        gfx->print(String(size));  // Print the size

        // Display speed next to the rectangle (below the food message)
        gfx->setCursor(180, 80);  // Set cursor position for the remaining period
        gfx->print("Speed: ");
        gfx->print(String(500 - period));  // Print the remaining period

        // Draw the snake within the adjusted rectangle
        for (int i = 0; i < size; i++) {
            gfx->fillRect(x[i] * 10, y[i] * 10 + 50, 10, 10, vga.rgb(100, 255, 0));  // Adjusted for new game rectangle
            gfx->fillRect(2 + x[i] * 10, 2 + y[i] * 10 + 50, 6, 6, vga.rgb(50, 150, 0));  // Adjusted for new game rectangle
        }

        // Draw the food within the adjusted rectangle
        gfx->fillRect(foodX * 10 + 1, foodY * 10 + 51, 8, 8, vga.rgb(255, 0, 0));  // Adjusted for new game rectangle
        gfx->fillRect(foodX * 10 + 3, foodY * 10 + 53, 4, 4, vga.rgb(255, 255, 0));  // Adjusted for new game rectangle
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

void drawDifficultyDot(int howHard, snakeHighScores highScores) {
    int yPos;

    // Load high scores from EEPROM
    int highScoreFood = highScores.size;
    int highScoreSpeed = highScores.speed;

    // Clear the screen only once, not repeatedly
    vga.clear(vga.rgb(0x00, 0x00, 0x00));
    gfx->drawRGBBitmap(0, 50, newGame, 170, 170);

    // Display instructions
    gfx->setCursor(10, 10);
    gfx->setTextColor(vga.rgb(255, 255, 255));
    gfx->print("< Press Y to return to the main menu");


    gfx->setCursor(175, 70);  // Adjusted to prevent overlap
    gfx->print("Multiplayer: ");

    // Display high scores
    gfx->setCursor(175, 90);  // Adjusted to prevent overlap
    gfx->print("Top food: ");
    gfx->print(String(highScoreFood));

    gfx->setCursor(175, 110);  // Adjusted to prevent overlap
    gfx->print("Top speed: ");
    gfx->print(String(highScoreSpeed));

    // Determine y-coordinate based on difficulty
    switch (howHard) {
        case 0:  // Easy
            yPos = 120;
            break;
        case 1:  // Normal
            yPos = 150;
            break;
        case 2:  // Hard
            yPos = 170;
            break;
        case 3:
            yPos = 73;
            break;
    }

    // Only clear the previous dot and draw the new one
    if (previousHowHard != -1) {
        int prevYPos;
        switch (previousHowHard) {
            case 0: prevYPos = 120; break;
            case 1: prevYPos = 150; break;
            case 2: prevYPos = 170; break;
            case 3: prevYPos = 73; break;
        }
        gfx->fillCircle(28, prevYPos, 5, vga.rgb(0, 0, 0));  // Clear the previous dot
    }

    // Draw the current dot
    if (howHard != 3) {
        gfx->fillCircle(28, yPos, 5, vga.rgb(255, 0, 0));  // Draw the red dot at the new position
    }

    // Draw a circle next to "Multiplayer:" if multiplayer mode is selected
    if (howHard == 3) {
        gfx->fillCircle(260, yPos, 5, vga.rgb(255, 0, 0));  // Draw circle next to "Multiplayer:"
    } else {
        // Clear the circle if it's not in multiplayer mode
        gfx->fillCircle(260, yPos, 5, vga.rgb(0, 0, 0));  // Clear the circle if not in multiplayer mode
    }

    // Store the current difficulty as the previous one
    previousHowHard = howHard;
}

void setupSnakeGame() {
    // Clear screen and draw the 'newGame' image
    vga.clear(vga.rgb(0x00, 0x00, 0x00));
    gfx->drawRGBBitmap(0, 50, newGame, 170, 170);  // Display the newGame screen
    vga.show();

    snakeHighScores highScores;
    EEPROM.get(SNAKE_HIGH_SCORE_ADDRESS, highScores);

    size = 1;
    period = 200;
    howHard = 0;
    gOver = false;

    drawDifficultyDot(howHard, highScores);
    vga.show();

    // Wait for user input to select difficulty
    while(controller1.big == controller2.big || controller2.big == controller1.big){
        if (controller2.y == 0 || controller1.y == 0) {
            return;
        }
        if (controller2.b == 0 || controller1.b == 0) {
            if (deb2 == 0) {
                deb2 = 1;

                // Change difficulty when 'B' is pressed
                howHard++;
                if (howHard == 4) howHard = 0;  // Wrap around to Easy after Hard

                // Redraw the dot based on the new difficulty
                drawDifficultyDot(howHard, highScores);
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

            if (size > highScores.size && (500 - period > highScores.speed)) {
                highScores.size = size;
                highScores.speed = 500 - period;
                EEPROM.put(SNAKE_HIGH_SCORE_ADDRESS, highScores);
                EEPROM.commit();

                vga.clear(vga.rgb(0x00, 0x00, 0x00));
                gfx->setCursor(50, 50);
                gfx->setTextColor(vga.rgb(255, 255, 255));
                gfx->print("New High Score!");

                vga.show();
            } else {
                vga.clear(vga.rgb(0x00, 0x00, 0x00));
                gfx->setCursor(50, 50);
                gfx->setTextColor(vga.rgb(255, 255, 255));
                gfx->print("Previous High Score: ");
                gfx->print(String(highScores.size));
                gfx->setCursor(50, 70);
                gfx->print("Previous High Speed: ");
                gfx->print(String(highScores.speed));

                vga.show();
            }

            delay(3500);

            size = 1;
            x[0] = 0;
            y[0] = random(5, 15);
            dirX = 1;
            dirY = 0;
            gOver = 0;

            break;
        }
    }
}
