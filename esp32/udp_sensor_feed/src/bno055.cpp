#include "bno055.h"

Adafruit_BNO055 bno = Adafruit_BNO055(55);

void setupBNO055() {
  Serial.begin(115200);
  delay(1000);

  Serial.println("BNO055 Test");

  if (!bno.begin()) {
    Serial.println("Fehler: BNO055 nicht gefunden. Überprüfe die Verkabelung!");
    while (1);
  }

  bno.setExtCrystalUse(true);
  Serial.println("Sensor initialisiert.");
}

String readBNO055Data() {
  sensors_event_t event;
  bno.getEvent(&event);

  imu::Vector<3> accel = bno.getVector(Adafruit_BNO055::VECTOR_ACCELEROMETER);

  String data = String(event.orientation.x, 2) +
                ";" + String(event.orientation.y, 2) +
                ";" + String(event.orientation.z, 2) +
                ";" + String(accel.x(), 2) +
                ";" + String(accel.y(), 2) +
                ";" + String(accel.z(), 2);
  return data;
}