#include "text_icon_screen.h"
#include "../fatpixel_font.h"
#include "../images.h"
#include "../constants.h"

TextIconScreen::TextIconScreen(const char* text, const uint16_t* iconData, int iconWidth, int iconHeight, 
                               uint16_t textColor, uint16_t iconColor) 
    : text(text), iconData(iconData), iconWidth(iconWidth), iconHeight(iconHeight), 
      textColor(textColor), iconColor(iconColor) {
}

void TextIconScreen::render(Adafruit_GFX* display) {
    display->fillScreen(BLACK);
    
    int iconX = (128 - iconWidth) / 2;
    int iconY = 20;
    
    int textWidth = strlen(text) * 16;
    int textX = (128 - textWidth) / 2;
    int textY = iconY + iconHeight + 10;
    
    drawBitmap(display, iconData, iconX, iconY, iconWidth, iconHeight);
    
    drawFatpixelText(display, text, textX, textY, textColor);
}

void TextIconScreen::setText(const char* newText) {
    text = newText;
    setDirty(true);
}

void TextIconScreen::setIcon(const uint16_t* newIconData, int newWidth, int newHeight) {
    iconData = newIconData;
    iconWidth = newWidth;
    iconHeight = newHeight;
    setDirty(true);
}

void TextIconScreen::setTextColor(uint16_t newColor) {
    textColor = newColor;
    setDirty(true);
}

void TextIconScreen::setIconColor(uint16_t newColor) {
    iconColor = newColor;
    setDirty(true);
} 