#include <Arduino.h>
#include <constants.h>
#include <lib/hal/MechaLeagueMotorController.h>
#include <lib/drive/ChassisSpeeds.h>
#include <lib/drive/MecanumDriveKinematics.h>
#include <BluetoothSerial.h>

BluetoothSerial bt;

MechaLeagueMotorController motors(MOTOR_CONTROLLER_RX_PIN, MOTOR_CONTROLLER_TX_PIN, MOTOR_CONTROLLER_ENABLE_PIN);
float wheelSpeeds[4];
MecanumDriveKinematics kinematics(WHEEL_DIAMETER_MM / 1000, TRACK_WIDTH_MM / 1000, WHEEL_BASE_MM / 1000);
ChassisSpeeds chassisSpeeds;

void setup() {
    Serial.begin(115200);
    bt.begin("LA CHACALOSA");
    pinMode(2, OUTPUT);
    digitalWrite(2, HIGH);
}

String lastVal;
void loop() {
    if (bt.available()) {
        String input = bt.readStringUntil('#');

        if (input != lastVal) {
            if (input == "%A") {  // Move Forward
                chassisSpeeds.set(0.75, 0, 0); // vx = 0.75 (max forward), vy = 0, omega = 0
            } else if (input == "%B") {  // Move Backward
                chassisSpeeds.set(-0.75, 0, 0); // vx = -0.75 (max backward), vy = 0, omega = 0
            } else if (input == "%C") {  // Move Left
                chassisSpeeds.set(0, -0.75, 0); // vx = 0, vy = -0.75 (max left), omega = 0
            } else if (input == "%D") {  // Move Right
                chassisSpeeds.set(0, 0.75, 0); // vx = 0, vy = 0.75 (max right), omega = 0
            } else if (input == "%E") {  // Rotate Left
                chassisSpeeds.set(0, 0, -0.75); // vx = 0, vy = 0, omega = -0.75 (max rotation left)
            } else if (input == "%F") {  // Rotate Right
                chassisSpeeds.set(0, 0, 0.75); // vx = 0, vy = 0, omega = 0.75 (max rotation right)
            } else if (input == "%G") {  // Move Upper Left
                chassisSpeeds.set(0.75, -0.75, 0); // vx = 0.75, vy = -0.75 (diagonal upper left)
            } else if (input == "%H") {  // Move Upper Right
                chassisSpeeds.set(0.75, 0.75, 0); // vx = 0.75, vy = 0.75 (diagonal upper right)
            } else if (input == "%I") {  // Move Lower Left
                chassisSpeeds.set(-0.75, -0.75, 0); // vx = -0.75, vy = -0.75 (diagonal lower left)
            } else if (input == "%J") {  // Move Lower Right
                chassisSpeeds.set(-0.75, 0.75, 0); // vx = -0.75, vy = 0.75 (diagonal lower right)
            } else if (input == "%K") {  // Drift Left
                chassisSpeeds.set(0.5, -0.75, 0); // vx = 0.5 (forward), vy = -0.75 (left drift), omega = 0
            } else if (input == "%L") {  // Drift Right
                chassisSpeeds.set(0.5, 0.75, 0); // vx = 0.5 (forward), vy = 0.75 (right drift), omega = 0
            } else if (input == "%S") {  // Stop
                chassisSpeeds.set(0, 0, 0); // Stop movement
            }
            lastVal = input;
        }

        Serial.println(input);
    }

    kinematics.calculateWheelSpeeds(wheelSpeeds, chassisSpeeds.getVx(), chassisSpeeds.getVy(), chassisSpeeds.getOmega());
    kinematics.normalizeWheelSpeeds(wheelSpeeds);  
    motors.setMotors(
        wheelSpeeds[0] * MOTOR_1_MULTIPLIER * 255,
        wheelSpeeds[2] * MOTOR_2_MULTIPLIER * 255,
        wheelSpeeds[1] * MOTOR_3_MULTIPLIER * 255,
        wheelSpeeds[3] * MOTOR_4_MULTIPLIER * 255
    );
    motors.flush();

    Serial.print("Wheel speeds: ");
    for (int i = 0; i < 4; i++) {
        Serial.print(wheelSpeeds[i]);
        Serial.print(" ");
    }
    Serial.println();

    delay(100);
}