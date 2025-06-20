#include <Arduino.h>
#include "constants.h"
#include "display.h"
#include "sensor.h"

// --- SETUP ---
void setup() {
  Serial.begin(115200);
  Serial.println("Initializing devices...");

  // Initialize display
  initializeDisplay();
  
  // Initialize barometer
  initializeBarometer();
  
  Serial.println("All devices initialized successfully!");
  
  // Test with built-in font first
  Serial.println("Testing with built-in font...");
  display.fillScreen(BLACK);
  display.setCursor(0, 0);
  display.setTextSize(2);
  display.setTextColor(WHITE);
  display.println("HALLO");
  Serial.println("Built-in font test complete");
  
  delay(2000);
  
  // Now try custom font (using 8x8 since 16x16 only has numbers)
  Serial.println("Attempting to show HALLO message with custom font...");
  showCustomMessage("HALLO", WHITE, FONT_SIZE_8);
  Serial.println("HALLO message should be displayed now");
}

// --- MAIN LOOP ---
void loop() {
  // Just keep the display showing "Hallo"
  delay(1000);
} 