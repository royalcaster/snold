#include "welcome_screen.h"
#include "../fatpixel_font.h"
#include "../constants.h"

WelcomeScreen::WelcomeScreen() {
}

void WelcomeScreen::render(Adafruit_GFX* display) {
    display->fillScreen(BLACK);
    
    drawFatpixelText(display, message, 24, 56, CYAN);
} 