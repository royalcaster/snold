#ifndef RESULT_SCREEN_H
#define RESULT_SCREEN_H

#include "screen.h"

class ResultScreen : public Screen {
public:
    ResultScreen(const char* name, const char* initialText);

    // Functions required by the parent Screen class
    void render(Adafruit_GFX* display) override;
    const char* getName() const override;

    // Custom function for this screen
    void setText(const char* newText);

private:
    const char* screenName;
    char text[32];
};

#endif // RESULT_SCREEN_H 