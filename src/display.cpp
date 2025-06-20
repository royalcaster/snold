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

/**
 * @brief Shows text using custom font at specific coordinates
 */
void showCustomText(const char* text, int x, int y, uint16_t color, uint8_t fontSize) {
  drawCustomText(&display, text, x, y, color, fontSize);
}

/**
 * @brief Shows a message using custom font (centered)
 */
void showCustomMessage(const char* message, uint16_t color, uint8_t fontSize) {
  display.fillScreen(BLACK);
  
  // Calculate text width for centering
  int textWidth = strlen(message) * fontSize;
  int x = (OLED_WIDTH - textWidth) / 2;
  int y = (OLED_HEIGHT - fontSize) / 2;
  
  drawCustomText(&display, message, x, y, color, fontSize);
}

/**
 * @brief Demonstrates the custom fonts
 */
void displayFontDemo() {
  display.fillScreen(BLACK);
  
  // Show 8x8 font
  showCustomText("8x8 FONT", 10, 10, CYAN, FONT_SIZE_8);
  showCustomText("ABCDEFGHIJKLM", 10, 25, WHITE, FONT_SIZE_8);
  showCustomText("NOPQRSTUVWXYZ", 10, 35, WHITE, FONT_SIZE_8);
  showCustomText("0123456789", 10, 45, GREEN, FONT_SIZE_8);
  showCustomText("!@#$%^&*()", 10, 55, YELLOW, FONT_SIZE_8);
  
  delay(3000);
  
  // Show 16x16 font
  display.fillScreen(BLACK);
  showCustomText("16x16 FONT", 5, 10, MAGENTA, FONT_SIZE_16);
  showCustomText("0123456789", 5, 35, RED, FONT_SIZE_16);
  
  delay(3000);
  display.fillScreen(BLACK);
} 