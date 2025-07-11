#include "result_screen.h"
#include "../constants.h"
#include <string.h>

ResultScreen::ResultScreen(const char* name, const char* initialText) : screenName(name) {
    strncpy(this->text, initialText, sizeof(this->text) - 1);
    this->text[sizeof(this->text) - 1] = '\0';
}

void ResultScreen::setText(const char* newText) {
    strncpy(this->text, newText, sizeof(this->text) - 1);
    this->text[sizeof(this->text) - 1] = '\0';
    setDirty(true);
}

const char* ResultScreen::getName() const {
    return screenName;
}

void ResultScreen::render(Adafruit_GFX* display) {
    display->fillScreen(BLACK);
    
    display->setTextSize(3);
    display->setTextColor(WHITE);
    
    int16_t x1, y1;
    uint16_t text_w, text_h;
    display->getTextBounds(this->text, 0, 0, &x1, &y1, &text_w, &text_h);
    
    int16_t x = (display->width() - text_w) / 2;
    int16_t y = (display->height() / 2) - (text_h / 2) + text_h;
    
    display->setCursor(x, y);
    display->print(this->text);
} 