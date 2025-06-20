#include <Arduino.h>
#include "constants.h"
#include "display.h"
#include "sensor.h"
#include "images.h"

// --- SETUP ---
void setup() {
  Serial.begin(115200);
  Serial.println("Initializing devices...");

  // Initialize display
  initializeDisplay();
  
  // Initialize barometer
  initializeBarometer();
  
  Serial.println("All devices initialized successfully!");
  
  // Show "Hallo" message
  Serial.println("Showing HALLO message...");
  showCustomMessage("HALLO", WHITE, FONT_SIZE_8);
}

// --- MAIN LOOP ---
void loop() {
  // Just keep the display showing "Hallo"
  delay(1000);
} 