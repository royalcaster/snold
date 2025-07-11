#ifndef SENSOR_H
#define SENSOR_H

#include <Adafruit_BMP3XX.h>
#include "constants.h"

extern Adafruit_BMP3XX bmp;
void initializeBarometer();
bool readAltitude(float& altitude);
bool readTemperature(float& temperature);
bool readPressure(float& pressure);

#endif // SENSOR_H 