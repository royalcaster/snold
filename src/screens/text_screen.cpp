#include "text_screen.h"
#include "../fatpixel_font.h"
#include <Adafruit_GFX.h>

namespace {
    // Get character index in font array
    int getCharIndex(char c) {
        if (c >= 'A' && c <= 'Z') {
            return c - 'A';
        }
        if (c >= 'a' && c <= 'z') {
            return c - 'a' + 26;
        }
        if (c >= '0' && c <= '9') {
            return c - '0' + 52;
        }
        
        const char special_chars[] = "!@#$%^&*()_+-=[]{}|;:,.<>? ";
        for (int i = 0; i < sizeof(special_chars) - 1; i++) {
            if (c == special_chars[i]) {
                return 62 + i;
            }
        }

        return -1;
    }

}

TextScreen::TextScreen(const char* text, uint16_t color, int x, int y)
    : text(text), color(color), x(x), y(y) {}

void TextScreen::render(Adafruit_GFX* display) {
    int16_t text_x, text_y;
    uint16_t textW, textH;
    getFatpixelTextBounds(text, 0, 0, &text_x, &text_y, &textW, &textH);
    
    int finalX = (x == -1) ? (display->width() - textW) / 2 : x;
    int finalY = (y == -1) ? (display->height() - textH) / 2 : y;

    drawFatpixelText(display, text, finalX, finalY, color);
}

void TextScreen::setText(const char* newText) {
    text = newText;
    setDirty(true);
}

void TextScreen::setColor(uint16_t newColor) {
    color = newColor;
    setDirty(true);
}

void TextScreen::setPosition(int newX, int newY) {
    x = newX;
    y = newY;
    setDirty(true);
} 