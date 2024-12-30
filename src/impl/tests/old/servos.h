#include <Arduino.h>
#include <constants.h>
#include <lib/hal/MechaLeagueMotorController.h>

MechaLeagueMotorController motors(MOTOR_CONTROLLER_RX_PIN, MOTOR_CONTROLLER_TX_PIN, MOTOR_CONTROLLER_ENABLE_PIN);

void setup() {
    motors.enable();
}

void loop() {
    motors.setServo(MLServoId::S1, 0);
    motors.setServo(MLServoId::S2, 0);

    delay(1000);

    motors.setServo(MLServoId::S1, 180);
    motors.setServo(MLServoId::S2, 180);

    delay(1000);
}