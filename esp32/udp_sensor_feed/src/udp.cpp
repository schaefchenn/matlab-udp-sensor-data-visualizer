#include "udp.h"

const char* ssid     = "vehicle_ap";
const char* password = "nohalfsends";

const int udpPort = 5005;
WiFiUDP udp;

// IP-Konfiguration für Access Point
IPAddress local_ip(192,168,4,1);
IPAddress gateway(192,168,4,1);
IPAddress subnet(255,255,255,0);

// Ziel-IP: Laptop (Client)
IPAddress udpTargetIP(192,168,4,2);   // ← WICHTIG

void setupWiFiAP() {
    Serial.println("Starting ESP32 as Access Point...");

    WiFi.mode(WIFI_AP);
    WiFi.softAP(ssid, password);
    WiFi.softAPConfig(local_ip, gateway, subnet);

    delay(100);

    Serial.print("AP IP address: ");
    Serial.println(WiFi.softAPIP());
}

void setupUdp() {
    udp.begin(udpPort);
    Serial.print("UDP started on port ");
    Serial.println(udpPort);
}

void sendUdpPacket(const uint8_t* data, size_t len) {
    udp.beginPacket(udpTargetIP, udpPort);
    udp.write(data, len);
    udp.endPacket();
}
