#ifndef FONTS_H
#define FONTS_H

#include <Adafruit_GFX.h>
#include "constants.h"

// --- FONT DEFINITIONS ---

// Custom 8x8 pixel font (simplified)
extern const uint8_t customFont8x8[][8];

// Custom 16x16 pixel font (more detailed)
extern const uint16_t customFont16x16[][16];

// --- FONT FUNCTIONS ---
void setCustomFont8x8(Adafruit_GFX* display);
void setCustomFont16x16(Adafruit_GFX* display);
void drawCustomText(Adafruit_GFX* display, const char* text, int x, int y, uint16_t color, uint8_t fontSize = 8);
void drawCustomChar(Adafruit_GFX* display, char c, int x, int y, uint16_t color, uint8_t fontSize = 8);

// --- FONT SIZES ---
#define FONT_SIZE_8 8
#define FONT_SIZE_16 16

#endif // FONTS_H 