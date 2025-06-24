#ifndef IMAGES_H
#define IMAGES_H

#include <Adafruit_GFX.h>
#include "constants.h"
#include <stdint.h>

// --- IMAGE FUNCTION DECLARATIONS ---

// Standard drawing functions
void drawBitmap(Adafruit_GFX* display, const uint16_t* bitmap, int x, int y, int width, int height);
void drawBitmapFast(Adafruit_GFX* display, const uint16_t* bitmap, int x, int y, int width, int height);
void drawBitmapFromArray(Adafruit_GFX* display, const uint16_t* bitmap, int x, int y, int width, int height);

// Interlaced rendering methods
void drawBitmapInterlaced(Adafruit_GFX* display, const uint16_t* bitmap, int x, int y, int width, int height);
void drawBitmapHybridInterlaced(Adafruit_GFX* display, const uint16_t* bitmap, int x, int y, int width, int height);
void drawBitmapInterlacedTimed(Adafruit_GFX* display, const uint16_t* bitmap, int x, int y, int width, int height, uint8_t delayMs = 0);

// Ultra-optimized performance methods
inline void drawBitmapUltraFast(Adafruit_GFX* display, const uint16_t* bitmap, int x, int y, int width, int height);
void drawBitmapChunked(Adafruit_GFX* display, const uint16_t* bitmap, int x, int y, int width, int height);

// Display convenience functions
void showImage(const uint16_t* bitmap, int width, int height, int x, int y);
void showCenteredImage(const uint16_t* bitmap, int width, int height);

// --- EXAMPLE IMAGE DECLARATIONS ---
// You can add your image arrays here
extern const uint16_t exampleImage[]; // Example 32x32 image

// DMA and advanced optimization functions
void drawBitmapDMA(const uint16_t* bitmap, int x, int y, int width, int height);
void drawBitmapMultiCore(Adafruit_GFX* display, const uint16_t* bitmap, int x, int y, int width, int height);
void drawBitmapDirectMemory(const uint16_t* bitmap, int x, int y, int width, int height);
void drawBitmapAdaptive(Adafruit_GFX* display, const uint16_t* bitmap, int x, int y, int width, int height);

#endif // IMAGES_H 