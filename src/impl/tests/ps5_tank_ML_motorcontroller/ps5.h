#include <Arduino.h>
#include <constants.h>
#include <lib/hal/MechaLeagueMotorController.h>
#include <lib/drive/ChassisSpeeds.h>
#include <lib/drive/MecanumDriveKinematics.h>
#include <Wire.h>
#include <WS2812FX.h>
#include <lib/control/JoystickUtils.h>
#include <lib/utility/hooks.h>
#include <ESP32Servo.h>
#include <lib/utility/math.h>
#include <lib/hal/AllianceLED.h>
#include <ps5Controller.h>
#include <lib/mechalib/motor/MOTOR_CONTROLLER_DRV8871.h>
#include <lib/mechalib/motor/MOTOR_CONTROLLER_L298N.h>

// Motors
MechaLeagueMotorController motors;

float wheelSpeeds[4];
MecanumDriveKinematics kinematics(WHEEL_DIAMETER_MM / 1000, TRACK_WIDTH_MM / 1000, WHEEL_BASE_MM / 1000);
ChassisSpeeds chassisSpeeds;

const char *controllerMAC = "90:b6:85:5a:94:ce";

void setup() {
    // Initialize motors (VERY IMPORTANT TO DO THIS VERY FIRST)
    motors.enable();

    // Begin serial
    Serial.begin(115200);

    // Initialize RemoteXY
    ps5.begin(controllerMAC);
}

const float deadband = 0.1;
void loop() {
    if (ps5.LStickX()) {
      Serial.printf("Left Stick x at %d\n", ps5.LStickX());
    }
    if (ps5.LStickY()) {
      Serial.printf("Left Stick y at %d\n", ps5.LStickY());
    }
    if (ps5.RStickX()) {
      Serial.printf("Right Stick x at %d\n", ps5.RStickX());
    }
    if (ps5.RStickY()) {
      Serial.printf("Right Stick y at %d\n", ps5.RStickY());
    }
    
    // Update chassis speeds
    if (ps5.isConnected()) {
        float xSpeed = ((float)ps5.LStickY() / 127.0) * 1; // Meters per second
        float ySpeed = (-(float)ps5.LStickX() / 127.0) * 0; // Meters per second
        float rotSpeed = (-(float)ps5.RStickX() / 127.0) * 2.25; // Radians per second

        xSpeed = abs(xSpeed) > deadband ? xSpeed : 0;
        ySpeed = abs(ySpeed) > deadband ? ySpeed : 0;
        rotSpeed = abs(rotSpeed) > deadband ? rotSpeed : 0;

        chassisSpeeds.set(xSpeed, ySpeed, rotSpeed);

        Serial.println(chassisSpeeds.toString());
    } else {
        chassisSpeeds.set(0, 0, 0);
        // ps5.begin(controllerMAC);
    }

    // Update wheel speeds
    kinematics.calculateWheelSpeeds(wheelSpeeds, chassisSpeeds.getVx(), chassisSpeeds.getVy(), chassisSpeeds.getOmega());
    kinematics.normalizeWheelSpeeds(wheelSpeeds);
    motors.setMotors(
      wheelSpeeds[0] * 255,
      wheelSpeeds[2] * 255,
      wheelSpeeds[3] * 255,
      wheelSpeeds[1] * 255
    );
    motors.flush();

    // print wheel speeds
    for (int i = 0; i < 4; i++) {
        Serial.print(wheelSpeeds[i]);
        Serial.print(" ");
    }
    Serial.println();

    // Print RAM usage percent
    Serial.print("RAM: ");
    Serial.print((ESP.getFreeHeap() * 100) / ESP.getHeapSize());
    Serial.println("%");

    delay(100);
}