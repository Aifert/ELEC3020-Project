#include "ESP32S3VGA.h"
#include <GfxWrapper.h>
#include <esp_now.h>
#include <WiFi.h>

// VGA pin configuration
const PinConfig pins(-1, -1, -1, -1, 43, -1, -1, -1, -1, -1, 44, -1, -1, -1, -1, 18, 1, 2);
VGA vga;
Mode mode = Mode::MODE_320x240x60;
GfxWrapper<VGA> gfx(vga, mode.hRes, mode.vRes);

// Game display settings
const int screenWidth = 320;
const int screenHeight = 240;
const int midScreen = screenWidth / 2;

// Snake settings
const int snakeSize = 5;
const int maxSnakeLength = 50; // Arbitrary max length for now
int snake1X[maxSnakeLength] = {50}, snake1Y[maxSnakeLength] = {120}, snake2X[maxSnakeLength] = {270}, snake2Y[maxSnakeLength] = {120};
int snake1DX = 5, snake1DY = 0, snake2DX = 5, snake2DY = 0;
int snake1Length = 1, snake2Length = 1; // Track the length of each snake
int snake1Score = 0, snake2Score = 0;
int player1Wins = 0, player2Wins = 0;
bool gameEnded = false;
bool player1Lost = false, player2Lost = false;

// Fruit settings
int fruit1X, fruit1Y, fruit2X, fruit2Y;

struct struct_message {
    int id;
    int a; // Button A (left)
    int b; // Button B (right)
    int x; // Button X (up)
    int y; // Button Y (down)
};

struct_message controller1;
struct_message controller2;

void resetGame();
void updateSnakes();
void checkCollision();
void drawGame();
void drawWinScreen();
void OnDataRecv(const uint8_t *mac, const uint8_t *incomingData, int len);   // Receive data from controllers (esp-now)
void placeFruit(int &fruitX, int &fruitY);
void drawCenterLine();

void setup() {
    // Setup and initialize VGA
    vga.bufferCount = 2;
    if (!vga.init(pins, mode, 16)) {
        while (1) delay(1);
    }
    vga.start();

    // Initialize ESP-NOW
    WiFi.mode(WIFI_STA);
    if (esp_now_init() != ESP_OK) {
        while (1) delay(1000);
    }
    esp_now_register_recv_cb(OnDataRecv);

    resetGame();
}

void loop() {
    if (!gameEnded) {
        updateSnakes();   
        drawGame();   
    } else {
        drawWinScreen(); 
    }
    delay(100);  // Control game speed
}

void OnDataRecv(const uint8_t *mac, const uint8_t *incomingData, int len) {
    struct_message incomingMessage;
    memcpy(&incomingMessage, incomingData, sizeof(incomingMessage));

    if (incomingMessage.id == 1) {
        controller1 = incomingMessage; // Update Player 1 controller data
    } else if (incomingMessage.id == 2) {
        controller2 = incomingMessage; // Update Player 2 controller data
    }
}

void updateSnakes() {
    // Player 1 controls (controller1)
    if (controller1.a == 0) { 
        snake1DX = -snakeSize; 
        snake1DY = 0;  // Move Left
    }
    if (controller1.b == 0) { 
        snake1DX = snakeSize; 
        snake1DY = 0;   // Move Right
    }
    if (controller1.x == 0) { 
        snake1DX = 0; 
        snake1DY = -snakeSize;  // Move Up
    }
    if (controller1.y == 0) { 
        snake1DX = 0; 
        snake1DY = snakeSize;   // Move Down
    }

    // Player 2 controls (controller2)
    if (controller2.a == 0) { 
        snake2DX = -snakeSize; 
        snake2DY = 0;  // Move Left
    }
    if (controller2.b == 0) { 
        snake2DX = snakeSize; 
        snake2DY = 0;   // Move Right
    }
    if (controller2.x == 0) { 
        snake2DX = 0; 
        snake2DY = -snakeSize;  // Move Up
    }
    if (controller2.y == 0) { 
        snake2DX = 0; 
        snake2DY = snakeSize;   // Move Down
    }

    // Update Player 1's snake body (increases the length of the snake body)
    for (int i = snake1Length - 1; i > 0; i--) {
        snake1X[i] = snake1X[i - 1];
        snake1Y[i] = snake1Y[i - 1];
    }
    snake1X[0] += snake1DX;
    snake1Y[0] += snake1DY;

    // Update Player 2's snake body (increase the length of the snake body)
    for (int i = snake2Length - 1; i > 0; i--) {
        snake2X[i] = snake2X[i - 1];
        snake2Y[i] = snake2Y[i - 1];
    }
    snake2X[0] += snake2DX;
    snake2Y[0] += snake2DY;

    //Player 1 hits the screen borders or midline
    if (snake1X[0] < 0 || snake1X[0] >= midScreen || snake1Y[0] < 0 || snake1Y[0] >= screenHeight) {
        player1Lost = true;
        gameEnded = true;
    }

    //Player 2 hits the screen borders or midline
    if (snake2X[0] < midScreen || snake2X[0] >= screenWidth || snake2Y[0] < 0 || snake2Y[0] >= screenHeight) {
        player2Lost = true;
        gameEnded = true;
    }

    //Player 1 collides with itself
    for (int i = 1; i < snake1Length; i++) {
        if (snake1X[0] == snake1X[i] && snake1Y[0] == snake1Y[i]) {
            player1Lost = true;
            gameEnded = true;
        }
    }

    //Player 2 collides with itself
    for (int i = 1; i < snake2Length; i++) {
        if (snake2X[0] == snake2X[i] && snake2Y[0] == snake2Y[i]) {
            player2Lost = true;
            gameEnded = true;
        }
    }
}

