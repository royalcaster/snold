#include <Arduino.h>
#include "constants.h"
#include "display.h"
#include "sensor.h"
#include "images.h"
#include "fatpixel_font.h"

// Screen management system
#include "screens/screen_manager.h"
#include "screens/animated_image_screen.h"

// Include all RGB565 image headers
#include "../include/img_welcome_a_rgb565.h"
#include "../include/img_welcome_b_rgb565.h"
#include "../include/img_play_a_rgb565.h"
#include "../include/img_play_b_rgb565.h"
#include "../include/img_walk_a_rgb565.h"
#include "../include/img_walk_b_rgb565.h"
#include "../include/img_throw_a_rgb565.h"
#include "../include/img_throw_b_rgb565.h"
#include "../include/img_rocket_a_rgb565.h"
#include "../include/img_rocket_b_rgb565.h"
#include "../include/img_shake_a_rgb565.h"
#include "../include/img_shake_b_rgb565.h"

// Global screen manager
ScreenManager* screenManager = nullptr;

// Button configuration
#define BUTTON_PIN 26
unsigned long lastDebounceTime = 0;
unsigned long debounceDelay = 50;
int lastButtonState = HIGH;

#define THROW_UP_THRESHOLD 2.0  // m/s, adjust as needed
#define THROW_COOLDOWN_MS 1000  // 1 second cooldown between throws

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
  
  // Add animated screens for each image pair (all images are 128x128)
  // Using the correct RGB565 arrays and variable names
  screenManager->addScreen("welcome", new AnimatedImageScreen("welcome", welcome_A, welcome_B, 128, 128));
  screenManager->addScreen("play", new AnimatedImageScreen("play", play_A, play_B, 128, 128));
  screenManager->addScreen("walk", new AnimatedImageScreen("walk", walk_A, walk_B, 128, 128));
  screenManager->addScreen("throw", new AnimatedImageScreen("throw", throw_A, throw_B, 128, 128));
  screenManager->addScreen("rocket", new AnimatedImageScreen("rocket", rocket_A, rocket_B, 128, 128));
  screenManager->addScreen("shake", new AnimatedImageScreen("shake", shake_A, shake_B, 128, 128));
  
  // Show welcome screen immediately for testing
  screenManager->showScreen("welcome");
  
  Serial.println("Animated image screens initialized with RGB565 format!");
}

// --- MAIN LOOP ---
void loop() {
  // --- Barometer reading and deviation calculation ---
  static unsigned long lastBaroReadTime = 0;
  static float lastAltitude = 0.0;
  static bool isFirstRead = true;
  const unsigned long baroReadInterval = 1000 / BAROMETER_READ_RATE_HZ;
  static unsigned long lastThrowTime = 0;

  if (millis() - lastBaroReadTime >= baroReadInterval) {
    float currentAltitude;
    if (readAltitude(currentAltitude)) {
      if (isFirstRead) {
        lastAltitude = currentAltitude;
        isFirstRead = false;
      } else {
        float timeDeltaSeconds = (millis() - lastBaroReadTime) / 1000.0f;
        if (timeDeltaSeconds > 0) {
          float altitudeDelta = currentAltitude - lastAltitude;
          float deviation = altitudeDelta / timeDeltaSeconds;
          // Print deviation with + or - sign, always align decimal point
          char sign = (deviation >= 0) ? '+' : '-';
          float absDeviation = fabs(deviation);
          Serial.print("Altitude Deviation: ");
          Serial.print(sign);
          Serial.print(absDeviation, 2);
          Serial.println(" m/s");

          // Throw detection logic
          if (deviation > THROW_UP_THRESHOLD && (millis() - lastThrowTime > THROW_COOLDOWN_MS)) {
            Serial.println("Throw detected!");
            lastThrowTime = millis();
            // TODO: Trigger your throw event/animation here
          }
        }
      }
      lastAltitude = currentAltitude;
    }
    lastBaroReadTime = millis();
  }

  // --- Button handling ---
  int reading = digitalRead(BUTTON_PIN);

  if (reading != lastButtonState) {
    lastDebounceTime = millis();
  }

  if ((millis() - lastDebounceTime) > debounceDelay) {
    if (reading != lastButtonState) {
      lastButtonState = reading;
      if (lastButtonState == LOW) {
        Serial.println("Button pressed!");
      }
    }
  }
  
  // --- Screen updates and cycling ---
  if (screenManager) {
    screenManager->update();
  }
  
  static unsigned long lastSwitch = 0;
  static int currentScreenIndex = 0;
  const char* screens[] = {"welcome", "play", "walk", "throw", "rocket", "shake"};
  const int numScreens = sizeof(screens) / sizeof(screens[0]);
  
  if (millis() - lastSwitch > 5000) {
    currentScreenIndex = (currentScreenIndex + 1) % numScreens;
    // Serial.printf("Switching to screen %d: %s\n", currentScreenIndex, screens[currentScreenIndex]);
    screenManager->showScreen(screens[currentScreenIndex]);
    lastSwitch = millis();
  }
  
  delay(10);
} 