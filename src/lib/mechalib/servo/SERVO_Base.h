#pragma once
#ifndef MECHALIB_SERVO_BASE_H
#define MECHALIB_SERVO_BASE_H

#include <Arduino.h>
#include <lib/mechalib/motor/MotorSafety.h>

/**
 * Recomended PWM pins: 2,4,12-19,21-23,25-27,32-33
 */
class SERVO_Base {
    protected:
        int minDeg = 0;
        int maxDeg = 180;

        int inverted = 0;
        int offset = 0;

        int minPulse = 1000;
        int maxPulse = 2000;
        
        SERVO_Base* followers[2] = {};
};

#endif