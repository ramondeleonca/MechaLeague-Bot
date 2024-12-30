#ifndef MECHALEAGUE_MOTOR_CONTROLLER_H
#define MECHALEAGUE_MOTOR_CONTROLLER_H

#include <Arduino.h>

enum MLMotorId {
    M1 = 0,
    M2 = 1,
    M3 = 2,
    M4 = 3,

    FRONT_LEFT = M1,
    FRONT_RIGHT = M4,
    BACK_LEFT = M2,
    BACK_RRIGHT = M3
};

enum MLServoId {
    S1 = 1,
    S2 = 2
};

/**
 * The motor controller for MechaLeague Robots.
 * MechaLeagueMotorController must be enabled to receive commands.
 * MechaLeagueMotorController must be disabled when control link is lost.
 */
class MechaLeagueMotorController {
    private:
        uint rxPin;
        uint txPin;
        uint enablePin;
        HardwareSerial controller = HardwareSerial(1);
        bool invertedMotors[4] = {false, false, false, false};
        int motorValues[4] = {0, 0, 0, 0};

    public:
        MechaLeagueMotorController(uint rxPin = 16, uint txPin = 17, uint enablePin = 2) {
            this->rxPin = rxPin;
            this->txPin = txPin;
            this->enablePin = enablePin;
            this->controller.begin(9600, SERIAL_8N1, this->rxPin, this->txPin);
            pinMode(this->enablePin, OUTPUT);
            digitalWrite(this->enablePin, LOW);
        }

        /**
         * Updates internal register with the value for the motor
         * @param motor The motor to update (1-4)
         * @param value The value to set the motor to (-255 to 255)
         * @warning This method does not send the value to the motor controller, updates are buffered until flush() is called
         * @see flush()
         */
        void setMotor(MLMotorId motor, int value) {
            if (this->invertedMotors[motor]) value = -value;
            this->motorValues[motor] = value;
        }

        /**
         * Sets the motor to be inverted
         * @param motor The motor to invert (1-4)
         * @param inverted True to invert the motor, false to uninvert the motor
         */
        void setMotorInverted(MLMotorId motor, bool inverted) {
            this->invertedMotors[motor] = inverted;
        }

        /**
         * Updates internal register with the values for all motors
         * @param m1 The value to set motor 1 to (-255 to 255)
         * @param m2 The value to set motor 2 to (-255 to 255)
         * @param m3 The value to set motor 3 to (-255 to 255)
         * @param m4 The value to set motor 4 to (-255 to 255)
         * @warning This method does not send the values to the motor controller, updates are buffered until flush() is called
         * @see flush()
         */
        void setMotors(int m1, int m2, int m3, int m4) {
            this->setMotor(M1, m1);
            this->setMotor(M2, m2);
            this->setMotor(M3, m3);
            this->setMotor(M4, m4);
        }

        /**
         * Updates internal register with 0s for all motors and flushes the values to the motor controller
         * @warning This method sends the values directly to the motor controller, updates are sent immediately
         */
        void stopMotors() {
            this->setMotors(0, 0, 0, 0);
            this->flush();
        }

        /**
         * Updates servo value and sends the value to the motor controller
         * @param servo The servo to update (1-2)
         * @param value The angle to set the servo to (0-180)
         * @warning This method sends the values directly to the motor controller, updates are sent immediately
         */
        void setServo(MLServoId servo, int value) {
            this->controller.print("s ");
            this->controller.print(servo);
            this->controller.print(" ");
            this->controller.print(value);
            this->controller.print("\r\n");
        }

        /**
         * Sends all buffered motor values to the motor controller
         * @warning This method should be called after all motor values have been set
         * @see setMotor()
         */
        void flush() {
            // if (memcmp(this->previousMotorValues, this->motorValues, sizeof(this->motorValues)) == 0) {
            //     return;
            // }

            this->controller.print("mm ");
            for (int i = 0; i < sizeof(this->motorValues) / sizeof(this->motorValues[0]); i++) {
                this->controller.print(this->motorValues[i]);
                this->controller.print(" ");
            }
            this->controller.print("\r\n");

            // memcpy(this->previousMotorValues, this->motorValues, sizeof(this->motorValues));
        }

        /**
         * Enables the motor controller
         */
        void enable() {
            digitalWrite(this->enablePin, HIGH);
        }

        /**
         * Disables the motor controller
         */
        void disable() {
            digitalWrite(this->enablePin, LOW);
        }

        /**
         * Sends a raw command to the motor controller
         * @param command The command to send
         */
        void sendRaw(String command) {
            this->controller.println(command);
        }
};

#endif