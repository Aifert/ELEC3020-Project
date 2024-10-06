#include <VGA.h>
#include "GfxWrapper.h"
#include "../main.h"
#include "splitScreenSnakeGame.h"
#include "audioFile.h"
#include "Snake.h"

int size1 = 1, size2 = 1;
int yPos1[120] = {0}, yPos2[120] = {0};
int x1[120] = {0}, x2[120] = {0};
unsigned long currentTime1 = 0, currentTime2 = 0;
int period1 = 200, period2 = 200;
int dirX1 = 1, dirY1 = 0, dirX2 = 1, dirY2 = 0;
bool gOver1 = 0, gOver2 = 0;
int foodX1 = 0, foodY1 = 0, foodX2 = 0, foodY2 = 0;
bool initialised = false;
bool clearedOnce = false;

// Terrain and snake colors for Player 1 and Player 2
unsigned short colors1[2] = {0x48ED, 0x590F};
unsigned short snakeColor1[2] = {0x9FD3, 0x38C9};
unsigned short colors2[2] = {0x7832, 0xA125};
unsigned short snakeColor2[2] = {0x1267, 0x4856};

void initializeSnakePositions() {
    // Initialize Snake 1's random position within Player 1's area (ensure at least 3 blocks from the border)
    x1[0] = random(3, 12);  // Random X position between 3 and 12
    yPos1[0] = random(3, 12);  // Random Y position between 3 and 12

    // Initialize Snake 2's random position within Player 2's area (ensure at least 3 blocks from the border)
    x2[0] = random(18, 27);  // Random X position between 18 and 27 for Player 2's frame
    yPos2[0] = random(3, 12);  // Random Y position between 3 and 12
}

// Helper functions for food placement
void getFood1() {
    bool validPosition;
    do {
        validPosition = true;
        // Ensure food is not placed outside the 1-14 range for both x and y
        foodX1 = random(1, 15);
        foodY1 = random(1, 15);
        // Ensure food is not on the snake's tail
        for (int i = 0; i < size1; i++) {
            if (foodX1 == x1[i] && foodY1 == yPos1[i]) {
                validPosition = false;
                break;
            }
        }
    } while (!validPosition);
}

// Helper functions for food placement for Player 2
void getFood2() {
    bool validPosition;
    do {
        validPosition = true;
        // Ensure food is placed within the 15-29 x range and 1-14 y range
        foodX2 = random(15, 30);
        foodY2 = random(1, 15);
        // Ensure food is not on the snake's tail
        for (int i = 0; i < size2; i++) {
            if (foodX2 == x2[i] && foodY2 == yPos2[i]) {
                validPosition = false;
                break;
            }
        }
    } while (!validPosition);
}

// Check if game is over for Player 1
void checkGameOver1() {
    // Adjusted boundaries for Player 1's 150x150 area
    if (x1[0] < 0 || x1[0] >= 15 || yPos1[0] < 0 || yPos1[0] >= 15) gOver1 = true;
    for (int i = 1; i < size1; i++) {
        if (x1[i] == x1[0] && yPos1[i] == yPos1[0]) gOver1 = true;
    }
}

// Update the check for game over for Player 2
void checkGameOver2() {
    // Adjusted boundaries for Player 2's 150x150 area
    if (x2[0] < 15 || x2[0] >= 30 || yPos2[0] < 0 || yPos2[0] >= 15) gOver2 = true;
    for (int i = 1; i < size2; i++) {
        if (x2[i] == x2[0] && yPos2[i] == yPos2[0]) gOver2 = true;
    }
}
// Update the direction of Snake 1 based on Player 1's input
void updateDirectionPlayer1Left() {
    if (currentYButtonState1 == 0 && lastYButtonState1 == 1 ) {
        playSnakeLeft();
        int temp = dirX1;
        dirX1 = dirY1;
        dirY1 = -temp;
        ready = 0;
        readyTime = millis();
    }
}

