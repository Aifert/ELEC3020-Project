// #include <TFT_eSPI.h>
// #include <joystick.h>
// #include <ESP32Servo.h>
// #include <EEPROM.h>  // Include EEPROM library

// // Initialize TFT
// TFT_eSPI tft = TFT_eSPI();

// // Define button pins
// const int buttonA = 0;
// const int buttonB = 14;

// // EEPROM settings
// #define EEPROM_SIZE 512
// #define EEPROM_VOLUME_ADDRESS 0  // Address to store volume

// struct GameSettings {
//   int volume;  // Volume level (0-100)
// };

// GameSettings settings;

// // Define joystick and servo pins
// const int SwitchPin = 1;
// const int xPin = 2;
// const int yPin = 3;
// const int servoPinX = 43;  // The pin for the SG90 servo

// Servo servoX;  // Create a servo object for X movement (controlling SG90)

// // Joystick smoothing parameters
// const int deadZone = 10;  // Set the dead zone threshold
// const int noiseThreshold = 5;  // Minimum difference to register movement
// const int numReadings = 10;  // Number of samples to average

// int readingsX[numReadings];   // Array to store readings for X
// int readIndex = 0;            // Index of the current reading
// int totalX = 0;               // Total of X readings
// int averageX = 0;             // The smoothed X value

// int neutralX = 512;           // Neutral X (centered position)

// // Function prototypes
// void displayMenu();
// void saveSettings();
// void loadSettings();
// void applyVolume(int volume);
// void setupJoystick();
// void detectPosition();
// void runJoystickControl();

// void setup() {
//   Serial.begin(115200);

//   pinMode(buttonA, INPUT_PULLUP);  // Use INPUT_PULLUP to avoid floating states
//   pinMode(buttonB, INPUT_PULLUP);
//   pinMode(SwitchPin, INPUT_PULLUP);

//   // Initialize EEPROM
//   if (!EEPROM.begin(EEPROM_SIZE)) {
//     Serial.println("Failed to initialize EEPROM");
//     // Handle error as needed
//   } else {
//     Serial.println("EEPROM initialized");
//   }

//   // Initialize the TFT display
//   tft.init();
//   tft.setRotation(1);
//   tft.fillScreen(TFT_BLACK);
//   tft.setTextColor(TFT_WHITE, TFT_BLACK);
//   tft.setTextSize(2);

//   servoX.attach(servoPinX);  // Attach the X servo (SG90) to the servoPinX

//   // Display menu options
//   displayMenu();
// }

// void loop() {
//   // Read button states
//   bool buttonAState = !digitalRead(buttonA);  // Active LOW
//   bool buttonBState = !digitalRead(buttonB);  // Active LOW
//   bool switchState = !digitalRead(SwitchPin);  // Active LOW

//   // Debug print
//   Serial.print("Switch state: ");
//   Serial.println(switchState);

//   if (switchState) {
//     Serial.println("Switch pressed: Run Joystick Control");
//     tft.fillScreen(TFT_BLACK);
//     tft.setCursor(10, 10);
//     tft.println("Switch button clicked");
//   }

//   if (buttonAState) {
//     Serial.println("Button A pressed: Save Volume");

//     // Read current X-axis value
//     int currentX = analogRead(xPin);

//     // Map X-axis value to volume (0-100)
//     int volume = map(currentX, 0, 1023, 0, 100);
//     settings.volume = volume;

//     Serial.print("Current X-axis: ");
//     Serial.print(currentX);
//     Serial.print(" -> Volume: ");
//     Serial.println(settings.volume);

//     // Save to EEPROM
//     saveSettings();

//     // Debounce delay
//     delay(500);
//   }

//   if (buttonBState) {
//     Serial.println("Button B pressed: Load Volume");

//     // Load from EEPROM
//     loadSettings();

//     // Apply loaded volume as needed
//     applyVolume(settings.volume);

//     // Debounce delay
//     delay(500);
//   }

//   // Add other loop tasks here if necessary
// }

// // Function to display the menu
// void displayMenu() {
//   tft.fillScreen(TFT_BLACK); // Clear the screen and set it to black
//   tft.setTextColor(TFT_WHITE, TFT_BLACK); // Set text color and background color
//   tft.setTextSize(2); // Set text size

//   // Define menu options
//   String option1 = "Press 0 to Save Volume";
//   String option2 = "Press 14 to Load Volume";

//   int screenWidth = tft.width();
//   int option1Width = tft.textWidth(option1); // Get the width of the option1 text
//   int option2Width = tft.textWidth(option2); // Get the width of the option2 text

//   // Calculate the X positions for both options (centered horizontally)
//   int option1X = (screenWidth - option1Width) / 2;
//   int option2X = (screenWidth - option2Width) / 2;

