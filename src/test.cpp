// #include "ESP32S3VGA.h"
// #include <GfxWrapper.h>

// // VGA pin configuration
// const PinConfig pins(-1,-1,-1,-1,43, -1,-1,-1,-1,-1,44, -1,-1,-1,-1,18, 1,2);
// VGA vga;
// GfxWrapper<VGA>* gfx = nullptr;

// // Use a more standard VGA mode
// Mode mode = Mode::MODE_640x480x60;

// // Colors
// const uint16_t colors[] = {
//     0xF800,  // Red
//     0x07E0,  // Green
//     0x001F,  // Blue
//     0xFFE0,  // Yellow
//     0xF81F,  // Magenta
//     0x07FF   // Cyan
// };
// const int numColors = sizeof(colors) / sizeof(colors[0]);

// void drawHelloWorld(uint16_t color) {
//   // Clear the screen with a black background
//   vga.clear(vga.rgb(0, 0, 0));

//   // Set text size and color
//   gfx->setTextSize(4);
//   gfx->setTextColor(color);

//   // Calculate position to center the text
//   int16_t x1, y1;
//   uint16_t w, h;
//   gfx->getTextBounds("Hello World", 0, 0, &x1, &y1, &w, &h);
//   int x = (gfx->width() - w) / 2;
//   int y = (gfx->height() - h) / 2;

//   // Draw "Hello World" text
//   gfx->setCursor(x, y);
//   gfx->print("Hello World");

//   // Show the frame
//   vga.show();
// }

// void setup() {
//   Serial.begin(115200);
//   delay(1000);
//   Serial.println("VGA Test Starting...");

//   vga.bufferCount = 2;
//   if (!vga.init(pins, mode, 16)) {
//     Serial.println("VGA initialization failed!");
//     while(1) delay(1000);
//   }

//   Serial.println("VGA initialized successfully");
//   gfx = new GfxWrapper<VGA>(vga, mode.hRes, mode.vRes);

//   vga.start();
//   Serial.println("VGA started");

//   drawHelloWorld(colors[0]);  // Initial draw with the first color
// }

// void loop() {
//   static int colorIndex = 0;
//   static unsigned long lastUpdate = 0;
//   const unsigned long updateInterval = 5000;  // Update every 5 seconds

//   unsigned long currentTime = millis();
//   if (currentTime - lastUpdate >= updateInterval) {
//     lastUpdate = currentTime;

//     // Move to next color
//     colorIndex = (colorIndex + 1) % numColors;

//     // Redraw with new color
//     drawHelloWorld(colors[colorIndex]);
//   }

//   // No delay here, we're using millis() for timing
// }
