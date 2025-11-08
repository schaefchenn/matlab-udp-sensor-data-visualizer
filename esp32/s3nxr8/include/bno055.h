#pragma once

#include <Wire.h>
#include <Adafruit_BNO055.h>
#include <Adafruit_Sensor.h>
#include <utility/imumaths.h>

#define SDA_PIN 8
#define SCL_PIN 9

extern Adafruit_BNO055 bno;

void setupBNO055();
void readBNO055Data(float *data);