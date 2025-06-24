#ifndef IMAGE_SCREEN_H
#define IMAGE_SCREEN_H

#include "screen.h"

class ImageScreen : public Screen {
public:
    ImageScreen(const uint16_t* imageData, int width, int height, int x = -1, int y = -1);
    
    void render(Adafruit_GFX* display) override;
    const char* getName() const override { return "image"; }
    
    // Update image
    void setImage(const uint16_t* newImageData, int newWidth, int newHeight);
    void setPosition(int x, int y);

private:
    const uint16_t* imageData;
    int width, height;
    int x, y; // -1 means center
};

#endif // IMAGE_SCREEN_H 