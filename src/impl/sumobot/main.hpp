#include <Arduino.h>
#include <lib/mechalib/motor/MOTOR_CONTROLLER_L298N.h>
#include <impl/sumobot/remoteconfig.hpp>

MOTOR_CONTROLLER_L298N leftMotor(0, 0, 0);
MOTOR_CONTROLLER_L298N rightMotor(0, 0, 0);

void setup() {
    // Initialize motors
    leftMotor.begin();
    rightMotor.begin();

    // Enable motors
    MotorSafety::getInstance().enable();

    // Print initialization message
    Serial.begin(115200);
    Serial.println("Sumobot enabled.");

    // Initialize RemoteXY
    RemoteXY_Init();
}

void loop() {
    RemoteXY_Handler();

    leftMotor.set(RemoteXY.lefty / 100.0);
    rightMotor.set(RemoteXY.righty / 100.0);
}