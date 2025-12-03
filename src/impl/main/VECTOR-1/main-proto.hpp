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
#include <PS4Controller.h>
#include <lib/mechalib/motor/MOTOR_CONTROLLER_DRV8871.h>
#include <lib/mechalib/motor/MOTOR_CONTROLLER_L298N.h>
#include <HardwareSerial.h>
#include <DFRobotDFPlayerMini.h>
#include <ps5Controller.h>
#include <ESP32Servo.h>
#include <lib/utility/hooks.h>

const int buzzerPin = 33;
const int buzzerFreq = 1000;

enum SOUNDS {
    SOUND_VICTOR = 1,
    SOUND_HORN = 2,
    SOUND_WINDOWS = 3,
    SOUND_SMASNUG = 4,
    SOUND_METALPIPE = 5
};

const int PLAYER_RX = 18;
const int PLAYER_TX = 19;
const int playerVolume = 20;
const int playerEq = DFPLAYER_EQ_NORMAL;
HardwareSerial playerSerial(1);
DFRobotDFPlayerMini player;
UseCallback playVictor([](bool last, bool current) {
    player.volume(playerVolume);
    if (current) player.play(SOUND_VICTOR);
});
UseCallback playHorn([](bool last, bool current) {
    if (current) {
        player.volume(30);
        player.play(SOUND_HORN);
    } else {
        player.stop();
        player.volume(playerVolume);
    }
});
UseCallback playSmasnug([](bool last, bool current) {
    if (current) {
        player.play(SOUND_SMASNUG);
    }
});
UseCallback playMetalPipe([](bool last, bool current) {
    if (current) {
        player.play(SOUND_METALPIPE);
    }
});
UseCallback resetPlayer([](bool last, bool current) {
    if (current) {
        player.reset();
    }
});

// Motors
MOTOR_CONTROLLER_L298N frontLeft(27, 26, 25, 0.15);
MOTOR_CONTROLLER_L298N frontRight(2, 4, 15, 0.15);
MOTOR_CONTROLLER_L298N backLeft(12, 14, 13, 0.15);
MOTOR_CONTROLLER_L298N backRight(16, 17, 5, 0.15);
MOTOR_CONTROLLER_DRV8871 climberMotor(32, 33, 0.2);

// Servos
Servo lowerJoint;
// Chassis
float wheelSpeeds[4];
MecanumDriveKinematics kinematics(
  60.0 / 1000,
  200.0 / 1000,
  200.0 / 1000
);
ChassisSpeeds chassisSpeeds;

const char *controllerMAC = "90:b6:85:5a:94:ce";
// const char *controllerMAC = "8A:4F:4A:16:25:FE";
// const char *controllerMAC = "8a:4f:4a:16:25:fe";

void setup() {
    // Initialize motors (VERY IMPORTANT TO DO THIS VERY FIRST)
    frontLeft.begin();
    frontRight.begin();
    backLeft.begin();
    backRight.begin();
    climberMotor.begin();

    playerSerial.begin(9600, SERIAL_8N1, PLAYER_RX, PLAYER_TX);
    player.begin(playerSerial);
    player.EQ(playerEq);
    player.volume(playerVolume);
    player.play(SOUND_WINDOWS);

    ps5.attachOnConnect([]() {
        player.play(SOUND_SMASNUG);
    });

    ps5.attachOnDisconnect([]() {
        player.play(SOUND_METALPIPE);
    });

    // Allocate pwm timers for servo
    ESP32PWM::allocateTimer(0);
    ESP32PWM::allocateTimer(1);
    ESP32PWM::allocateTimer(2);
    ESP32PWM::allocateTimer(3);

    lowerJoint.attach(23);

    pinMode(buzzerPin, OUTPUT);
    noTone(buzzerPin);

    // Set all motors to brake
    frontLeft.setIdleMode(MOTOR_CONTROLLER_L298N::L298N_IdleMode::kBrake);
    frontRight.setIdleMode(MOTOR_CONTROLLER_L298N::L298N_IdleMode::kBrake);
    backLeft.setIdleMode(MOTOR_CONTROLLER_L298N::L298N_IdleMode::kBrake);
    backRight.setIdleMode(MOTOR_CONTROLLER_L298N::L298N_IdleMode::kBrake);
    climberMotor.setIdleMode(MOTOR_CONTROLLER_DRV8871::DRV8871_IdleMode::kBrake);

    // Begin serial
    Serial.begin(115200);

    // Initialize PS4 controller
    ps5.begin(controllerMAC);

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
    // Update chassis speeds
    float xSpeed = 0;
    float ySpeed = 0;
    float rotSpeed = 0;

    if (ps5.isConnected()) {
        xSpeed = (-(float)ps5.LStickY() / 127.0) * 1.5 ; // Meters per second
        ySpeed = ((float)ps5.LStickX() / 127.0) * 1.75; // Meters per second
        rotSpeed = -(((float)ps5.L2Value() - (float)ps5.R2Value()) / 127.0) * 2; // Radians per second

        chassisSpeeds.set(xSpeed, ySpeed, rotSpeed);
        Serial.println(chassisSpeeds.toString());

        // Update climber motor
        // float climberSpeed = ((float)ps5.RStickX() / 100) * 0.5; // Max Voltage used to calculate vbat percentage
        // climberMotor.set(climberSpeed);

        // Update arm servos
        if (ps5.L1()) {
            lowerJoint.write(150);
        } else if (ps5.R1()) {
            lowerJoint.write(180);
        } else if (ps5.R3()) {
            lowerJoint.write(map(ps5.RStickY(), -127, 127, 0, 180));
        }
    } else {
        chassisSpeeds.set(0, 0, 0);
    }

    // Update wheel speeds
    kinematics.calculateWheelSpeeds(wheelSpeeds, xSpeed, ySpeed, rotSpeed);
    kinematics.normalizeWheelSpeeds(wheelSpeeds);
    frontLeft.set(wheelSpeeds[0]);
    frontRight.set(wheelSpeeds[1]);
    backLeft.set(wheelSpeeds[2]);
    backRight.set(wheelSpeeds[3]);

    // print wheel speeds
    for (int i = 0; i < 4; i++) {
        Serial.print(wheelSpeeds[i]);
        Serial.print(" ");
    }
    Serial.println();

    // Print RAM usage percent
    // Serial.print("RAM: ");
    // Serial.print((ESP.getFreeHeap() * 100) / ESP.getHeapSize());
    // Serial.println("%");

    playVictor.update(ps5.Square());
    playHorn.update(ps5.Triangle());
    playSmasnug.update(ps5.Left());
    playMetalPipe.update(ps5.Down());

    resetPlayer.update(ps5.Options());

    delay(20);
}