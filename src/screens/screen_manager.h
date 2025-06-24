#ifndef SCREEN_MANAGER_H
#define SCREEN_MANAGER_H

#include <map>
#include <vector>
#include "screen.h"

class ScreenManager {
public:
    ScreenManager(Adafruit_GFX* display);
    ~ScreenManager();
    
    // Add a screen to the manager
    void addScreen(const char* name, Screen* screen);
    
    // Show a screen
    void showScreen(const char* name);
    
    // Go back to previous screen
    void goBack();
    
    // Update the current screen
    void update();
    
    // Get current screen name
    const char* getCurrentScreenName() const;
    
private:
    Adafruit_GFX* display;
    std::map<const char*, Screen*> screens;
    std::vector<const char*> screenStack;
    Screen* currentScreen = nullptr;
    
    void renderCurrentScreen();
};

#endif // SCREEN_MANAGER_H 