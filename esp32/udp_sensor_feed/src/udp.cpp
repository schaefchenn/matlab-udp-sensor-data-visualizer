#include <udp.h>

// Wi-Fi & UDP
const char* ssid     = "WLAN-448466";
const char* password = "37917074502947442992";
const char* udpAddress = "192.168.2.224"; // PC IP
const int remotePort = 5005;              // PC hört hier
WiFiUDP udp;

void connectToWiFi() {
    Serial.print("Connecting to ");
    Serial.println(ssid);
    
    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }

    Serial.println("\nWiFi connected. IP: ");
    Serial.println(WiFi.localIP());
}

void setupUdp() {
    udp.begin(remotePort); // ESP32 bindet lokalen Port
}

void sendUdpPacket(const uint8_t* data, size_t len) {
    if (udp.beginPacket(udpAddress, remotePort)) {
        udp.write(data, len);
        udp.endPacket();
    } else {
        Serial.println("UDP beginPacket failed or buffer full");
    }
}