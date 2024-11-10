#include <Arduino.h>
#include <constants.h>
#include <lib/hal/MechaLeagueMotorController.h>
#include <lib/drive/ChassisSpeeds.h>
#include <lib/drive/MecanumDriveKinematics.h>
#include <impl/tests/remotexy/defs.h>
#include <Wire.h>
#include <WS2812FX.h>
#include <lib/hal/Gyrometer.cpp>
#include <lib/control/JoystickUtils.h>
#include <lib/utility/hooks.h>
#include <ESP32Servo.h>

MechaLeagueMotorController motors(MOTOR_CONTROLLER_RX_PIN, MOTOR_CONTROLLER_TX_PIN, MOTOR_CONTROLLER_ENABLE_PIN);
float wheelSpeeds[4];
MecanumDriveKinematics kinematics(WHEEL_DIAMETER_MM / 1000, TRACK_WIDTH_MM / 1000, WHEEL_BASE_MM / 1000);
ChassisSpeeds chassisSpeeds;

// LEDS
WS2812FX leds(LED_COUNT, LED_PIN, NEO_GRB + NEO_KHZ800);

// Hooks
UseCallback servoStateCallback;

Servo servo1;
Servo servo2;

float getVbat() {
    int val = analogRead(BATTERY_PIN);
    float vin = val * (3.5 / 4095.0);
    float vbat = vin / (BATTERY_R2 / (BATTERY_R1 + BATTERY_R2));
    return vbat;
}

String buildUpperMessage(float vbat) {
    String message = "Vbat: " + String(vbat) + "V";
    return message;
}

TaskHandle_t ledTaskHandle;
void ledTask(void *pvParameters) {
    for (;;) {
        leds.service();
        // vTaskDelay(200 / portTICK_PERIOD_MS);
        delay(20);
    }
}

Gyrometer gyro;
TaskHandle_t gyroTaskHandle;
void gyroTask(void *pvParameters) {
    for (;;) {
        gyro.update();
        delay(10);
    }
}

void setup() {
    Serial.begin(115200);

    // Initialize RemoteXY
    RemoteXY_Init();

    // Initialize IMU
    pinMode(15, OUTPUT);
    digitalWrite(15, LOW);

    pinMode(BATTERY_PIN, INPUT_PULLDOWN);

    // start leds
    leds.init();
    leds.setBrightness(128);
    leds.setSpeed(100);
    leds.setColor(0xFF0000);
    leds.setMode(FX_MODE_STATIC);
    leds.start();
    xTaskCreate(ledTask, "ledTask", 2048, NULL, 1, &ledTaskHandle);

    // start gyro
    gyro.begin();
    xTaskCreate(gyroTask, "gyroTask", 2048, NULL, 1, &gyroTaskHandle);

    pinMode(2, OUTPUT);
    digitalWrite(2, HIGH);

    ESP32PWM::allocateTimer(0);
	ESP32PWM::allocateTimer(1);
	ESP32PWM::allocateTimer(2);
	ESP32PWM::allocateTimer(3);

    servo1.setPeriodHertz(50);
    servo1.attach(SERVO_1_PIN, 400, 2400);

    servo2.setPeriodHertz(50);
    servo2.attach(SERVO_2_PIN, 400, 2400);

    servo1.write(95);
    servo2.write(0);

    servoStateCallback.setCallback([](bool last, bool current) {
        if (RemoteXY.fieldOriented) {
            servo1.write(95);
            servo2.write(0);
        } else {
            servo1.write(0);
            servo2.write(95);
        }
    }); 
}

ulong lastTime;
void loop() {
    // Track time
    ulong currentTime = millis();
    ulong deltaTime = currentTime - lastTime;

    // Handle RemoteXY events
    RemoteXY_Handler();

    servoStateCallback.update(RemoteXY.fieldOriented == 1);

    float heading = gyro.getYaw();

    // Update chassis speeds
    if (RemoteXY.connect_flag) {
        float xSpeed = RemoteXY.lefty / 100.0 * 0.5; // Meters per second
        float ySpeed = -RemoteXY.leftx / 100.0 * 0.75; // Meters per second
        float rotSpeed = -RemoteXY.rightx / 100.0 * 1.25; // Radians per second

        chassisSpeeds.set(xSpeed, ySpeed, rotSpeed);
        
        if (abs(RemoteXY.leftx) > 10 || abs(RemoteXY.lefty) > 10 || abs(RemoteXY.rightx) > 10) {
            leds.setMode(FX_MODE_RAINBOW_CYCLE);
            leds.setSpeed(1000);
        } else {
            leds.setMode(FX_MODE_BREATH);
            leds.setSpeed(1);
            leds.setColor(0x0000FF);
        }
    } else {
        leds.setMode(FX_MODE_BREATH);
        leds.setSpeed(1);
        leds.setColor(0xFF0000); 
    }

    float vbat = getVbat();
    String upperMessage = buildUpperMessage(vbat);
    RemoteXY.message[0] = '\0';
    upperMessage.toCharArray(RemoteXY.message, 51);

    RemoteXY.heading = heading;

    // Update wheel speeds
    kinematics.calculateWheelSpeeds(wheelSpeeds, chassisSpeeds.getVx(), chassisSpeeds.getVy(), chassisSpeeds.getOmega());
    kinematics.normalizeWheelSpeeds(wheelSpeeds);  
    motors.setMotors(
        wheelSpeeds[0] * MOTOR_1_MULTIPLIER * 255,
        wheelSpeeds[2] * MOTOR_2_MULTIPLIER * 255,
        wheelSpeeds[3] * MOTOR_4_MULTIPLIER * 255,
        wheelSpeeds[1] * MOTOR_3_MULTIPLIER * 255
    );
    motors.flush();

    // Update telemetry
    // RemoteXY.m1 = wheelSpeeds[0];
    // RemoteXY.m2 = wheelSpeeds[1];
    // RemoteXY.m3 = wheelSpeeds[2];
    // RemoteXY.m4 = wheelSpeeds[3];

    Serial.print("Wheel speeds: ");
    for (int i = 0; i < 4; i++) {
        Serial.print(wheelSpeeds[i]);
        Serial.print(" ");
    }
    Serial.println();

    lastTime = currentTime;
    delay(100);
}