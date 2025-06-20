#include <Arduino.h>
#include "constants.h"
#include "display.h"
#include "sensor.h"
#include "images.h"
#include "fatpixel_font.h"

// --- SETUP ---
void setup() {
  Serial.begin(115200);
  Serial.println("Initializing devices...");

  // Initialize display
  initializeDisplay();
  
  // Initialize barometer
  initializeBarometer();
  
  Serial.println("All devices initialized successfully!");
  
  // Display "HALLO" centered on screen
  Serial.println("Displaying HALLO...");
  display.fillScreen(BLACK);
  
  // Calculate center position for "HALLO" (5 characters * 16px width = 80px total)
  // Screen width is 128px, so center at (128-80)/2 = 24px from left
  // Screen height is 128px, so center vertically around 56px from top
  drawFatpixelText(&display, "HALLO", 24, 56, CYAN);
}

// --- MAIN LOOP ---
void loop() {
  // Keep the display showing "HALLO"
  delay(1000);
} 