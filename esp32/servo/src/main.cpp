#include <Arduino.h>
#include <ESP32Servo.h> // Servo library for ESP32

#define steeringPin 21     // Pin for steering servo
Servo absimaServo;


void setup() {
  absimaServo.attach(steeringPin);
  absimaServo.write(90); // Center the steering servo
  Serial.begin(115200);
  Serial.println("Steering Setup Done!");
}

void loop() {
  // put your main code here, to run repeatedly:
}
