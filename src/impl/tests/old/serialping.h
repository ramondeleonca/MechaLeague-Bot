#include <Arduino.h>

HardwareSerial controller(1);

void setup() {
    Serial.begin(115200);
    controller.begin(9600, SERIAL_8N1, 16, 17);
}

void loop() {
    while (controller.available() > 0) {
        Serial.write(controller.read());
    }
    while (Serial.available() > 0) {
        controller.write(Serial.read());
    }
}