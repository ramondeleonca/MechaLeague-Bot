#include <Arduino.h>
#include <constants.h>
#include <WS2812FX.h>
#include <WiFi.h>
#include <cJSON.h>
#include <WebSocketsServer.h>
#include <ESPAsyncWebServer.h>
#include <ESPmDNS.h>
#include <lib/hal/MechaLeagueMotorController.h>
#include <lib/drive/ChassisSpeeds.h>
#include <lib/drive/MecanumDriveKinematics.h>

// Drivetrain
MechaLeagueMotorController motors(MOTOR_CONTROLLER_RX_PIN, MOTOR_CONTROLLER_TX_PIN, MOTOR_CONTROLLER_ENABLE_PIN);
float wheelSpeeds[4];
MecanumDriveKinematics kinematics(WHEEL_DIAMETER_MM / 1000, TRACK_WIDTH_MM / 1000, WHEEL_BASE_MM / 1000);
ChassisSpeeds chassisSpeeds;

// LEDs
WS2812FX leds(LED_COUNT, LED_PIN, NEO_GRB + NEO_KHZ800);

// Communication
AsyncWebServer rest(REST_SERVER_PORT);
WebSocketsServer ws(WS_SERVER_PORT);

// Regisster Websocket Events
void handleWebsocketEvent(uint8_t num, WStype_t type, uint8_t *payload, size_t length) {
    switch (type) {
        case WStype_TEXT: {
            cJSON *root = cJSON_Parse((char *)payload);
            cJSON *vx = cJSON_GetObjectItem(root, "vx");
            cJSON *vy = cJSON_GetObjectItem(root, "vy");
            cJSON *omega = cJSON_GetObjectItem(root, "omega");

            if (vx != NULL) {
                chassisSpeeds.setVx(vx->valuedouble);
            }
            if (vy != NULL) {
                chassisSpeeds.setVy(vy->valuedouble);
            }
            if (omega != NULL) {
                chassisSpeeds.setOmega(omega->valuedouble);
            }

            cJSON_Print(root);

            cJSON_Delete(root);
            break;
        }
        case WStype_DISCONNECTED:
            break;
        case WStype_CONNECTED:
            break;
        case WStype_ERROR:
            break;
        case WStype_BIN:
            break;
        case WStype_FRAGMENT_TEXT_START:
            break;
        case WStype_FRAGMENT_BIN_START:
            break;
        case WStype_FRAGMENT:
            break;
        case WStype_FRAGMENT_FIN:
            break;
    }
}

// Reguster REST routess
void registerRestRoutes() {
    rest.on("/ping", HTTP_GET, [](AsyncWebServerRequest *request) {
        request->send(200, "text/plain", "pong");
    });
}

void setup() {
    // Initialize serial
    Serial.begin(115200);

    // Initialize LEDs
    leds.init();
    leds.setBrightness(255);
    leds.setSpeed(100);
    leds.setColor(0xFF0000);
    leds.start();
    leds.service();

    // Initalize communication
    WiFi.softAP(SSID, PASSWORD, WIFI_CHANNEL, 0, 1);
    WiFi.setHostname(HOSTNAME.c_str());
    if (MDNS.begin(HOSTNAME.c_str())) {
        MDNS.addService("http", "tcp", REST_SERVER_PORT);
        MDNS.addService("ws", "tcp", WS_SERVER_PORT);
    }

    // Start REST server
    registerRestRoutes();
    rest.begin();

    // Start WebSocket server
    ws.enableHeartbeat(1000, 6000, 3);
    ws.onEvent(handleWebsocketEvent);
    ws.begin();

    // Update LEDs
    leds.setColor(0x00FF00);
    leds.service();

    // Enable motors
    motors.enable();
}

void loop() {
    ws.loop();
    leds.service();

    kinematics.calculateWheelSpeeds(wheelSpeeds, chassisSpeeds.getVx(), chassisSpeeds.getVy(), chassisSpeeds.getOmega());
    kinematics.normalizeWheelSpeeds(wheelSpeeds);  
    motors.setMotors(
        wheelSpeeds[0] * MOTOR_1_MULTIPLIER * 255,
        wheelSpeeds[2] * MOTOR_2_MULTIPLIER * 255,
        wheelSpeeds[1] * MOTOR_3_MULTIPLIER * 255,
        wheelSpeeds[3] * MOTOR_4_MULTIPLIER * 255
    );
    motors.flush();

    Serial.print("Wheel speeds: ");
    for (int i = 0; i < 4; i++) {
        Serial.print(wheelSpeeds[i]);
        Serial.print(" ");
    }
    Serial.println();

    delay(100);
}