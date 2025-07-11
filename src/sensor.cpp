#include "sensor.h"
#include <Wire.h>

Adafruit_BMP3XX bmp;

// Initialize BMP388 sensor
void initializeBarometer() {
  Wire.begin(BMP_SDA_PIN, BMP_SCL_PIN);
  if (!bmp.begin_I2C()) {
    Serial.println("Could not find a valid BMP3XX sensor, check wiring!");
    while (1) delay(10);
  }


  bmp.setTemperatureOversampling(BMP3_OVERSAMPLING_2X);
  bmp.setPressureOversampling(BMP3_OVERSAMPLING_4X);
  bmp.setIIRFilterCoeff(BMP3_IIR_FILTER_COEFF_3);
  bmp.setOutputDataRate(BMP3_ODR_25_HZ);
}

// Read altitude from sensor
bool readAltitude(float& altitude) {
  if (!bmp.performReading()) {
    return false;
  }
  altitude = bmp.readAltitude(SEALEVELPRESSURE_HPA);
  return true;
}

// Read temperature from sensor
bool readTemperature(float& temperature) {
  if (!bmp.performReading()) {
    return false;
  }
  temperature = bmp.temperature;
  return true;
}

// Read pressure from sensor
bool readPressure(float& pressure) {
  if (!bmp.performReading()) {
    return false;
  }
  pressure = bmp.pressure / 100.0;
  return true;
} 