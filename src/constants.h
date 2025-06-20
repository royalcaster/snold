#ifndef CONSTANTS_H
#define CONSTANTS_H

// --- CONSTANTS ---
// Sea level pressure in hPa. Used for altitude calculation.
#define SEALEVELPRESSURE_HPA 1013.25

// Color definitions (RGB565 format for SSD1351)
#define BLACK 0x0000
#define BLUE 0x001F
#define RED 0xF800
#define GREEN 0x07E0
#define CYAN 0x07FF
#define MAGENTA 0xF81F
#define YELLOW 0xFFE0
#define WHITE 0xFFFF

// --- PIN DEFINITIONS ---
// I2C Pins for BMP388 sensor
const int BMP_SDA_PIN = 21;
const int BMP_SCL_PIN = 22;

// SPI Pins for OLED Display (SSD1351)
#define OLED_WIDTH 128
#define OLED_HEIGHT 128
#define OLED_CS 5      // Chip Select
#define OLED_DC 27     // Data/Command
#define OLED_RST 4     // Reset

#endif // CONSTANTS_H 