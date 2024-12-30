#ifndef DRV8871_H
#define DRV8871_H

#include <Arduino.h>

class DRV8871 {
    private:
        int in1;
        int in2;
    
    public:
        DRV8871(int in1, int in2) {
            this->in1 = in1;
            this->in2 = in2;
        };

        void begin() {
            pinMode(in1, OUTPUT);
            pinMode(in2, OUTPUT);

            analogWrite(in1, LOW);
            analogWrite(in2, LOW);
        }

        void set(float speed) {
            if (speed > 0) {
                analogWrite(in1, abs(speed) * 255);
                analogWrite(in2, LOW);
            } else if (speed < 0) {
                analogWrite(in1, LOW);
                analogWrite(in2, abs(speed) * 255);
            } else {
                analogWrite(in1, LOW);
                analogWrite(in2, LOW);
            }
        }

        void stop() {
            analogWrite(in1, LOW);
            analogWrite(in2, LOW);
        }
};

#endif