void updateDirectionPlayer1Right() {
    if (currentAButtonState1 == 0 && lastAButtonState1 == 1 ) {
        playSnakeRight();
        int temp = dirX1;
        dirX1 = -dirY1;
        dirY1 = temp;
        ready = 0;
        readyTime = millis();
    }
}
void updateDirectionPlayer2Left() {
    if (currentYButtonState2 == 0 && lastYButtonState2 == 1 && ready == 1) {
        playSnakeLeft();
        int temp = dirX2;
        dirX2 = dirY2;
        dirY2 = -temp;
        ready = 0;
        readyTime = millis();
    }
}

void updateDirectionPlayer2Right() {
    if (currentAButtonState2 == 0 && lastAButtonState2 == 1 && ready == 1) {
        playSnakeRight();
        int temp = dirX2;
        dirX2 = -dirY2;
        dirY2 = temp;
        ready = 0;
        readyTime = millis();
    }
}


// Draw the game for Player 1
void runPlayer1() {
    for (int i = size1; i > 0; i--) {
        x1[i] = x1[i - 1];
        yPos1[i] = yPos1[i - 1];
    }
    x1[0] = x1[0] + dirX1;
    yPos1[0] = yPos1[0] + dirY1;

    if (x1[0] == foodX1 && yPos1[0] == foodY1) {
        size1++;
        getFood1();
        period1 = period1 - 1;
    }


    gfx->fillRect(0, 50, 150, 150, vga.rgb(0, 0, 0));  // Clear Player 1's area to be 150x150 pixels
    checkGameOver1();
    if (!gOver1) {
        gfx->drawRect(0, 50, 150, 150, vga.rgb(0, 255, 255));  // Adjust Player 1's border to 150x150
        for (int i = 0; i < size1; i++) {
            gfx->fillRect(x1[i] * 10, yPos1[i] * 10 + 50, 10, 10, vga.rgb(100, 255, 0));  // Draw snake in 150x150 area
        }
        gfx->fillRect(foodX1 * 10 + 1, foodY1 * 10 + 51, 8, 8, vga.rgb(255, 0, 0));  // Draw food in 150x150 area
    } else {
        vga.clear(vga.rgb(0x00, 0x00, 0x00));
        gfx->setCursor(20, 160);  // Adjust text position for game over
        gfx->print("Player 1 Game Over");

        gameOverSound();
    }
}

// Draw the game for Player 2
void runPlayer2() {
    for (int i = size2; i > 0; i--) {
        x2[i] = x2[i - 1];
        yPos2[i] = yPos2[i - 1];
    }
    x2[0] = x2[0] + dirX2;
    yPos2[0] = yPos2[0] + dirY2;

    if (x2[0] == foodX2 && yPos2[0] == foodY2) {
        size2++;
        getFood2();
        period2 = period2 - 1;
    }

    gfx->fillRect(160, 50, 150, 150, vga.rgb(0, 0, 0));  // Clear Player 2's area (150x150 pixels)
    checkGameOver2();
    if (!gOver2) {
        gfx->drawRect(160, 50, 150, 150, vga.rgb(255, 0, 0));  // Draw Player 2's border (150x150)

        // Adjust snake drawing for Player 2 (relative to Player 2's box starting at 160)
        for (int i = 0; i < size2; i++) {
            gfx->fillRect((x2[i] - 15) * 10 + 160, yPos2[i] * 10 + 50, 10, 10, vga.rgb(0, 100, 255));  // Draw snake inside Player 2's area
        }

        // Adjust food drawing for Player 2
        gfx->fillRect((foodX2 - 15) * 10 + 160, foodY2 * 10 + 51, 8, 8, vga.rgb(255, 0, 0));  // Draw food inside Player 2's area
    } else {
        gfx->setCursor(200, 160);  // Game over message for Player 2
        gfx->print("Player 2 Game Over");

        gameOverSound();
    }

}


