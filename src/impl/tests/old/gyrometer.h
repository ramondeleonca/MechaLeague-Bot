#include <Arduino.h>
#include <lib/hal/Gyrometer.cpp>

Gyrometer gyro;

void setup() {
    Serial.begin(115200);

    gyro.begin();
}

void loop() {
    gyro.update();
    
    Serial.println("Yaw: " + String(gyro.getYaw()));
    Serial.println("Mag: " + String(gyro.compass->getAzimuth()));

    delay(100);
}