#include <Arduino.h>
#include <constants.h>
#include <lib/hal/L298N.cpp>
#include <lib/drive/ChassisSpeeds.h>
#include <lib/drive/MecanumDriveKinematics.h>

L298N motor1(M1_VELO_PIN, M1_DIRA_PIN, M1_DIRB_PIN);
L298N motor2(M2_VELO_PIN, M2_DIRA_PIN, M2_DIRB_PIN);
L298N motor3(M3_VELO_PIN, M3_DIRA_PIN, M3_DIRB_PIN);
L298N motor4(M4_VELO_PIN, M4_DIRA_PIN, M4_DIRB_PIN);

float wheelSpeeds[4];
ChassisSpeeds speeds;
MecanumDriveKinematics kinematics(WHEEL_DIAMETER_MM, TRACK_WIDTH_MM, WHEEL_BASE_MM);

float distance = 0;
TaskHandle_t sensorTaskHandle;
void sensorTask(void* args) {
    while (true) {
        // Send a 10 µs pulse to trigger the sensor
        digitalWrite(ULTRASONIC_TRIG, LOW);
        delayMicroseconds(2);
        digitalWrite(ULTRASONIC_TRIG, HIGH);
        delayMicroseconds(10);
        digitalWrite(ULTRASONIC_TRIG, LOW);

        // Measure the time for the echo to return
        long duration = pulseIn(ULTRASONIC_PING, HIGH);

        // Calculate the distance in cm
        float distance = duration * 0.034 / 2;

        Serial.println(distance);
        delay(500);
    }
}

void setup() {
    Serial.begin(115200);

    // Configure motors
    motor1.setInverted(M1_INVERTED);
    motor2.setInverted(M2_INVERTED);
    motor3.setInverted(M3_INVERTED);
    motor4.setInverted(M4_INVERTED);

    // Begin sensor
    xTaskCreate(sensorTask, "Sensor Task", 10000, NULL, 1, &sensorTaskHandle);
}

void loop() {
    if (distance < ULTRASONIC_THRESHOLD) {
        Serial.println("Obstacle detected");
        wheelSpeeds[0] =  0.25;
        wheelSpeeds[1] = -0.25;
        wheelSpeeds[2] = -0.25;
        wheelSpeeds[3] =  0.25;
    } else {
        Serial.println("No obstacle detected");
        wheelSpeeds[0] = 0.3;
        wheelSpeeds[1] = 0.3;
        wheelSpeeds[2] = 0.3;
        wheelSpeeds[3] = 0.3;
    }

    // Update wheels
    // kinematics.calculateWheelSpeeds(wheelSpeeds, speeds.getVx(), speeds.getVy(), speeds.getOmega());
    // kinematics.normalizeWheelSpeeds(wheelSpeeds);
    motor1.set(wheelSpeeds[0]);
    motor2.set(wheelSpeeds[1]);
    motor3.set(wheelSpeeds[2]);
    motor4.set(wheelSpeeds[3]);

    // Print wheel speeds
    // for (int i = 0; i < 4; i++) {
    //     Serial.print(wheelSpeeds[i]);
    //     Serial.print(" ");
    // }
    // Serial.println();

    delay(200);
}
