#ifndef ANIMATED_IMAGE_SCREEN_H
#define ANIMATED_IMAGE_SCREEN_H

#include "screen.h"
#include "../constants.h"
#include <stdint.h>

class AnimatedImageScreen : public Screen {
public:
    AnimatedImageScreen(const char* name, const uint16_t* imageA, const uint16_t* imageB, int width, int height);
    
    virtual void render(Adafruit_GFX* display) override;
    virtual void update() override;
    virtual const char* getName() const override;

private:
    const char* name;
    const uint16_t* imageA;
    const uint16_t* imageB;
    int width;
    int height;
    unsigned long lastToggle;
    bool showingA;
    static const unsigned long TOGGLE_INTERVAL = 500;
};

#endif // ANIMATED_IMAGE_SCREEN_H 