#include "ESP32S3VGA.h"
#include "main.h"

// void showLeaderboard() {
//     // Retrieve scores from EEPROM
//     EEPROM.get(SNAKE_HIGH_SCORE_ADDRESS, highScores);
//     EEPROM.get(REACTION_HIGH_SCORE_ADDRESS, reactionHighScores);

//     // Clear the VGA screen before displaying the leaderboard
//     vga.clear(vga.rgb(0, 0, 0));  // Clear VGA with a black background

//     // Set column headers
//     gfx->setCursor(20, 20);  // Set position for the header
//     gfx->println("Snake High Scores    Reaction High Scores");

//     // Display the high scores in two columns with numbering
//     for (int i = 0; i < 3; i++) {
//         // Set position for each row (y-position based on the index i)
//         gfx->setCursor(20, 40 + i * 20);  // x = 20, y = 40 + (i * 20) to space rows

//         // Display the number label (1., 2., 3.)
//         gfx->print(i + 1);  // Print the number (1, 2, 3)
//         gfx->print(". ");

//         // Display snakeHighScores (left column)
//         gfx->print("Size: ");
//         gfx->print(highScores[i].size);
//         gfx->print(" Speed: ");
//         gfx->print(highScores[i].speed);

//         // Move the cursor to display reactionHighScores (right column)
//         gfx->setCursor(240, 40 + i * 20);  // Move to x = 240 for the right column
//         gfx->print(i + 1);  // Print the number (1, 2, 3)
//         gfx->print(". ");
//         gfx->print(reactionHighScores[i]);
//     }

//     // Show everything on the VGA display after drawing
//     vga.show();
// }
void showLeaderboard() {
    // Retrieve scores from EEPROM
    EEPROM.get(SNAKE_HIGH_SCORE_ADDRESS, highScores);
    EEPROM.get(REACTION_HIGH_SCORE_ADDRESS, reactionHighScores);

    // Clear the VGA screen before displaying the leaderboard
    vga.clear(vga.rgb(0, 0, 0));  // Clear VGA with a black background

    // Add "Press Y to return to main menu" text
    gfx->setCursor(10, 10);
    gfx->setTextColor(vga.rgb(255, 255, 255));
    gfx->print("< Press Y to return to the main menu");

    // Set column headers
    gfx->setCursor(20, 40);  // Moved down to accommodate the new text
    gfx->println("Snake High Scores    Reaction High Scores");

    // Display the high scores in two columns with numbering
    for (int i = 0; i < 3; i++) {
        gfx->setCursor(20, 60 + i * 20);  // Adjusted y-position

        gfx->print(i + 1);
        gfx->print(". ");

        gfx->print("Size: ");
        gfx->print(highScores[i].size);
        gfx->print(" Speed: ");
        gfx->print(highScores[i].speed);

        gfx->setCursor(240, 60 + i * 20);  // Adjusted y-position
        gfx->print(i + 1);
        gfx->print(". ");
        gfx->print(reactionHighScores[i]);
        gfx->print(" ms");
    }

    vga.show();

    // Wait for Y button press to return to main menu
    while (true) {
        if (controller1.y == 0 || controller2.y == 0) {
            // Y button pressed, return to main menu
            Serial.print("pressed going out now");
            return;
        }
        delay(50);
    }

    delay(10);
}
