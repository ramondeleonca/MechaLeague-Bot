#include <Arduino.h>
#include <WebSocketsServer.h>
#include <ESPAsyncWebServer.h>
#include <constants.h>
#include <WiFi.h>
#include <lib/hal/MechaLeagueMotorController.h>

// REST API
AsyncWebServer rest(80);

// WebSockets
WebSocketsServer ws(81);

void setup() {
    WiFi.softAP("MechaLeague-Bot-AP", "password123", 11, 0, 1);
}

void loop() {

}