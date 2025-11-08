#pragma once

#include <Arduino.h>
#include <Adafruit_NeoPixel.h>

#define LED_PIN   38
#define LED_COUNT 1

enum LedStatus {
  STATUS_IDLE,
  STATUS_BT_SEARCHING,
  STATUS_BT_CONNECTED,
  STATUS_WIFI_CONNECTED,
  STATUS_BOTH_CONNECTED,
  STATUS_ERROR
};

void initLED();
void updateLED();
void setLEDStatus(LedStatus newStatus);

void setLED(uint8_t r, uint8_t g, uint8_t b);
void ledBluetoothSearching();
void ledBothConnectedFade();