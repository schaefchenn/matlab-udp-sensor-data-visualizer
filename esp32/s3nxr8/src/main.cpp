#include <Arduino.h>
#include "bno055.h"
#include "xbox.h"
#include "led.h"
#include "udp.h"

bool btConnected = false;

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

void SENSOR (void * pvParameters){
  while (1){
    float sensorValues[6];
    readBNO055Data(sensorValues);
    
    vTaskDelay(100 / portTICK_PERIOD_MS); // 100Hz
  }
}

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


void CONTROL (void * pvParameters){
  float axes[6];
  bool buttons[15];

  while (1){
    xboxOnLoop(axes, buttons, btConnected);
    vTaskDelay(10/ portTICK_PERIOD_MS); // 100Hz
  }
}

void LED (void * pvParameters){
  while (1){
    if (btConnected) {
      setLEDStatus(STATUS_BT_CONNECTED);
    } else {
      setLEDStatus(STATUS_BT_SEARCHING);
    }

    updateLED();
    vTaskDelay(50 / portTICK_PERIOD_MS); // 20Hz
  }
}


void setup() {
  Serial.begin(115200);
  while (!Serial) {
    vTaskDelay(10 / portTICK_PERIOD_MS);
  }

  Serial.println("Serial ready...");

  initLED();
  setupXboxController();
  setupBNO055();
  connectToWiFi();
  setupUdp();

  // Start CANcommunication (priority set to 1, 0 is the lowest priority)
  xTaskCreatePinnedToCore(SENSOR,                                         // Function to be called
                          "Read Sensor Data",                             // Name of task
                          8192,                                           // Increased stack size
                          NULL,                                           // Parameter to pass to function
                          2,                                              // Increased priority
                          NULL,                                           // Task handle
                          app_cpu);                                       // Assign to protocol core

  // Start CANcommunication (priority set to 1, 0 is the lowest priority)
  xTaskCreatePinnedToCore(CONTROL,                                        // Function to be called
                          "Control the Vehicle",                          // Name of task
                          8192,                                           // Increased stack size
                          NULL,                                           // Parameter to pass to function
                          2,                                              // Increased priority
                          NULL,                                           // Task handle
                          pro_cpu);                                       // Assign to protocol core
  
  // Start CANcommunication (priority set to 1, 0 is the lowest priority)
  xTaskCreatePinnedToCore(UDP,                                        // Function to be called
                          "Send Data over UDP",                          // Name of task
                          8192,                                           // Increased stack size
                          NULL,                                           // Parameter to pass to function
                          2,                                              // Increased priority
                          NULL,                                           // Task handle
                          pro_cpu);                                       // Assign to protocol core

  // Start CANcommunication (priority set to 1, 0 is the lowest priority)
  xTaskCreatePinnedToCore(LED,                                            // Function to be called
                          "Handle LED",                                   // Name of task
                          4096,                                           // Increased stack size
                          NULL,                                           // Parameter to pass to function
                          2,                                              // Increased priority
                          NULL,                                           // Task handle
                          app_cpu);                                       // Assign to protocol core
}

void loop() {
}
