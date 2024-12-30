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

    // Begin each encoder
    for (int i = 0; i < 4; i++) {
        mux.exposeChannel(i);
        encoder.begin(false);
        mux.hideChannel(i);
    }
}

void loop() {
    float val1, val2, val3, val4;

    mux.exposeChannel(0);
    val1 = encoder.getAbsoluteAngle() * AS5600_RAW_TO_DEGREES;
    mux.hideChannel(0);

    mux.exposeChannel(1);
    val2 = encoder.getAbsoluteAngle() * AS5600_RAW_TO_DEGREES;
    mux.hideChannel(1);

    mux.exposeChannel(2);
    val3 = encoder.getAbsoluteAngle() * AS5600_RAW_TO_DEGREES;
    mux.hideChannel(2);

    mux.exposeChannel(3);
    val4 = encoder.getAbsoluteAngle() * AS5600_RAW_TO_DEGREES;
    mux.hideChannel(3);

    Serial.print("Val1: " + String(val1) + ", ");
    Serial.print("Val2: " + String(val2) + ", ");
    Serial.print("Val3: " + String(val3) + ", ");
    Serial.print("Val4: " + String(val4));
    Serial.println();

    delay(100);
}