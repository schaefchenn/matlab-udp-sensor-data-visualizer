#include <Arduino.h>
#include "bno055.h"
#include "xbox.h"
#include "led.h"

#define LED_PIN     38
#define LED_COUNT   1   // nur eine LED onboard

bool btConnected = false;

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

    /*
    Serial.print("Yaw: "); Serial.print(sensorValues[0], 2);
    Serial.print(" | Pitch: "); Serial.print(sensorValues[1], 2);
    Serial.print(" | Roll: "); Serial.print(sensorValues[2], 2);
    Serial.print(" | AccX: "); Serial.print(sensorValues[3], 2);
    Serial.print(" | AccY: "); Serial.print(sensorValues[4], 2);
    Serial.print(" | AccZ: "); Serial.println(sensorValues[5], 2);
    */

    vTaskDelay(100 / portTICK_PERIOD_MS); // 100Hz
  }
}

void CONTROL (void * pvParameters){
  float axes[6];
  bool buttons[15];

  while (1){
    xboxOnLoop(axes, buttons, btConnected);
    Serial.print("LHx: " + String(axes[0])); Serial.print(" | LHy: " + String(axes[1]));
    Serial.print(" | RHx: " + String(axes[2])); Serial.print(" | RHy: " + String(axes[3]));
    Serial.print(" | LT: " + String(axes[4])); Serial.print(" | RT: " + String(axes[5]));
    Serial.print(" | A pressed: "); Serial.println(buttons[0]);
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
  vTaskDelay(2000 / portTICK_PERIOD_MS);

  initLED();
  setupXboxController();
  setupBNO055();

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
                          app_cpu);                                       // Assign to protocol core

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
