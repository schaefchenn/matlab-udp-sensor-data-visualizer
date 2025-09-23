#include <Arduino.h>
#include "udp.h"
#include "bno055.h"

#define RINGBUFFER_SIZE 28

String ringBuffer[RINGBUFFER_SIZE];
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
  while (1){
    if (readIndex != writeIndex) {
      String msg = ringBuffer[readIndex] + "\n";
      sendUdpPacket(msg.c_str());
      readIndex = (readIndex + 1) % RINGBUFFER_SIZE;
    }
    vTaskDelay(1 / portTICK_PERIOD_MS); // kleine Pause
  }
}

void SENSOR (void * pvParameters){
  while (1){
    float timestamp = (millis() - millisOffset) / 1000.0;
    ringBuffer[writeIndex] = String(timestamp, 3) + ";" + readBNO055Data();
    writeIndex = (writeIndex + 1) % RINGBUFFER_SIZE;
    vTaskDelay(10 / portTICK_PERIOD_MS); // 100Hz
  }
}

void setup() {
  // Initialize serial communication at 115200 baud rate
  Serial.begin(115200);

  // Wait a moment to start (so we don't miss Serial output)
  vTaskDelay(2000 / portTICK_PERIOD_MS);
  Serial.println("Starting...");
  pinMode(LED_BUILTIN, OUTPUT);
  setupBNO055();
  connectToWiFi();
  setupUdp();
  
  digitalWrite(LED_BUILTIN, HIGH);

  millisOffset = millis();

  // Start UDP handling (priority set to 1, 0 is the lowest priority)
  xTaskCreatePinnedToCore(UDP,                                          // Function to be called
                          "UDP handling",                               // Name of task
                          8192,                                         // Increased stack size
                          NULL,                                         // Parameter to pass to function
                          2,                                            // Increased priority
                          NULL,                                         // Task handle
                          app_cpu);

  // Start CANcommunication (priority set to 1, 0 is the lowest priority)
  xTaskCreatePinnedToCore(SENSOR,                                       // Function to be called
                          "Read Sensor Data",                           // Name of task
                          8192,                                         // Increased stack size
                          NULL,                                         // Parameter to pass to function
                          2,                                            // Increased priority
                          NULL,                                         // Task handle
                          app_cpu);                                     // Assign to protocol core 
}

void loop() {
  // NO CODE HERE!
}
