#include <Arduino.h>
#include <PCA9685.h>
#include <Wire.h>

PCA9685 pwmController(0x40);                  // Library using default B000000 (A5-A0) i2c address, and default Wire @400kHz

void setup() {
    Serial.begin(115200);               // Begin Serial and Wire interfaces
    Wire.begin();

    pwmController.resetDevices();       // Resets all PCA9685 devices on i2c line

    pwmController.init();               // Initializes module using default totem-pole driver mode, and default disabled phase balancer

    pwmController.setPWMFrequency(100); // Set PWM freq to 100Hz (default is 200Hz, supports 24Hz to 1526Hz)
    
    for (int i = 0; i < 16; i++) {
        pwmController.setChannelPWM(i, 128 << 4); // Set all channels to 0
    }

    Serial.println(pwmController.getChannelPWM(0)); // Should output 2048, which is 128 << 4
}

void loop() {
}