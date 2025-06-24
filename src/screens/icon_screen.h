#ifndef ICON_SCREEN_H
#define ICON_SCREEN_H

#include "screen.h"
#include "../constants.h"

class IconScreen : public Screen {
public:
    IconScreen(const uint16_t* iconData, int width, int height, uint16_t color = WHITE, int x = -1, int y = -1);
    
    void render(Adafruit_GFX* display) override;
    const char* getName() const override { return "icon"; }
    
    // Update icon
    void setIcon(const uint16_t* newIconData, int newWidth, int newHeight);
    void setColor(uint16_t newColor);
    void setPosition(int x, int y);

private:
    const uint16_t* iconData;
    int width, height;
    uint16_t color;
    int x, y; // -1 means center
};

#endif // ICON_SCREEN_H 