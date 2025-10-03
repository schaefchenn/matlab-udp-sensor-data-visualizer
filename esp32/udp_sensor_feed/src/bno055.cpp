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

void readBNO055Data(float *data) {
  sensors_event_t event;
  bno.getEvent(&event);

  imu::Vector<3> accel = bno.getVector(Adafruit_BNO055::VECTOR_ACCELEROMETER);

  data[0] = event.orientation.x;
  data[1] = event.orientation.y;
  data[2] = event.orientation.z;
  data[3] = accel.x();
  data[4] = accel.y();
  data[5] = accel.z();
}