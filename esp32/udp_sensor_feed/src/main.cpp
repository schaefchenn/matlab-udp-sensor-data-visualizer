#include <Arduino.h>
#include "udp.h"
#include "bno055.h"

#define RINGBUFFER_SIZE 28

float ringBuffer[RINGBUFFER_SIZE][7]; // [timestamp, roll, pitch, yaw, accel_x, accel_y, accel_z]
volatile int writeIndex = 0;
volatile bool firstHalfReady = false;

unsigned long millisOffset = 0;

// Core definitions (assuming you have dual-core ESP32)
static const BaseType_t pro_cpu = 0; // protocol core
static const BaseType_t app_cpu = 1; // application core

// Initialize CPU cores
TaskHandle_t Task1;
TaskHandle_t Task2;

void UDP (void * pvParameters){
  int readIndex = 0;

  Serial.println("Client connected — starting UDP stream");
  while (1){
    if (readIndex != writeIndex) {
      sendUdpPacket((uint8_t*)ringBuffer[readIndex], sizeof(float) * 7);
      readIndex = (readIndex + 1) % RINGBUFFER_SIZE;
    }
    vTaskDelay(5 / portTICK_PERIOD_MS);
  }
}

void SENSOR (void * pvParameters){
  while (1){
    float timestamp = (millis() - millisOffset) / 1000.0;
    float sensorValues[6];
    readBNO055Data(sensorValues);

    ringBuffer[writeIndex][0] = timestamp;
    for (int i = 0; i < 6; i++) {
      ringBuffer[writeIndex][i + 1] = sensorValues[i];
    }
    writeIndex = (writeIndex + 1) % RINGBUFFER_SIZE;
    vTaskDelay(20 / portTICK_PERIOD_MS); // 100Hz
  }
}

void setup() {
  // Initialize serial communication at 115200 baud rate
  Serial.begin(115200);
  Serial.println("Starting...");

  // Wait a moment to start (so we don't miss Serial output)
  vTaskDelay(2000 / portTICK_PERIOD_MS);
  Serial.println("Starting...");
  pinMode(LED_BUILTIN, OUTPUT);
  setupBNO055();
  setupWiFiAP();
  setupUdp();
  
  digitalWrite(LED_BUILTIN, HIGH);

  millisOffset = millis();

  // Start UDP handling (priority set to 1, 0 is the lowest priority)
  xTaskCreatePinnedToCore(UDP,                                          // Function to be called
                          "UDP handling",                               // Name of task
                          8192,                                         // Increased stack size
                          NULL,                                         // Parameter to pass to function
                          2,                                            // Increased priority
                          &Task1,                                         // Task handle
                          pro_cpu);

  // Start CANcommunication (priority set to 1, 0 is the lowest priority)
  xTaskCreatePinnedToCore(SENSOR,                                       // Function to be called
                          "Read Sensor Data",                           // Name of task
                          8192,                                         // Increased stack size
                          NULL,                                         // Parameter to pass to function
                          2,                                            // Increased priority
                          &Task2,                                       // Task handle
                          app_cpu);                                     // Assign to protocol core  
}

void loop() {
  // NO CODE HERE!
}
