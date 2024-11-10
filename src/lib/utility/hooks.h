#ifndef LIB_UTILITY_HOOKS_H
#define LIB_UTILITY_HOOKS_H

#include <Arduino.h>

class UseCallback {
    private:
        bool lastValue;
        bool currentValue;
        void (*callback)(bool, bool);

    public:
        UseCallback() {}
        UseCallback(void (*callback)(bool, bool)) {
            this->callback = callback;
            this->lastValue = false;
            this->currentValue = false;
        }

        void setCallback(void (*callback)(bool, bool)) {
            this->callback = callback;
        }

        /**
         * Updates the value of the hook and calls the callback if the value has changed.
         * Your value must evaluate to a boolean.
         */
        void update(bool value) {
            this->currentValue = value;
            if (this->lastValue != this->currentValue) this->callback(this->lastValue, this->currentValue);
            this->lastValue = this->currentValue;
        }

};

#endif