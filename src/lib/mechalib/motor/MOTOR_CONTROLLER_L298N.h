#pragma once
#ifndef MECHALIB_MOTOR_CONTROLLER_L298N_H
#define MECHALIB_MOTOR_CONTROLLER_L298N_H

#include <Arduino.h>
#include <lib/mechalib/motor/MOTOR_CONTROLLER_Base.h>
#include <lib/mechalib/motor/MotorSafety.h>

class MOTOR_CONTROLLER_L298N : public MOTOR_CONTROLLER_Base {
    public:
        enum L298N_IdleMode {
            kCoast = 0,
            kBrake = 1
        };

    private:
        int in1;
        int in2;
        int en;
        L298N_IdleMode idleMode = L298N_IdleMode::kCoast;

    public:
        /**
         * @brief Constructor for the L298N motor controller.
         * @param in1 Pin for IN1 control.
         * @param in2 Pin for IN2 control.
         * @param en Pin for EN control (PWM).
         */
        MOTOR_CONTROLLER_L298N(int in1, int in2, int en, float deadband = 0.075) {
            this->in1 = in1;
            this->in2 = in2;
            this->en = en;
            this->deadband = deadband;
        };

        void setIdleMode(L298N_IdleMode mode) {
            this->idleMode = mode;
        }

        void begin() {
            pinMode(in1, OUTPUT);
            pinMode(in2, OUTPUT);
            pinMode(en, OUTPUT);

            digitalWrite(in1, LOW);
            digitalWrite(in2, LOW);
            analogWrite(en, LOW);
        }

        void set(float speed, bool isBeep) {
            speed = applySpeedFilters(speed, isBeep);

            if (speed > 0) {
                digitalWrite(in1, HIGH);
                digitalWrite(in2, LOW);
                analogWrite(en, abs(speed) * 255);
            } else if (speed < 0) {
                digitalWrite(in1, LOW);
                digitalWrite(in2, HIGH);
                analogWrite(en, abs(speed) * 255);
            } else {
                digitalWrite(in1, HIGH);
                digitalWrite(in2, HIGH);
                analogWrite(en, (idleMode == L298N_IdleMode::kBrake) ? 255 : 0);
            }

            // Update followers
            updateFollowers(speed, isBeep);
        }
        
        void set(float speed) {
            set(speed, false);
        };
};

#endif