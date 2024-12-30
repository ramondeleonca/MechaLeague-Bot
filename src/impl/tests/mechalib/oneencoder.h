#include <Arduino.h>
#include <Wire.h>
#include <lib/mechalib/i2c/I2C_MUX_TCA9548A.h>
#include <lib/mechalib/encoders/ENCODER_I2C_AS5600.h>

I2C_MUX_TCA9548A mux(&Wire, 0x70);
ENCODER_AS5600 encoder;

void setup() {
    Serial.begin(115200);

    Wire.begin();
    mux.begin(false);

    mux.exposeChannel(0);
    encoder.begin(false);
}

void loop() {
    Serial.print(encoder.getAbsoluteAngle());
    Serial.println();

    delay(100);
}