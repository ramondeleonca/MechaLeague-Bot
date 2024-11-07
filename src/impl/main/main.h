#include <Arduino.h>
#include <constants.h>
#include <lib/hal/MechaLeagueMotorController.h>
#include <lib/drive/ChassisSpeeds.h>
#include <lib/drive/MecanumDriveKinematics.h>
#include <impl/main/drivetrain.h>
#include <impl/main/controller.h>
#include <QMC5883LCompass.h>
#include <Wire.h>

// Chassis
MechaLeagueMotorController motors(MOTOR_CONTROLLER_RX_PIN, MOTOR_CONTROLLER_TX_PIN, MOTOR_CONTROLLER_ENABLE_PIN);
Drivetrain drivetrain(&motors);
ChassisSpeeds chassisSpeeds;

// Gyroscope
QMC5883LCompass compass;

void setup() {
    // Serial
    Serial.begin(115200);

    // I2C
    Wire.begin();

    // Gyroscope
    compass.init();
    compass.setMode(0x01, 0x08, 0x00, 0x40);
    compass.setSmoothing(10, false);
}

void loop() {
}