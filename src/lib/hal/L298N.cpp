#ifndef L298N_CPP
#define L298N_CPP

#include <Arduino.h>

class L298N {
    private:
        bool inverted = false;
        uint8_t velo_pin;
        uint8_t dira_pin;
        uint8_t dirb_pin;

        float lastSpeed = 0.0;

    public:
        L298N(uint8_t velo_pin, uint8_t dira_pin, uint8_t dirb_pin) {
            this->velo_pin = velo_pin;
            this->dira_pin = dira_pin;
            this->dirb_pin = dirb_pin;
        
            pinMode(velo_pin, OUTPUT);
            pinMode(dira_pin, OUTPUT);
            pinMode(dirb_pin, OUTPUT);

            digitalWrite(velo_pin, LOW);
            digitalWrite(dira_pin, LOW);
            digitalWrite(dirb_pin, LOW);
        }

        void setInverted(bool inverted) {
            this->inverted = inverted;
        }

        void set(float speed) {
            // Invert speed if needed
            if (this->inverted) speed = -speed;

            // Debounce speed
            if (abs(speed - this->lastSpeed) < 0.01) return;

            // Set speed
            this->lastSpeed = speed;

            // Set direction
            if (speed > 0) {
                digitalWrite(this->dira_pin, HIGH);
                digitalWrite(this->dirb_pin, LOW);
            } else if (speed < 0) {
                digitalWrite(this->dira_pin, LOW);
                digitalWrite(this->dirb_pin, HIGH);
            } else {
                digitalWrite(this->dira_pin, LOW);
                digitalWrite(this->dirb_pin, LOW);
            }

            // Set speed
            analogWrite(this->velo_pin, abs(speed) * 255);
        }
};

#endif