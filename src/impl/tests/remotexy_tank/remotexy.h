#include <Arduino.h>
#include <constants.h>
#include <lib/drive/ChassisSpeeds.h>
#include <impl/tests/remotexy/defs.h>
#include <lib/mechalib/motor/MOTOR_CONTROLLER_DRV8871.h>
#include <lib/mechalib/motor/MotorSafety.h>
#include <ESP32PWM.h>

ChassisSpeeds chassisSpeeds;
MOTOR_CONTROLLER_DRV8871 frontLeft(25, 26, 0.15);
MOTOR_CONTROLLER_DRV8871 frontRight(13, 12, 0.15);
MOTOR_CONTROLLER_DRV8871 backLeft(2, 4, 0.15);
MOTOR_CONTROLLER_DRV8871 backRight(18, 19, 0.15);
MOTOR_CONTROLLER_DRV8871 climberMotor(32, 33, 0.15);

void setup() {
    // Initialize motors (VERY IMPORTANT TO DO THIS VERY FIRST)
    frontLeft.begin();
    frontRight.begin();
    backLeft.begin();
    backRight.begin();

    // Begin serial
    Serial.begin(115200);

    // Initialize RemoteXY
    RemoteXY_Init();

    // Set up followers
    frontLeft.addFollower(&backLeft);
    frontRight.addFollower(&backRight);

    frontLeft.set(1, true);
    frontRight.set(1, true);
    delay(75);
    frontLeft.set(0, true);
    frontRight.set(0, true);
    delay(75);
    frontLeft.set(1, true);
    frontRight.set(1, true);
    delay(75);
    frontLeft.set(0, true);
    frontRight.set(0, true);
    

    delay(1000);

    MotorSafety::getInstance().enable();
}

float mapJoystick(float value) {
    return value / 100;
}

float processJoystick(float value) {
    return mapJoystick(value);
}

void loop() {
    // Handle RemoteXY events
    RemoteXY_Handler();

    // Update chassis speeds
    if (RemoteXY.connect_flag) {
        float ySpeed = processJoystick(RemoteXY.lefty); // Meters per second
        float rotSpeed = processJoystick(RemoteXY.rightx) * 0.5; // Radians per second

        chassisSpeeds.set(0, ySpeed, rotSpeed);
    } else {
        chassisSpeeds.set(0, 0, 0);
    }

    // Update wheel speeds
    float leftSpeed = chassisSpeeds.getVy() - chassisSpeeds.getOmega();
    float rightSpeed = chassisSpeeds.getVy() + chassisSpeeds.getOmega();

    frontLeft.set(leftSpeed);
    frontRight.set(rightSpeed);

    delay(20);
}