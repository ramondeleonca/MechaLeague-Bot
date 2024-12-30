#pragma once
#ifndef MECHALIB_MOTOR_CONTROLLER_DRV8871_H
#define MECHALIB_MOTOR_CONTROLLER_DRV8871_H

#include <Arduino.h>
#include <lib/mechalib/motor/MOTOR_CONTROLLER_Base.h>
#include <lib/mechalib/motor/MotorSafety.h>

class MOTOR_CONTROLLER_DRV8871 : public MOTOR_CONTROLLER_Base {
    private:
        int in1;
        int in2;
    
    public:
        MOTOR_CONTROLLER_DRV8871(int in1, int in2, float deadband = 0.075) {
            this->in1 = in1;
            this->in2 = in2;
            this->deadband = deadband;
        };

        void begin() {
            pinMode(in1, OUTPUT);
            pinMode(in2, OUTPUT);

            analogWrite(in1, LOW);
            analogWrite(in2, LOW);
        }

        void set(float speed, bool isBeep) {
            speed = applySpeedFilters(speed, isBeep);

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

            // Update followers
            updateFollowers(speed, isBeep);
        }
        
        void set(float speed) {
            set(speed, false);
        };
};

#endif