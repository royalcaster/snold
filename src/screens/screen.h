#ifndef SCREEN_H
#define SCREEN_H

#include <Adafruit_GFX.h>

class Screen {
public:
    virtual ~Screen() = default;
    
    // Core rendering method - must be implemented by derived classes
    virtual void render(Adafruit_GFX* display) = 0;
    
    // Optional lifecycle methods
    virtual void onEnter() {}
    virtual void onExit() {}
    virtual void update() {}
    
    // Check if screen needs redrawing
    virtual bool isDirty() const { return dirty; }
    virtual void setDirty(bool value) { dirty = value; }
    
    // Get screen name for identification
    virtual const char* getName() const = 0;

protected:
    bool dirty = true; // Screen starts dirty (needs initial render)
};

#endif // SCREEN_H 