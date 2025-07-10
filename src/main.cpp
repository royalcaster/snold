#include <Arduino.h>
#include "constants.h"
#include "display.h"
#include "sensor.h"
#include "images.h"
#include "fatpixel_font.h"

// Screen management system
#include "screens/screen_manager.h"
#include "screens/animated_image_screen.h"
#include "screens/result_screen.h"

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
ResultScreen* resultScreen = nullptr;

// Button configuration
#define BUTTON_PIN 26
unsigned long lastDebounceTime = 0;
unsigned long debounceDelay = 50;
int lastButtonState = HIGH;

#define THROW_UP_THRESHOLD 2  // m/s, adjust as needed (made more generous)
#define THROW_COOLDOWN_MS 1000  // 1 second cooldown between throws

// Screen cycling system - global variables
static unsigned long lastScreenSwitchTime = 0;
static int currentScreenIndex = 0;
const unsigned long SCREEN_DISPLAY_DURATION = 5000; // 5 seconds per screen
const char* cyclableScreens[] = {"welcome", "play", "walk", "throw", "rocket", "shake"};
const int numCyclableScreens = sizeof(cyclableScreens) / sizeof(cyclableScreens[0]);

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
  // Using the updated RGB565 arrays from img_new
  screenManager->addScreen("welcome", new AnimatedImageScreen("welcome", welcome_A, welcome_B, 128, 128));
  screenManager->addScreen("play", new AnimatedImageScreen("play", play_A, play_B, 128, 128));
  screenManager->addScreen("walk", new AnimatedImageScreen("walk", walk_A, walk_B, 128, 128));
  screenManager->addScreen("throw", new AnimatedImageScreen("throw", throw_A, throw_B, 128, 128));
  screenManager->addScreen("rocket", new AnimatedImageScreen("rocket", rocket_A, rocket_B, 128, 128));
  screenManager->addScreen("shake", new AnimatedImageScreen("shake", shake_A, shake_B, 128, 128));
  
  // Add the result screen
  resultScreen = new ResultScreen("result", "0.00 m");
  screenManager->addScreen("result", resultScreen);

  // Initialize screen cycling - start with the first screen
  currentScreenIndex = 0;
  lastScreenSwitchTime = millis();
  screenManager->showScreen(cyclableScreens[currentScreenIndex]);
  
  Serial.printf("Animated image screens initialized with RGB565 format! Starting cycling with: %s\n", cyclableScreens[currentScreenIndex]);
}

// --- MAIN LOOP ---

// States for throw detection
enum ThrowState { IDLE, THROW_ACTIVE, THROW_ENDED };
static ThrowState throwState = IDLE;
static float startAltitude = 0.0;
static float maxAltitude = 0.0;
static int descentCounter = 0;
const int DESCENT_SAMPLES_TO_END_THROW = 3; // Number of descending samples to confirm throw has ended

// Result screen timing
static unsigned long resultScreenStartTime = 0;
const unsigned long RESULT_DISPLAY_DURATION = 7000; // 7 seconds in milliseconds (definitely long enough!)

