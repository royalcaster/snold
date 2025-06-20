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
}

// --- MAIN LOOP ---
void loop() {
  // Example: Read sensor data and display it
  float altitude, temperature, pressure;
  
  if (readAltitude(altitude)) {
    Serial.printf("Altitude: %.2f m\n", altitude);
    showMessage("Altitude OK", GREEN);
  } else {
    showMessage("Sensor Error", RED);
  }
  
  delay(2000);
} 