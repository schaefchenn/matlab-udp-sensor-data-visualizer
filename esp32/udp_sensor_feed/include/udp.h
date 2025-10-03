#pragma once

#include <WiFi.h>
#include <WiFiUdp.h>

extern const char* ssid;
extern const char* password;
extern const char* udpAddress;
extern const int udpPort;
extern WiFiUDP udp;

void connectToWiFi();
void setupUdp();
void sendUdpPacket(const uint8_t* data, size_t len);