void loop() {
  // --- Barometer reading and state machine for throw detection ---
  static unsigned long lastBaroReadTime = 0;
  static float lastAltitude = 0.0;
  static bool isFirstRead = true;
  const unsigned long baroReadInterval = 1000 / BAROMETER_READ_RATE_HZ;

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
          float verticalSpeed = altitudeDelta / timeDeltaSeconds; // This is the deviation in m/s

          switch (throwState) {
            case IDLE: {
              const char* currentScreen = screenManager->getCurrentScreenName();
              
              // Check if result screen timer has expired first
              if (currentScreen != nullptr &&
                  strcmp(currentScreen, "result") == 0 &&
                  resultScreenStartTime > 0 &&
                  millis() - resultScreenStartTime >= RESULT_DISPLAY_DURATION) {
                // Timer expired - start cycling from beginning and reset timer
                resultScreenStartTime = 0;
                currentScreenIndex = 0;
                lastScreenSwitchTime = millis();
                screenManager->showScreen(cyclableScreens[currentScreenIndex]);
                Serial.printf("IDLE: Result display timeout - starting screen cycling with %s\n", cyclableScreens[currentScreenIndex]);
              }
              // If result screen is showing and timer hasn't expired, do nothing (don't cycle)
              else if (currentScreen != nullptr && 
                       strcmp(currentScreen, "result") == 0 && 
                       resultScreenStartTime > 0) {
                // Do nothing - let result screen continue displaying
                Serial.printf("IDLE: Result screen still showing (%lu ms remaining)\n", 
                              RESULT_DISPLAY_DURATION - (millis() - resultScreenStartTime));
              }
              // If not showing result screen, handle cycling
              else {
                // Check if it's time to switch to the next screen
                if (millis() - lastScreenSwitchTime >= SCREEN_DISPLAY_DURATION) {
                  currentScreenIndex = (currentScreenIndex + 1) % numCyclableScreens;
                  lastScreenSwitchTime = millis();
                  screenManager->showScreen(cyclableScreens[currentScreenIndex]);
                  Serial.printf("IDLE: Cycling to screen %s (%d/%d)\n", cyclableScreens[currentScreenIndex], currentScreenIndex + 1, numCyclableScreens);
                }
                // If this is the first run or screen manager is out of sync, initialize cycling
                else if (currentScreen == nullptr || 
                         (strcmp(currentScreen, "result") != 0 && 
                          strcmp(currentScreen, cyclableScreens[currentScreenIndex]) != 0)) {
                  lastScreenSwitchTime = millis();
                  screenManager->showScreen(cyclableScreens[currentScreenIndex]);
                  Serial.printf("IDLE: Initializing screen cycling with %s\n", cyclableScreens[currentScreenIndex]);
                }
              }
              
              // Check for new throw regardless of which screen is showing (but not during result display)
              if (currentScreen == nullptr || strcmp(currentScreen, "result") != 0) {
                if (verticalSpeed > THROW_UP_THRESHOLD) {
                  throwState = THROW_ACTIVE;
                  startAltitude = currentAltitude;
                  maxAltitude = currentAltitude;
                  descentCounter = 0;
                  Serial.printf("THROW STATE: ACTIVE (detected on screen: %s)\n", currentScreen ? currentScreen : "unknown");
                }
              }
              break;
            }

            case THROW_ACTIVE:
              if (currentAltitude > maxAltitude) {
                maxAltitude = currentAltitude;
                descentCounter = 0; // Reset counter if we're still going up
              } else {
                descentCounter++; // Increment counter if we're not going up
              }

              // If we've had a few samples not increasing, the throw is over
              if (descentCounter >= DESCENT_SAMPLES_TO_END_THROW) {
                throwState = THROW_ENDED;
                Serial.println("THROW STATE: ENDED");
              }
              break;

            case THROW_ENDED:
              // 1. Calculate height
              float throwHeight = maxAltitude - startAltitude;
              if (throwHeight < 0) {
                throwHeight = 0;
              }
              Serial.printf("Final throw height: %.2f meters\n", throwHeight);

              // 2. Update and show the result screen
              char resultText[32];
              int throwHeightCm = (int)(throwHeight * 100.0f + 0.5f); // Convert to cm and round
              
              if (throwHeightCm >= 100) {
                // Show in meters for throws >= 100cm
                int throwHeightM = (int)(throwHeight + 0.5f); // Round to nearest meter
                snprintf(resultText, sizeof(resultText), "%d m", throwHeightM);
              } else {
                // Show in centimeters for throws < 100cm
                snprintf(resultText, sizeof(resultText), "%d cm", throwHeightCm);
              }
              resultScreen->setText(resultText);
              screenManager->showScreen("result");
              
              // 3. Start timer for result display
              resultScreenStartTime = millis();
              Serial.println("Started 7-second result display timer");

              // 4. Go back to IDLE to be ready for the next throw
              throwState = IDLE;
              Serial.println("THROW STATE: IDLE");
              break;
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
  
  // --- Result screen timer check is now handled in the IDLE state above ---

  // --- Screen updates ---
  if (screenManager) {
    screenManager->update();
  }
  
  delay(10);
} 