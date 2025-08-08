#include <PCA9685.h>
#include <Arduino.h>
#include <Wire.h>

PCA9685 driver(0x40);

void setup() {
    Serial.begin(115200);
    Wire.begin();

    driver.resetDevices();
    driver.setPWMFrequency(100);
    driver.setChannelPWM(0, 2048);
    driver.setChannelPWM(1, 0);

}

void loop() {
    
    // Serial.println("PWM signal sent");
}