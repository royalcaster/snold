#include "animated_image_screen.h"
#include <Adafruit_GFX.h>
#include <Arduino.h>
#include "../images.h"  // For the custom drawBitmap function

AnimatedImageScreen::AnimatedImageScreen(const char* name, const uint16_t* imageA, const uint16_t* imageB, int width, int height)
    : name(name), imageA(imageA), imageB(imageB), width(width), height(height), lastToggle(0), showingA(true) {}

void AnimatedImageScreen::render(Adafruit_GFX* display) {
    int x = (display->width() - width) / 2;
    int y = (display->height() - height) / 2;
    
    const uint16_t* currentImage = showingA ? imageA : imageB;
    
    drawBitmapHybridInterlaced(display, currentImage, x, y, width, height);
}

void AnimatedImageScreen::update() {
    Screen::update();
    
    // Check if it's time to toggle the image
    unsigned long currentTime = millis();
    if (currentTime - lastToggle >= TOGGLE_INTERVAL) {
        showingA = !showingA;
        lastToggle = currentTime;
        setDirty(true);
    }
}

const char* AnimatedImageScreen::getName() const {
    return name;
} 