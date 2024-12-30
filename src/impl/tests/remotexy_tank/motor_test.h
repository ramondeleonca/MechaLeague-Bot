#include <Arduino.h>
#include <constants.h>
#include <lib/drive/ChassisSpeeds.h>
#include <lib/mechalib/motor/MOTOR_CONTROLLER_DRV8871.h>
#include <lib/mechalib/motor/MotorSafety.h>
#include <ESP32PWM.h>

ChassisSpeeds chassisSpeeds;
MOTOR_CONTROLLER_DRV8871 frontLeft(14, 27, 0.2);
MOTOR_CONTROLLER_DRV8871 frontRight(13, 12, 0.2);
MOTOR_CONTROLLER_DRV8871 backLeft(2, 4, 0.2);
MOTOR_CONTROLLER_DRV8871 backRight(18, 19, 0.2);

void setup() {
    // Begin serial
    Serial.begin(115200);

    // Initialize motors
    frontLeft.begin();
    frontRight.begin();
    backLeft.begin();
    backRight.begin();

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
    
    MotorSafety::getInstance().enable();

    // write test value
    frontLeft.set(0.25);
    frontRight.set(0.25);
}

void loop() {
    frontLeft.set(0.25);
    frontRight.set(0.25);
    delay(1000);
    frontLeft.set(-0.25);
    frontRight.set(-0.25);
    delay(1000);
}