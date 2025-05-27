#include <Arduino.h>
#include <constants.h>
#include <lib/hal/MechaLeagueMotorController.h>
#include <lib/drive/ChassisSpeeds.h>
#include <lib/drive/MecanumDriveKinematics.h>
#include <Wire.h>
#include <lib/control/JoystickUtils.h>
#include <lib/utility/hooks.h>
#include <ESP32Servo.h>
#include <lib/utility/math.h>
#include <lib/hal/AllianceLED.h>
#include <../lib/ps5-esp32-main/src/ps5Controller.h>
#include <lib/mechalib/motor/MOTOR_CONTROLLER_DRV8871.h>
#include <ESP32Servo.h>

// Motors
MOTOR_CONTROLLER_DRV8871 frontLeft(25, 26, 0.15);
MOTOR_CONTROLLER_DRV8871 frontRight(13, 12, 0.15);
MOTOR_CONTROLLER_DRV8871 backLeft(2, 4, 0.15);
MOTOR_CONTROLLER_DRV8871 backRight(18, 19, 0.15);
MOTOR_CONTROLLER_DRV8871 climberMotor(32, 33, 0.2);

// Servos
Servo lowerJoint;
Servo upperJoint;

struct ArmSetpoint {
  int joint1;
  int joint2;
};

namespace ArmSetpoints {
  constexpr ArmSetpoint PICKUP = {160, 0};
  constexpr ArmSetpoint STOW = {60, 0};
  constexpr ArmSetpoint SCORE = {120, 60};
}

// Chassis
float wheelSpeeds[4];
MecanumDriveKinematics kinematics(WHEEL_DIAMETER_MM / 1000, TRACK_WIDTH_MM / 1000, WHEEL_BASE_MM / 1000);
ChassisSpeeds chassisSpeeds;

const char *controllerMAC = "90:b6:85:5a:94:ce";

void setup() {
    // Initialize motors (VERY IMPORTANT TO DO THIS VERY FIRST)
    frontLeft.begin();
    frontRight.begin();
    backLeft.begin();
    backRight.begin();
    climberMotor.begin();

    // Allocate pwm timers for servo
    ESP32PWM::allocateTimer(0);
    ESP32PWM::allocateTimer(1);
    ESP32PWM::allocateTimer(2);
    ESP32PWM::allocateTimer(3);

    lowerJoint.attach(14);
    upperJoint.attach(27);

    // Set all motors to brake
    frontLeft.setIdleMode(MOTOR_CONTROLLER_DRV8871::DRV8871_IdleMode::kBrake);
    frontRight.setIdleMode(MOTOR_CONTROLLER_DRV8871::DRV8871_IdleMode::kBrake);
    backLeft.setIdleMode(MOTOR_CONTROLLER_DRV8871::DRV8871_IdleMode::kBrake);
    backRight.setIdleMode(MOTOR_CONTROLLER_DRV8871::DRV8871_IdleMode::kBrake);
    climberMotor.setIdleMode(MOTOR_CONTROLLER_DRV8871::DRV8871_IdleMode::kBrake);

    // Begin serial
    Serial.begin(115200);

    // Initialize PS5 controller
    ps5.begin(controllerMAC);
    ps5.attachOnConnect([]() {
        Serial.println("PS5 connected");
        ps5.setLed(255, 165, 0);
    });

    frontLeft.set(1, true);
    frontRight.set(1, true);
    climberMotor.set(0.5, true);
    delay(75);
    frontLeft.set(0, true);
    frontRight.set(0, true);
    climberMotor.set(0, true);
    delay(75);
    frontLeft.set(1, true);
    frontRight.set(1, true);
    climberMotor.set(0.5, true);
    delay(75);
    frontLeft.set(0, true);
    frontRight.set(0, true);
    climberMotor.set(0, true);

    MotorSafety::getInstance().enable();
}

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
        float xSpeed = ((float)ps5.LStickY() / 127.0) * 0.5; // Meters per second
        float ySpeed = (-(float)ps5.LStickX() / 127.0) * 0.75; // Meters per second
        float rotSpeed = (((float)ps5.L2Value() - (float)ps5.R2Value()) / 127.0); // Radians per second

        chassisSpeeds.set(xSpeed, ySpeed, rotSpeed);

        Serial.println(chassisSpeeds.toString());

        // Update climber motor
        float climberSpeed = ((float)ps5.RStickY() / 100) * 0.5; // Max Voltage used to calculate vbat percentage
        climberMotor.set(climberSpeed);

        // Update arm servos
        if (ps5.L1()) {
            lowerJoint.write(ArmSetpoints::PICKUP.joint1);
            upperJoint.write(ArmSetpoints::PICKUP.joint2);
        } else if (ps5.R1()) {
            lowerJoint.write(ArmSetpoints::SCORE.joint1);
            upperJoint.write(ArmSetpoints::SCORE.joint2);
        } else {
          lowerJoint.write(ArmSetpoints::STOW.joint1);
          upperJoint.write(ArmSetpoints::STOW.joint2);
        }
    } else {
        chassisSpeeds.set(0, 0, 0);
        // ps5.begin(controllerMAC);
    }

    // Update wheel speeds
    kinematics.calculateWheelSpeeds(wheelSpeeds, chassisSpeeds.getVx(), chassisSpeeds.getVy(), chassisSpeeds.getOmega());
    kinematics.normalizeWheelSpeeds(wheelSpeeds);
    frontLeft.set(wheelSpeeds[0] * 1);
    frontRight.set(wheelSpeeds[1] * 0.65);
    backLeft.set(wheelSpeeds[3] * 0.65);
    backRight.set(wheelSpeeds[2] * 1.3);

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

    delay(20);
}