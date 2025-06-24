#ifndef TEXT_SCREEN_H
#define TEXT_SCREEN_H

#include "screen.h"
#include "../constants.h"
#include "../fatpixel_font.h"

class TextScreen : public Screen {
public:
    TextScreen(const char* text, uint16_t color = CYAN, int x = -1, int y = -1);
    
    void render(Adafruit_GFX* display) override;
    const char* getName() const override { return "text"; }
    
    // Update text content
    void setText(const char* newText);
    void setColor(uint16_t newColor);
    void setPosition(int x, int y);

private:
    const char* text;
    uint16_t color;
    int x, y; // -1 means center
};

#endif // TEXT_SCREEN_H 