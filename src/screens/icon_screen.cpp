#include "icon_screen.h"
#include "../images.h"
#include "../constants.h"

IconScreen::IconScreen(const uint16_t* iconData, int width, int height, uint16_t color, int x, int y) 
    : iconData(iconData), width(width), height(height), color(color), x(x), y(y) {
}

void IconScreen::render(Adafruit_GFX* display) {
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
    
    // Draw icon using the existing drawBitmap function
    drawBitmap(display, iconData, drawX, drawY, width, height);
}

void IconScreen::setIcon(const uint16_t* newIconData, int newWidth, int newHeight) {
    iconData = newIconData;
    width = newWidth;
    height = newHeight;
    setDirty(true);
}

void IconScreen::setColor(uint16_t newColor) {
    color = newColor;
    setDirty(true);
}

void IconScreen::setPosition(int newX, int newY) {
    x = newX;
    y = newY;
    setDirty(true);
} 