//   // Set Y positions to center vertically as needed
//   int option1Y = 50;
//   int option2Y = option1Y + 30;

//   // Display the options
//   tft.setCursor(option1X, option1Y);
//   tft.print(option1);

//   tft.setCursor(option2X, option2Y);
//   tft.print(option2);
// }

// // Function to save settings to EEPROM
// void saveSettings() {
//   EEPROM.put(EEPROM_VOLUME_ADDRESS, settings.volume);
//   if (EEPROM.commit()) {
//     Serial.println("Settings saved successfully");
//     tft.fillScreen(TFT_BLACK);
//     tft.setCursor(10, 10);
//     tft.print("Volume Saved!");
//     delay(2000);  // Display message for 2 seconds
//     displayMenu();
//   } else {
//     Serial.println("Failed to save settings");
//   }
// }

// // Function to load settings from EEPROM
// void loadSettings() {
//   EEPROM.get(EEPROM_VOLUME_ADDRESS, settings.volume);

//   // Validate loaded volume (e.g., between 0 and 100)
//   Serial.print("Loaded Volume: ");
//   Serial.println(settings.volume);
//   tft.fillScreen(TFT_BLACK);
//   tft.setCursor(10, 10);
//   tft.print("Volume Loaded!");
//   delay(2000);  // Display message for 2 seconds
//   displayMenu();
// }

// // Function to apply the loaded volume setting
// void applyVolume(int volume) {
//   // Implement how volume affects your game or hardware
//   // For example, adjust servo speed or other parameters based on volume
//   Serial.print("Applying Volume: ");
//   Serial.println(volume);

//   // Example: Map volume to servo movement speed or position
//   // This is a placeholder and should be adjusted to your specific needs
// //   int servoPos = map(volume, 0, 100, 0, 180);
//   int angle = constrain(volume, 0, 180);
//   Serial.println(angle);
//   servoX.write(angle);
// }

// // Initialize joystick readings
// void setupJoystick() {
//     // Initialize all the readings to 0
//     for (int i = 0; i < numReadings; i++) {
//         readingsX[i] = 0;
//     }
// }

// // Function to detect and handle joystick position
// void detectPosition() {
//     static int previousX = 0; // Start relative to neutral at 0

//     int x_position = analogRead(xPin);

//     // Apply smoothing using a moving average for X
//     totalX = totalX - readingsX[readIndex];  // Subtract the last reading from total
//     readingsX[readIndex] = x_position;       // Read the new X value
//     totalX = totalX + readingsX[readIndex];  // Add the new reading to the total
//     averageX = totalX / numReadings;         // Calculate the average X

//     // Advance to the next position in the array
//     readIndex++;
//     if (readIndex >= numReadings) {
//         readIndex = 0;  // Wrap around if we've reached the end of the array
//     }

//     // Apply dead zone logic for X
//     if (abs(averageX - neutralX) < deadZone) {
//         averageX = neutralX;  // Neutral X position
//     }

//     // Calculate the relative position from the neutral point
//     int relativeX = averageX - neutralX;

//     // Display the relative X position
//     tft.fillScreen(TFT_BLACK);
//     tft.setCursor(0, 0);
//     tft.print("X: ");
//     tft.print(relativeX);

//     // Only update the servo if the change exceeds the noise threshold
//     if (abs(relativeX - previousX) > noiseThreshold) {
//         // Map the relative joystick values to servo positions (0-180 degrees)
//         int servoXPos = map(averageX, 0, 1023, 0, 180);  // Map X to 180-degree SG90 range

//         // Move the servo to the mapped position
//         servoX.write(servoXPos);

//         // Update the previous position to the new relative value
//         previousX = relativeX;
//     }

//     delay(50);  // Short delay for smooth servo movement
// }

// // Function to run joystick control
// void runJoystickControl() {
//     pinMode(SwitchPin, INPUT_PULLUP);
//     pinMode(xPin, INPUT);
//     pinMode(yPin, INPUT);  // Keep Y pin for future use

//     // Initialize the servo pin

//     // Initialize smoothing arrays
//     setupJoystick();

//     tft.init();
//     tft.setRotation(1);

//     tft.fillScreen(TFT_BLACK);
//     tft.setTextColor(TFT_WHITE, TFT_BLACK);
//     tft.setTextSize(2);

//     while (true) {
//         detectPosition();

//         // Optionally, display current volume
//         tft.setCursor(0, 20);
//         tft.print("Volume: ");
//         tft.print(settings.volume);

//         // Adjust game behavior based on volume
//         // For example, adjust servo speed or other parameters
//     }
// }
