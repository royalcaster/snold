#ifndef IMAGES_H
#define IMAGES_H

#include <Adafruit_GFX.h>
#include "constants.h"

// --- IMAGE FUNCTION DECLARATIONS ---
void drawBitmap(Adafruit_GFX* display, const uint16_t* bitmap, int x, int y, int width, int height);
void drawBitmapFromArray(Adafruit_GFX* display, const uint16_t* bitmap, int x, int y, int width, int height);
void showImage(const uint16_t* bitmap, int width, int height, int x = 0, int y = 0);
void showCenteredImage(const uint16_t* bitmap, int width, int height);

// --- EXAMPLE IMAGE DECLARATIONS ---
// You can add your image arrays here
extern const uint16_t exampleImage[]; // Example 32x32 image

#endif // IMAGES_H 