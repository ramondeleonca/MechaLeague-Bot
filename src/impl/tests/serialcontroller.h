#include <Arduino.h>
#define MECHALEAGUE_CREATE_MOTOR_CONTROLLER_INSTANCE
#include <lib/hal/MechaLeagueMotorController.h>

void setup() {
    Serial.begin(115200);
    motorControllerInstance.enable();
}

void loop() {
    while (Serial.available() > 0) {
        motorControllerInstance.sendRaw(Serial.readStringUntil('\n'));
    }
}