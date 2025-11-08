#pragma once

#include <Arduino.h>
#include <XboxSeriesXControllerESP32_asukiaaa.hpp>

extern XboxSeriesXControllerESP32_asukiaaa::Core xboxController;
extern bool firstConnection;

void setupXboxController();
void xboxOnLoop(float* axes, bool* buttons, bool& connected);
void vibrate();
