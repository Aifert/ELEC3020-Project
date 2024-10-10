#include "ESP32S3VGA.h"
#include "main.h"

void showLeaderboard() {
    // Retrieve scores from EEPROM
    EEPROM.get(SNAKE_HIGH_SCORE_ADDRESS, highScores);
    EEPROM.get(REACTION_HIGH_SCORE_ADDRESS, reactionHighScores);

    // Clear the VGA screen before displaying the leaderboard
    vga.clear(vga.rgb(0, 0, 0));  // Clear VGA with a black background

    // Set column headers
    gfx->setCursor(20, 20);  // Set position for the header
    gfx->println("Snake High Scores    Reaction High Scores");

    // Display the high scores in two columns with numbering
    for (int i = 0; i < 3; i++) {
        // Set position for each row (y-position based on the index i)
        gfx->setCursor(20, 40 + i * 20);  // x = 20, y = 40 + (i * 20) to space rows

        // Display the number label (1., 2., 3.)
        gfx->print(i + 1);  // Print the number (1, 2, 3)
        gfx->print(". ");

        // Display snakeHighScores (left column)
        gfx->print("Size: ");
        gfx->print(highScores[i].size);
        gfx->print(" Speed: ");
        gfx->print(highScores[i].speed);

        // Move the cursor to display reactionHighScores (right column)
        gfx->setCursor(240, 40 + i * 20);  // Move to x = 240 for the right column
        gfx->print(i + 1);  // Print the number (1, 2, 3)
        gfx->print(". ");
        gfx->print(reactionHighScores[i]);
    }

    // Show everything on the VGA display after drawing
    vga.show();
}