#ifndef WELCOME_SCREEN_H
#define WELCOME_SCREEN_H

#include "screen.h"

class WelcomeScreen : public Screen {
public:
    WelcomeScreen();
    
    void render(Adafruit_GFX* display) override;
    const char* getName() const override { return "welcome"; }

private:
    const char* message = "HALLO";
};

#endif // WELCOME_SCREEN_H 