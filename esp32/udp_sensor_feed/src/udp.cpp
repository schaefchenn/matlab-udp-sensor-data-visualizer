#include "udp.h"

const char* ssid     = "WLAN-448466";
const char* password = "37917074502947442992";
const char* udpAddress = "192.168.2.210";
const int udpPort = 5005;
WiFiUDP udp;

void connectToWiFi() {
    Serial.print("Connecting to ");
    Serial.println(ssid);
    
    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }

    Serial.println("WiFi connected. IP: ");
    Serial.println(WiFi.localIP());
}

void setupUdp() {
    udp.begin(udpPort);
}

void sendUdpPacket(const uint8_t* data, size_t len) {
    udp.beginPacket(udpAddress, udpPort);
    udp.write(data, len);
    udp.endPacket();
}