// Check collisions with fruit if there is a collision then increase the length of the snake.
void checkCollision() {
    if (snake1X[0] == fruit1X && snake1Y[0] == fruit1Y) {
        snake1Score++;
        placeFruit(fruit1X, fruit1Y);
        snake1Length++; // Increase Player 1 snake length
    }

    if (snake2X[0] == fruit2X && snake2Y[0] == fruit2Y) {
        snake2Score++;
        placeFruit(fruit2X, fruit2Y);
        snake2Length++; // Increase Player 2 snake length
    }
}

void placeFruit(int &fruitX, int &fruitY) {
    if (&fruitX == &fruit1X) { // Player 1's fruit
        fruitX = random(0, midScreen / snakeSize) * snakeSize;
        fruitY = random(0, screenHeight / snakeSize) * snakeSize;
    } else { // Player 2's fruit
        fruitX = random(midScreen / snakeSize, screenWidth / snakeSize) * snakeSize;
        fruitY = random(0, screenHeight / snakeSize) * snakeSize;
    }
}

void drawCenterLine() {
    for(int y = 0; y < screenHeight; y += 10) {
        gfx.fillRect(midScreen, y, 1, 4, vga.rgb(255, 255, 255));
    }
}

void drawGame() {
    gfx.clear(vga.rgb(0x00, 0x00, 0x00));

    drawCenterLine();

    // Player 1 snake & fruit
    for (int i = 0; i < snake1Length; i++) {
        gfx.fillRect(snake1X[i], snake1Y[i], snakeSize, snakeSize, vga.rgb(0x00, 0xFF, 0x00));  // Green snake
    }
    gfx.fillRect(fruit1X, fruit1Y, snakeSize, snakeSize, vga.rgb(0xFF, 0x00, 0x00));  // Red fruit

    // Player 2 snake & fruit
    for (int i = 0; i < snake2Length; i++) {
        gfx.fillRect(snake2X[i], snake2Y[i], snakeSize, snakeSize, vga.rgb(0x00, 0x00, 0xFF));  // Blue snake
    }
    gfx.fillRect(fruit2X, fruit2Y, snakeSize, snakeSize, vga.rgb(0xFF, 0x00, 0x00));  // Red fruit

    gfx.commit();
}

void drawWinScreen() {
    gfx.clear(vga.rgb(0x00, 0x00, 0x00));

    gfx.setCursor(midScreen - 50, screenHeight / 2 - 10);
    gfx.setTextColor(vga.rgb(255, 255, 255));

    if (player1Lost) {
        gfx.print("Player 2 Wins!");
        player2Wins++;
    } else if (player2Lost) {
        gfx.print("Player 1 Wins!");
        player1Wins++;
    } else {
        gfx.print("It's a Tie!");
    }

    gfx.setCursor(midScreen - 70, screenHeight / 2 + 20);
    gfx.print("Press Y to Restart");
    gfx.commit();

    // Restart condition: press 'Y' to restart game
    if (controller1.y == 0 || controller2.y == 0) {
        resetGame();
    }
}

void resetGame() {
    snake1X[0] = 50;
    snake1Y[0] = 120;
    snake2X[0] = 270;
    snake2Y[0] = 120;
    snake1Length = 1;
    snake2Length = 1;
    snake1DX = 5;
    snake1DY = 0;
    snake2DX = 5;
    snake2DY = 0;
    snake1Score = 0;
    snake2Score = 0;
    player1Lost = false;
    player2Lost = false;
    gameEnded = false;

    placeFruit(fruit1X, fruit1Y);
    placeFruit(fruit2X, fruit2Y);
}
