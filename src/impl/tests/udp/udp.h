#include <Arduino.h>
#include <WiFi.h>
#include <WiFiUDP.h>
#include <ESPmDNS.h>
#include <impl/tests/udp/network.h>

const char *host = "mechaleague-bot-0";

const uint controlServerPort = 1010;
WiFiUDP controlServer;

const uint telemetryServerPort = 1011;
WiFiUDP telemetryServer;

TaskHandle_t controlServerTaskHandle;
void controlServerTask(void *pvParameters) {
    while (true) {
        int packetSize = controlServer.parsePacket();
        if (packetSize) {
            Serial.println("Received packet from control server");
            char packetBuffer[packetSize];
            controlServer.read(packetBuffer, packetSize);
            controlServer.beginPacket(controlServer.remoteIP(), controlServer.remotePort());
            controlServer.print(packetBuffer);
            controlServer.endPacket();
        }
        vTaskDelay(10 / portTICK_PERIOD_MS);
    }
}

TaskHandle_t telemetryServerTaskHandle;
void telemetryServerTask(void *pvParameters) {
    while (true) {
        int packetSize = telemetryServer.parsePacket();
        if (packetSize) {
            Serial.println("Received packet from telemetry server");
            char packetBuffer[packetSize];
            telemetryServer.read(packetBuffer, packetSize);
            telemetryServer.beginPacket(telemetryServer.remoteIP(), telemetryServer.remotePort());
            telemetryServer.print(packetBuffer);
            telemetryServer.endPacket();
        }
        vTaskDelay(50 / portTICK_PERIOD_MS);
    }
}

void setup() {
    Serial.begin(115200);

    WiFi.mode(WIFI_STA);
    // WiFi.config(localIP, gateway, subnet);
    WiFi.begin(SSID, PASS);

    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.println("Connecting to WiFi...");
        WiFi.begin(SSID, PASS);
    }

    Serial.println("Connected to WiFi");

    controlServer.begin(controlServerPort);
    telemetryServer.begin(telemetryServerPort);

    xTaskCreate(controlServerTask, "Control Server Task", 4096, NULL, 1, &controlServerTaskHandle);
    xTaskCreate(telemetryServerTask, "Telemetry Server Task", 4096, NULL, 1, &telemetryServerTaskHandle);

    if (MDNS.begin(host)) {
        Serial.println("MDNS responder started");
    } else {
        Serial.println("Error setting up MDNS responder!");
    }

    MDNS.addService("udp", "control", controlServerPort);
    MDNS.addService("udp", "telemetry", telemetryServerPort);

    Serial.println("Setup complete");
}

void loop() {

}
