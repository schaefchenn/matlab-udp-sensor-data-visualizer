#include "led.h"
#include <math.h>

static Adafruit_NeoPixel led(LED_COUNT, LED_PIN, NEO_GRB + NEO_KHZ800);
static LedStatus currentStatus = STATUS_IDLE;

// === Initialisierung ===
void initLED() {
  led.begin();
  led.show();
  setLED(0, 0, 0);
}

// === Setze aktuellen Status ===
void setLEDStatus(LedStatus newStatus) {
  currentStatus = newStatus;
}

// === Grundfunktion zum Setzen der Farbe ===
void setLED(uint8_t r, uint8_t g, uint8_t b) {
  led.setPixelColor(0, led.Color(r, g, b));
  led.show();
}

// === Atmung in Blau für Bluetooth-Suche ===
void ledBluetoothSearching() {
  static float t = 0;
  t += 0.2;
  float brightness = (sin(t) + 1.0) / 2.0 * 255.0;
  setLED(0, 0, (uint8_t)brightness);
}

// === Gegentakt-Atmung für beide verbunden ===
void ledBothConnectedFade() {
  static float t = 0;
  t += 0.05;
  uint8_t g = (sin(t) + 1.0) / 2.0 * 255;
  uint8_t b = (sin(t + PI) + 1.0) / 2.0 * 255;
  setLED(0, g, b);
}

// === LED-Update basierend auf Status ===
void updateLED() {
  switch (currentStatus) {
    case STATUS_IDLE:
      setLED(0, 0, 0); // aus
      break;

    case STATUS_BT_SEARCHING:
      ledBluetoothSearching();
      break;

    case STATUS_BT_CONNECTED:
      setLED(0, 0, 255); // blau
      break;

    case STATUS_WIFI_CONNECTED:
      setLED(0, 255, 0); // grün
      break;

    case STATUS_BOTH_CONNECTED:
      ledBothConnectedFade(); // sanftes Farbspiel grün↔blau
      break;

    case STATUS_ERROR:
      setLED(255, 0, 0); // rot
      break;
  }
}