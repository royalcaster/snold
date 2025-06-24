#include <Arduino.h>
#include "constants.h"
#include "display.h"
#include "sensor.h"
#include "images.h"
#include "fatpixel_font.h"

// Screen management system
#include "screens/screen_manager.h"
#include "screens/welcome_screen.h"
#include "screens/text_screen.h"
#include "screens/image_screen.h"
#include "screens/icon_screen.h"
#include "screens/text_icon_screen.h"

// Global screen manager
ScreenManager* screenManager = nullptr;

// --- SETUP ---
void setup() {
  Serial.begin(115200);
  Serial.println("Initializing devices...");

  // Initialize display
  initializeDisplay();
  
  // Initialize barometer
  initializeBarometer();
  
  Serial.println("All devices initialized successfully!");
  
  // Initialize screen manager
  screenManager = new ScreenManager(&display);
  
  // Add screens to the manager
  screenManager->addScreen("welcome", new WelcomeScreen());
  screenManager->addScreen("text1", new TextScreen("HELLO WORLD", GREEN));
  screenManager->addScreen("text2", new TextScreen("ESP32 OLED", YELLOW));
  
  // Show welcome screen immediately (no transition for first screen)
  screenManager->showScreen("welcome");
  
  Serial.println("Screen management system initialized!");
}

// --- MAIN LOOP ---
void loop() {
  // Update screen manager (handles transitions and screen updates)
  if (screenManager) {
    screenManager->update();
  }
  
  // Simple demo: cycle through screens every 5 seconds
  static unsigned long lastSwitch = 0;
  static int currentScreenIndex = 0;
  const char* screens[] = {"welcome", "text1", "text2"};
  
  if (millis() - lastSwitch > 5000) {
    currentScreenIndex = (currentScreenIndex + 1) % 3;
    
    Serial.printf("Switching to screen %d: %s\n", currentScreenIndex, screens[currentScreenIndex]);
    
    screenManager->showScreen(screens[currentScreenIndex]);
    
    lastSwitch = millis();
  }
  
  delay(100); // Slightly longer delay
} 