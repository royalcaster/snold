#include "animated_image_screen.h"
#include <Adafruit_GFX.h>
#include <Arduino.h>
#include "../images.h"  // For the custom drawBitmap function

AnimatedImageScreen::AnimatedImageScreen(const char* name, const uint16_t* imageA, const uint16_t* imageB, int width, int height)
    : name(name), imageA(imageA), imageB(imageB), width(width), height(height), lastToggle(0), showingA(true) {}

void AnimatedImageScreen::render(Adafruit_GFX* display) {
    // Clear the entire display first to prevent overlaying
    display->fillScreen(BLACK);
    
    // Center the image on the display
    int x = (display->width() - width) / 2;
    int y = (display->height() - height) / 2;
    
    // Choose which image to display
    const uint16_t* currentImage = showingA ? imageA : imageB;
    
    // Use the custom drawBitmap function for RGB565 images
    // This is the KEY FIX - using the correct function from images.cpp
    drawBitmap(display, currentImage, x, y, width, height);
}

void AnimatedImageScreen::update() {
    Screen::update(); // Call parent update
    
    // Check if it's time to toggle the image
    unsigned long currentTime = millis();
    if (currentTime - lastToggle >= TOGGLE_INTERVAL) {
        showingA = !showingA;
        lastToggle = currentTime;
        setDirty(true); // Mark for redraw - this ensures continuous animation
        
        // Debug output to monitor the animation
        Serial.printf("AnimatedImageScreen '%s': Showing image %c\n", name, showingA ? 'A' : 'B');
    }
}

const char* AnimatedImageScreen::getName() const {
    return name;
} 