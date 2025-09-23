#pragma once

#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BNO055.h>

extern Adafruit_BNO055 bno;

void setupBNO055();
String readBNO055Data();