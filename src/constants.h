#ifndef CONSTANTS_H
#define CONSTANTS_H

#define SEALEVELPRESSURE_HPA 1013.25
#define BAROMETER_READ_RATE_HZ 10

// RGB565 color definitions
#define BLACK 0x0000
#define BLUE 0x001F
#define RED 0xF800
#define GREEN 0x07E0
#define CYAN 0x07FF
#define MAGENTA 0xF81F
#define YELLOW 0xFFE0
#define WHITE 0xFFFF

// I2C pins
const int BMP_SDA_PIN = 21;
const int BMP_SCL_PIN = 22;

// SPI pins
#define OLED_WIDTH 128
#define OLED_HEIGHT 128
#define OLED_CS 5
#define OLED_DC 27
#define OLED_RST 4

#endif // CONSTANTS_H 