#ifndef JOYSTICK_UTILS_H
#define JOYSTICK_UTILS_H

#include <Arduino.h>

class JoystickUtils {
    public:
        /**
         * Provided the positive side deadband and min/max values, this function will return the value of the joystick 
         * ** ignoring portions of the joystick that are within the deadband. **
         */
        static float deadbandMap(float x, float deadband, float in_min, float in_max, float out_min, float out_max) {
            if (abs(x) < deadband) return 0.0;
            float sign = x > 0 ? 1 : -1;
            return sign * map(abs(x), in_min + deadband, in_max, out_min, out_max);
        };
};


#endif