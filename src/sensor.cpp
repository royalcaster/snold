#include "sensor.h"
#include <Wire.h>

// --- HARDWARE OBJECT DEFINITION ---
Adafruit_BMP3XX bmp;

// --- FUNCTION IMPLEMENTATIONS ---

/**
 * @brief Initializes the BMP388 barometer sensor
 */
void initializeBarometer() {
  // Initialize I2C communication
  Wire.begin(BMP_SDA_PIN, BMP_SCL_PIN);

  // Initialize BMP388 sensor
  if (!bmp.begin_I2C()) {
    Serial.println("Could not find a valid BMP3XX sensor, check wiring!");
    while (1) delay(10);
  }

  // Configure BMP388 sensor settings for optimal performance
  bmp.setTemperatureOversampling(BMP3_OVERSAMPLING_2X);
  bmp.setPressureOversampling(BMP3_OVERSAMPLING_4X);
  bmp.setIIRFilterCoeff(BMP3_IIR_FILTER_COEFF_3);
  bmp.setOutputDataRate(BMP3_ODR_25_HZ);
  
  Serial.println("BMP388 sensor configured successfully!");
}

/**
 * @brief Reads altitude from the sensor
 * @param altitude Reference to store the altitude value
 * @return true if reading was successful, false otherwise
 */
bool readAltitude(float& altitude) {
  if (!bmp.performReading()) {
    return false;
  }
  altitude = bmp.readAltitude(SEALEVELPRESSURE_HPA);
  return true;
}

/**
 * @brief Reads temperature from the sensor
 * @param temperature Reference to store the temperature value
 * @return true if reading was successful, false otherwise
 */
bool readTemperature(float& temperature) {
  if (!bmp.performReading()) {
    return false;
  }
  temperature = bmp.temperature;
  return true;
}

/**
 * @brief Reads pressure from the sensor
 * @param pressure Reference to store the pressure value
 * @return true if reading was successful, false otherwise
 */
bool readPressure(float& pressure) {
  if (!bmp.performReading()) {
    return false;
  }
  pressure = bmp.pressure / 100.0; // Convert to hPa
  return true;
} 