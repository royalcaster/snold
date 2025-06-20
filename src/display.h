#ifndef DISPLAY_H
#define DISPLAY_H

#include <Adafruit_GFX.h>
#include <Adafruit_SSD1351.h>
#include <SPI.h>
#include "constants.h"

// --- HARDWARE OBJECT ---
extern Adafruit_SSD1351 display;

// --- FUNCTION DECLARATIONS ---
void initializeDisplay();
void displayTest();
void clearDisplay();
void showMessage(const char* message, uint16_t color = WHITE);

#endif // DISPLAY_H 