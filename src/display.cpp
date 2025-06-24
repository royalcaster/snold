#include "display.h"
#include <SPI.h>

// --- HARDWARE OBJECT DEFINITION ---
Adafruit_SSD1351 display = Adafruit_SSD1351(
  OLED_WIDTH, OLED_HEIGHT, &SPI, OLED_CS, OLED_DC, OLED_RST);

// --- FUNCTION IMPLEMENTATIONS ---

/**
 * @brief Initializes the OLED display (SSD1351) with optimized SPI settings
 */
void initializeDisplay() {
  // PERFORMANCE OPTIMIZATION: Set maximum SPI speed before display initialization
  // SSD1351 supports up to 20MHz SPI clock (datasheet spec)
  // ESP32 can handle this speed reliably with proper wiring
  SPI.setFrequency(20000000);  // 20MHz - maximum stable speed for SSD1351
  SPI.setDataMode(SPI_MODE0);  // Clock polarity/phase optimization
  SPI.setBitOrder(MSBFIRST);   // Most significant bit first
  
  display.begin();
  
  // Clear screen immediately for faster startup
  display.fillScreen(BLACK);
  display.setCursor(0, 0);
  display.setTextSize(1);
  display.setTextColor(WHITE);
  
  Serial.println("Display initialized with 20MHz SPI!");
}

/**
 * @brief Optimized display initialization with custom SPI speed
 * @param spiSpeed Custom SPI frequency in Hz (default: 20MHz)
 */
void initializeDisplayCustomSpeed(uint32_t spiSpeed) {
  Serial.printf("Initializing display with %dMHz SPI...\n", spiSpeed / 1000000);
  
  SPI.setFrequency(spiSpeed);
  SPI.setDataMode(SPI_MODE0);
  SPI.setBitOrder(MSBFIRST);
  
  display.begin();
  display.fillScreen(BLACK);
  display.setCursor(0, 0);
  display.setTextSize(1);
  display.setTextColor(WHITE);
  
  Serial.printf("Display ready at %dMHz SPI\n", spiSpeed / 1000000);
}

/**
 * @brief Performance test function to measure display refresh rates
 */
void benchmarkDisplay() {
  Serial.println("=== DISPLAY PERFORMANCE BENCHMARK ===");
  
  uint32_t startTime, endTime;
  const int TEST_ITERATIONS = 10;
  
  // Test 1: fillScreen performance
  startTime = millis();
  for (int i = 0; i < TEST_ITERATIONS; i++) {
    display.fillScreen(i % 2 ? WHITE : BLACK);
  }
  endTime = millis();
  float fillScreenTime = (endTime - startTime) / (float)TEST_ITERATIONS;
  Serial.printf("fillScreen(): %.1fms per frame (%.1f FPS)\n", fillScreenTime, 1000.0 / fillScreenTime);
  
  // Test 2: drawRGBBitmap performance (simulated with small bitmap)
  uint16_t testBitmap[128*4]; // 4 rows of test data
  for (int i = 0; i < 128*4; i++) {
    testBitmap[i] = 0xF800; // Red pixels
  }
  
  startTime = millis();
  for (int i = 0; i < TEST_ITERATIONS; i++) {
    for (int row = 0; row < 32; row += 4) { // Draw 4-row strips
      display.drawRGBBitmap(0, row, testBitmap, 128, 4);
    }
  }
  endTime = millis();
  float bitmapTime = (endTime - startTime) / (float)TEST_ITERATIONS;
  Serial.printf("drawRGBBitmap(): %.1fms per frame (%.1f FPS)\n", bitmapTime, 1000.0 / bitmapTime);
  
  Serial.println("=== BENCHMARK COMPLETE ===");
  display.fillScreen(BLACK);
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