// #include <TFT_eSPI.h>
// #include <joystick.h>
// #include <EEPROM.h>

// #define EEPROM_SIZE 512
// #define EEPROM_VOLUME_ADDRESS 0

// TFT_eSPI tft = TFT_eSPI(); // Define the tft object here

// const int buttonA = 0;
// const int buttonB = 14;
// int notSelected = 0;

// void setup() {
//   Serial.begin(115200);
//   pinMode(buttonA, INPUT_PULLUP);
//   pinMode(buttonB, INPUT_PULLUP);

//   if (!EEPROM.begin(EEPROM_SIZE)) {
//     Serial.println("Failed to initialize EEPROM");
//     // Handle error as needed
//   } else {
//     Serial.println("EEPROM initialized");
//   }


//   tft.init();        // Initialize the display
//   tft.setRotation(1); // Set the orientation (optional, you can change 0-3 for different rotations)

//   tft.fillScreen(TFT_BLACK); // Clear the screen and set it to black
//   tft.setTextColor(TFT_WHITE, TFT_BLACK); // Set text color and background color

//   tft.setTextSize(2); // Set text size (change for bigger/smaller text)

//   // Center text horizontally by calculating the text width and using half of the screen width
//   String option1 = "Press 0 for Cross Road";
//   String option2 = "Press 14 for Flappy Bird";

//   int screenWidth = tft.width();
//   int option1Width = tft.textWidth(option1); // Get the width of the option1 text
//   int option2Width = tft.textWidth(option2); // Get the width of the option2 text

//   // Calculate the X positions for both options (centered horizontally)
//   int option1X = (screenWidth - option1Width) / 2;
//   int option2X = (screenWidth - option2Width) / 2;

//   // Set Y positions to center vertically as needed
//   int option1Y = 50;
//   int option2Y = option1Y + 30;

//   tft.setCursor(option1X, option1Y);
//   tft.print(option1);

//   tft.setCursor(option2X, option2Y);
//   tft.print(option2);
// }

// void loop() {
//   while(!notSelected){
//     int buttonAState = digitalRead(buttonA);
//     int buttonBState = digitalRead(buttonB);

//     if (buttonAState == LOW) {
//       tft.fillScreen(TFT_BLACK);
//       tft.setCursor(10, 10);
//       tft.print("Option 1 Selected");

//       // Call the joystick control function here
//       runJoystickControl();

//       notSelected = 1;
//       break;
//     } else if (buttonBState == LOW) {
//       tft.fillScreen(TFT_BLACK);
//       tft.setCursor(10, 10);
//       tft.print("Option 2 Selected");

//       // Implement the logic for Flappy Bird or other game here

//       notSelected = 1;
//       break;
//     }
//   }
// }
