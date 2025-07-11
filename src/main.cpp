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

#define THROW_UP_THRESHOLD 2
#define THROW_COOLDOWN_MS 1000

// Screen cycling system - global variables
static unsigned long lastScreenSwitchTime = 0;
static int currentScreenIndex = 0;
const unsigned long SCREEN_DISPLAY_DURATION = 5000;
const char* cyclableScreens[] = {"welcome", "play", "walk", "throw", "rocket", "shake"};
const int numCyclableScreens = sizeof(cyclableScreens) / sizeof(cyclableScreens[0]);


void setup() {
  Serial.begin(115200);

  initializeDisplay();
  initializeBarometer();
  
  screenManager = new ScreenManager(&display);
  
  // Add animated screens
  screenManager->addScreen("welcome", new AnimatedImageScreen("welcome", welcome_a, welcome_b, 128, 128));
  screenManager->addScreen("play", new AnimatedImageScreen("play", play_a, play_b, 128, 128));
  screenManager->addScreen("walk", new AnimatedImageScreen("walk", walk_a, walk_b, 128, 128));
  screenManager->addScreen("throw", new AnimatedImageScreen("throw", throw_a, throw_b, 128, 128));
  screenManager->addScreen("rocket", new AnimatedImageScreen("rocket", rocket_a, rocket_b, 128, 128));
  screenManager->addScreen("shake", new AnimatedImageScreen("shake", shake_a, shake_b, 128, 128));
  
  // Add result screen
  resultScreen = new ResultScreen("result", "0.00 m");
  screenManager->addScreen("result", resultScreen);

  // Start screen cycling
  currentScreenIndex = 0;
  lastScreenSwitchTime = millis();
  screenManager->showScreen(cyclableScreens[currentScreenIndex]);
}

// Throw detection states
enum ThrowState { IDLE, THROW_ACTIVE, THROW_ENDED };
static ThrowState throwState = IDLE;
static float startAltitude = 0.0;
static float maxAltitude = 0.0;
static int descentCounter = 0;
const int DESCENT_SAMPLES_TO_END_THROW = 3;

// Result screen timing
static unsigned long resultScreenStartTime = 0;
const unsigned long RESULT_DISPLAY_DURATION = 7000;

void loop() {
  // Barometer reading and throw detection
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
          float verticalSpeed = altitudeDelta / timeDeltaSeconds;

          switch (throwState) {
            case IDLE: {
              const char* currentScreen = screenManager->getCurrentScreenName();
              
              // Check if result screen timer has expired first
              if (currentScreen != nullptr &&
                  strcmp(currentScreen, "result") == 0 &&
                  resultScreenStartTime > 0 &&
                  millis() - resultScreenStartTime >= RESULT_DISPLAY_DURATION) {

                resultScreenStartTime = 0;
                currentScreenIndex = 0;
                lastScreenSwitchTime = millis();
                screenManager->showScreen(cyclableScreens[currentScreenIndex]);

              }

              else if (currentScreen != nullptr && 
                       strcmp(currentScreen, "result") == 0 && 
                       resultScreenStartTime > 0) {
                // Result screen still showing
              }

              else {

                if (millis() - lastScreenSwitchTime >= SCREEN_DISPLAY_DURATION) {
                  currentScreenIndex = (currentScreenIndex + 1) % numCyclableScreens;
                  lastScreenSwitchTime = millis();
                  screenManager->showScreen(cyclableScreens[currentScreenIndex]);

                }

                else if (currentScreen == nullptr || 
                         (strcmp(currentScreen, "result") != 0 && 
                          strcmp(currentScreen, cyclableScreens[currentScreenIndex]) != 0)) {
                  lastScreenSwitchTime = millis();
                  screenManager->showScreen(cyclableScreens[currentScreenIndex]);

                }
              }
              
              // Check for new throw
              if (currentScreen == nullptr || strcmp(currentScreen, "result") != 0) {
                if (verticalSpeed > THROW_UP_THRESHOLD) {
                  throwState = THROW_ACTIVE;
                  startAltitude = currentAltitude;
                  maxAltitude = currentAltitude;
                  descentCounter = 0;

                }
              }
              break;
            }

            case THROW_ACTIVE:
              if (currentAltitude > maxAltitude) {
                maxAltitude = currentAltitude;
                descentCounter = 0;
              } else {
                descentCounter++;
              }


              if (descentCounter >= DESCENT_SAMPLES_TO_END_THROW) {
                throwState = THROW_ENDED;

              }
              break;

            case THROW_ENDED:
              float throwHeight = maxAltitude - startAltitude;
              if (throwHeight < 0) {
                throwHeight = 0;
              }

              char resultText[32];
              int throwHeightCm = (int)(throwHeight * 100.0f + 0.5f);
              
              if (throwHeightCm >= 100) {
                int throwHeightM = (int)(throwHeight + 0.5f);
                snprintf(resultText, sizeof(resultText), "%d m", throwHeightM);
              } else {
                snprintf(resultText, sizeof(resultText), "%d cm", throwHeightCm);
              }
              resultScreen->setText(resultText);
              screenManager->showScreen("result");
              
              resultScreenStartTime = millis();
              throwState = IDLE;
              break;
          }
        }
      }
      lastAltitude = currentAltitude;
    }
    lastBaroReadTime = millis();
  }

  // Button handling
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
  
  // Screen updates
  if (screenManager) {
    screenManager->update();
  }
  
  delay(10);
} 