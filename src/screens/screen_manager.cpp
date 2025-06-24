#include "screen_manager.h"
#include <algorithm>

ScreenManager::ScreenManager(Adafruit_GFX* display) : display(display) {}

ScreenManager::~ScreenManager() {
    // Clean up screens
    for (auto& pair : screens) {
        delete pair.second;
    }
    screens.clear();
}

void ScreenManager::addScreen(const char* name, Screen* screen) {
    screens[name] = screen;
}

void ScreenManager::showScreen(const char* name) {
    auto it = screens.find(name);
    if (it == screens.end()) {
        Serial.printf("Screen '%s' not found\n", name);
        return;
    }
    
    Screen* newScreen = it->second;
    
    // Exit current screen
    if (currentScreen) {
        currentScreen->onExit();
    }
    
    // Update screen stack
    if (currentScreen) {
        screenStack.push_back(getCurrentScreenName());
    }
    
    // Set new current screen
    currentScreen = newScreen;
    currentScreen->onEnter();
    currentScreen->setDirty(true);
    
    Serial.printf("Switched to screen: %s\n", name);
}

void ScreenManager::goBack() {
    if (screenStack.empty()) {
        Serial.println("No screens to go back to");
        return;
    }
    
    const char* previousScreenName = screenStack.back();
    screenStack.pop_back();
    
    showScreen(previousScreenName);
}

void ScreenManager::update() {
    // Update current screen
    if (currentScreen) {
        currentScreen->update();
        
        // Render if dirty
        if (currentScreen->isDirty()) {
            renderCurrentScreen();
            currentScreen->setDirty(false);
        }
    }
}

const char* ScreenManager::getCurrentScreenName() const {
    if (!currentScreen) return nullptr;
    return currentScreen->getName();
}

void ScreenManager::renderCurrentScreen() {
    if (currentScreen) {
        currentScreen->render(display);
    }
} 