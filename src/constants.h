#ifndef CONSTANTS_H
#define CONSTANTS_H

#include <Arduino.h>

// ! TEAM NUMBER
const int TEAM_NUMBER = -1;

// * NEW CONSTANTS
// Motor controller
const int MOTOR_CONTROLLER_RX_PIN = 16;
const int MOTOR_CONTROLLER_TX_PIN = 17;
const int MOTOR_CONTROLLER_ENABLE_PIN = 2;
const float MOTOR_1_MULTIPLIER = 1.0;
const float MOTOR_2_MULTIPLIER = 1.0;
const float MOTOR_3_MULTIPLIER = 1.0;
const float MOTOR_4_MULTIPLIER = 1.0;

// Servos
const int SERVO_L_PIN = 19;
const int SERVO_R_PIN = 23;

const int SERVO_L_UP_POSITION = 190;
const int SERVO_L_DOWN_POSITION = 0;

const int SERVO_R_UP_POSITION = 0;
const int SERVO_R_DOWN_POSITION = 188;

// AllianceLED
const int ALLIANCE_LED_BLUE_PIN = 5;
const int ALLIANCE_LED_RED_PIN = 18;

// Buzzer
const int BUZZER_PIN = 15;

// Battery
const int BATTERY_PIN = 34;
const float BATTERY_R1 = 9530.0;  // Resistor value in ohms
const float BATTERY_R2 = 2240.0;   // Resistor value in ohms

// Controller
const float CONTROLLER_DEADBAND = 0.05;
const int CONTROLLER_LOG_BASE = 10;

// LEDs
const uint LED_PIN = 4;
const uint LED_COUNT = 30;

// * OLD CONSTANTS
// Communication
const uint REST_SERVER_PORT = 80;
const uint WS_SERVER_PORT = 81;
const String SSID = "MechaLeague-Bot_" + String(TEAM_NUMBER);
// ! PASSWORD IS CHANGED INTERNALLY
const String PASSWORD = "blue3526";
const String HOSTNAME = "mechaleague-bot";
const uint WIFI_CHANNEL = 11;

const int M1_DIRA_PIN = 25; //cafe
const int M1_DIRB_PIN = 26; //negro
const int M1_VELO_PIN = 12; //rojo
const bool M1_INVERTED = false;

const int M2_DIRA_PIN = 14; //gris
const int M2_DIRB_PIN = 27; //blanco
const int M2_VELO_PIN = 13; //morado
const bool M2_INVERTED = true;

const int M3_DIRA_PIN = 19; //cafe
const int M3_DIRB_PIN = 23; //negro
const int M3_VELO_PIN = 32; //rojo
const bool M3_INVERTED = true;

const int M4_DIRA_PIN = 5; //gris
const int M4_DIRB_PIN = 18; //blanco
const int M4_VELO_PIN = 4; //morado
const bool M4_INVERTED = false;

const float WHEEL_DIAMETER_MM = 60.0;

const float TRACK_WIDTH_MM = 140.0;
const float WHEEL_BASE_MM = 250.0;

const float SOUND_SPEED = 0.034;

#endif