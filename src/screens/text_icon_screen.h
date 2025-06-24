#ifndef TEXT_ICON_SCREEN_H
#define TEXT_ICON_SCREEN_H

#include "screen.h"
#include "../constants.h"

class TextIconScreen : public Screen {
public:
    TextIconScreen(const char* text, const uint16_t* iconData, int iconWidth, int iconHeight, 
                   uint16_t textColor = CYAN, uint16_t iconColor = WHITE);
    
    void render(Adafruit_GFX* display) override;
    const char* getName() const override { return "text_icon"; }
    
    // Update content
    void setText(const char* newText);
    void setIcon(const uint16_t* newIconData, int newWidth, int newHeight);
    void setTextColor(uint16_t newColor);
    void setIconColor(uint16_t newColor);

private:
    const char* text;
    const uint16_t* iconData;
    int iconWidth, iconHeight;
    uint16_t textColor, iconColor;
};

#endif // TEXT_ICON_SCREEN_H 