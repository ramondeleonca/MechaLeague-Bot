#include <Arduino.h>
#include <constants.h>
#include <impl/tests/remotexy/defs.h>
#include <lib/hal/MechaLeagueMotorController.h>
#include <lib/drive/ChassisSpeeds.h>
#include <lib/drive/MecanumDriveKinematics.h>
#include <Wire.h>
#include <QMC5883LCompass.h>
#include <WS2812FX.h>

// Drivetrain
MechaLeagueMotorController motors(MOTOR_CONTROLLER_RX_PIN, MOTOR_CONTROLLER_TX_PIN, MOTOR_CONTROLLER_ENABLE_PIN);
float wheelSpeeds[4];
MecanumDriveKinematics kinematics(WHEEL_DIAMETER_MM / 1000, TRACK_WIDTH_MM / 1000, WHEEL_BASE_MM / 1000);
ChassisSpeeds chassisSpeeds;

// Compass
QMC5883LCompass compass;

// LEDS
WS2812FX leds(LED_COUNT, LED_PIN, NEO_GRB + NEO_KHZ800);