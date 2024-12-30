#include <Arduino.h>
#include <ESP32Servo.h>

Servo servo;
Servo servo2;

void setup() {
    ESP32PWM::allocateTimer(0);
    ESP32PWM::allocateTimer(1);
    ESP32PWM::allocateTimer(2);
    ESP32PWM::allocateTimer(3);

    servo.setTimerWidth(20);
    servo.setPeriodHertz(50);
    servo.attach(2, 1000, 2000);

    servo2.setTimerWidth(20);
    servo2.setPeriodHertz(50);
    servo2.attach(15, 1000, 2000);
}

void loop() {
    servo.write(0);
    servo2.write(0);
    delay(5000);
    servo.write(180);
    servo2.write(180);
    delay(5000);
}