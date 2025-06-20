#include "display.h"

// --- HARDWARE OBJECT DEFINITION ---
Adafruit_SSD1351 display = Adafruit_SSD1351(
  OLED_WIDTH, OLED_HEIGHT, &SPI, OLED_CS, OLED_DC, OLED_RST);

// --- FUNCTION IMPLEMENTATIONS ---

/**
 * @brief Initializes the OLED display (SSD1351)
 */
void initializeDisplay() {
  display.begin();
  display.fillScreen(BLACK);
  display.setCursor(0, 0);
  display.setTextSize(1);
  display.setTextColor(WHITE);
  Serial.println("Display initialized!");
  
  // Optional: Display a test message
  displayTest();
}

/**
 * @brief Displays a test message on the OLED screen
 */
void displayTest() {
  display.fillScreen(BLACK);
  display.setCursor(0, 0);
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.println("Display Test");
  display.setTextColor(CYAN);
  display.println("OLED Ready!");
  display.setTextColor(GREEN);
  display.println("BMP388 Ready!");
  
  delay(2000); // Show test message for 2 seconds
  display.fillScreen(BLACK); // Clear screen
}

/**
 * @brief Clears the display to black
 */
void clearDisplay() {
  display.fillScreen(BLACK);
}

/**
 * @brief Shows a simple message on the display
 */
void showMessage(const char* message, uint16_t color) {
  display.fillScreen(BLACK);
  display.setCursor(0, 0);
  display.setTextSize(1);
  display.setTextColor(color);
  display.println(message);
} 