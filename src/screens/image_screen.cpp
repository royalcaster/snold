#include "image_screen.h"
#include "../images.h"
#include "../constants.h"

ImageScreen::ImageScreen(const uint16_t* imageData, int width, int height, int x, int y) 
    : imageData(imageData), width(width), height(height), x(x), y(y) {
}

void ImageScreen::render(Adafruit_GFX* display) {
    // Clear screen
    display->fillScreen(BLACK);
    
    // Calculate position
    int drawX = x;
    int drawY = y;
    
    if (drawX == -1) {
        // Center horizontally
        drawX = (128 - width) / 2;
    }
    
    if (drawY == -1) {
        // Center vertically
        drawY = (128 - height) / 2;
    }
    
    // Draw image using the existing drawBitmap function
    drawBitmap(display, imageData, drawX, drawY, width, height);
}

void ImageScreen::setImage(const uint16_t* newImageData, int newWidth, int newHeight) {
    imageData = newImageData;
    width = newWidth;
    height = newHeight;
    setDirty(true);
}

void ImageScreen::setPosition(int newX, int newY) {
    x = newX;
    y = newY;
    setDirty(true);
} 