#include "welcome_screen.h"
#include "../fatpixel_font.h"
#include "../constants.h"

WelcomeScreen::WelcomeScreen() {
    // Constructor - nothing special needed
}

void WelcomeScreen::render(Adafruit_GFX* display) {
    // Clear screen
    display->fillScreen(BLACK);
    
    // Calculate center position for "HALLO" (5 characters * 16px width = 80px total)
    // Screen width is 128px, so center at (128-80)/2 = 24px from left
    // Screen height is 128px, so center vertically around 56px from top
    drawFatpixelText(display, message, 24, 56, CYAN);
} 