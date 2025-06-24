#ifndef DISPLAY_H
#define DISPLAY_H

#include <Adafruit_GFX.h>
#include <Adafruit_SSD1351.h>
#include <SPI.h>
#include "constants.h"
#include "fonts.h"
#include "fatpixel_font.h"

// --- HARDWARE OBJECT ---
extern Adafruit_SSD1351 display;

// --- FUNCTION DECLARATIONS ---
void initializeDisplay();
void initializeDisplayCustomSpeed(uint32_t spiSpeed = 20000000);
void benchmarkDisplay();
void displayTest();
void clearDisplay();
void showMessage(const char* message, uint16_t color = WHITE);

// --- CUSTOM FONT FUNCTIONS ---
void showCustomText(const char* text, int x, int y, uint16_t color = WHITE, uint8_t fontSize = FONT_SIZE_8);
void showCustomMessage(const char* message, uint16_t color = WHITE, uint8_t fontSize = FONT_SIZE_8);
void displayFontDemo();

#endif // DISPLAY_H 