// Run the split-screen snake game for two players
void runSplitScreenSnakeGame() {
    if (millis() > currentTime1 + period1 && !gOver1) {
        runPlayer1();
        // runPlayer2();
        currentTime1 = millis();
    }

    // Update directions based on player input
    // Controller 1
    lastBigButtonState1 = currentBigButtonState1;
    currentBigButtonState1 = controller1.big;
    lastYButtonState1 = currentYButtonState1;
    currentYButtonState1 = controller1.y;
    lastAButtonState1 = currentAButtonState1;
    currentAButtonState1 = controller1.a;

    // // Controller 2
    lastBigButtonState2 = currentBigButtonState2;
    currentBigButtonState2 = controller2.big;
    lastYButtonState2 = currentYButtonState2;
    currentYButtonState2 = controller2.y;
    lastAButtonState2 = currentAButtonState2;
    currentAButtonState2 = controller2.a;


    updateDirectionPlayer1Left();
    updateDirectionPlayer1Right();

    // updateDirectionPlayer2Left();
    // updateDirectionPlayer2Right();

    if (millis() > readyTime + 100 && ready == 0) {
        ready = 1;
    }

    vga.show();

    delay(20);

}

// void drawSplitScreenMainMenu(){
//     vga.clear(vga.rgb(0x00, 0x00, 0x00));

//     if (!initialised){
//         gfx->setCursor(10, 10);
//         gfx->setTextColor(vga.rgb(255, 255, 255));
//         gfx->print("< Press Y to return to the main menu");

//         gfx->setCursor(40, 20);
//         gfx->setTextColor(vga.rgb(255, 255, 255));
//         gfx->print("Two-Player Snake Game");

//         gfx->setCursor(40, 30);
//         gfx->print("Press Big to start the game");

//         Serial.print("drawing main menu\n");

//         vga.show();
//     }
// }

void clearTextArea() {
    // Clear the space above the player areas (0 to 50 pixels) where text is displayed
    gfx->fillRect(0, 0, 320, 50, vga.rgb(0, 0, 0));  // Clear top area (width 320, height 50)
    clearedOnce = true; // Set the cleared flag to true

}

void setupSplitScreenSnakeGame() {
    size1 = 1, size2 = 1;
    dirX1 = 1, dirY1 = 0;
    dirX2 = 1, dirY2 = 0;
    gOver1 = 0, gOver2 = 0;
    initialised = false;

    // drawSplitScreenMainMenu();

    initialised = true;

    // Wait for "Big" button to start the game
    while (controller1.big == controller2.big || controller2.big == controller1.big) {
        if (controller2.y == 0 || controller1.y == 0) {
            return;
        }
        delay(100);

    }


    vga.clear(0);
    vga.show();

    initializeSnakePositions();

    Serial.print("intialising snake positions");

    getFood1();
    getFood2();
    runPlayer1();
    runPlayer2();

    vga.show();

    Serial.print("Starting game");

    delay(2500);

    // Maybe play game start here

    playCantina();

    while (true) {

        runSplitScreenSnakeGame();

        if (gOver1 || gOver2) {

            vga.clear(vga.rgb(0x00, 0x00, 0x00));
            delay(3000);
            // Reset game state if game ends
            if (gOver1 && !gOver2) {
                vga.clear(vga.rgb(0x00, 0x00, 0x00));
                gfx->setCursor(100, 80);
                gfx->setTextColor(vga.rgb(255, 255, 255));
                gfx->print("Player 2 Wins!");
                vga.show();
                delay(3000);
            }

            // End the game if Player 2 loses
            if (gOver2 && !gOver1) {
                vga.clear(vga.rgb(0x00, 0x00, 0x00));
                gfx->setCursor(100, 80);
                gfx->setTextColor(vga.rgb(255, 255, 255));
                gfx->print("Player 1 Wins!");
                vga.show();
                delay(3000);
            }

            size1 = 1, size2 = 1;
            dirX1 = 1, dirY1 = 0;
            dirX2 = 1, dirY2 = 0;
            gOver1 = 0, gOver2 = 0;
            initialised = false;
            break;
        }
    }
}
