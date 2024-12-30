#include <Arduino.h>
#include <lib/mechalib/comms/comms.h>

void setup() {
    Serial.begin(115200);
    Serial.println("Starting MechaLib");
    COMMS_CONN_ERR err = Comms::getInstance().connectToField();
    if (err == COMMS_SUCCESS) {
        Serial.println("Connected to field");
    } else {
        Serial.println("Failed to connect to field");
    }
    Serial.println(err);
}

void loop() {

}