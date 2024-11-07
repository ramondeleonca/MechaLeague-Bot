#include <Arduino.h>
#include <constants.h>
#include <lib/hal/MechaLeagueMotorController.h>
#include <lib/drive/MecanumDriveKinematics.h>

class Drivetrain {
    public:
        MechaLeagueMotorController* motors;
        MecanumDriveKinematics kinematics = MecanumDriveKinematics(WHEEL_DIAMETER_MM / 1000, TRACK_WIDTH_MM / 1000, WHEEL_BASE_MM / 1000);
        float wheelSpeeds[4];

    public:
        Drivetrain(MechaLeagueMotorController* motors){
            this->motors = motors;
        }

        void drive(float vx, float vy, float omega) {
            kinematics.calculateWheelSpeeds(wheelSpeeds, vx, vy, omega);
            kinematics.normalizeWheelSpeeds(wheelSpeeds);
            update();
        }

        void update() {
            motors->setMotors(
                wheelSpeeds[0] * MOTOR_1_MULTIPLIER * 255,
                wheelSpeeds[2] * MOTOR_2_MULTIPLIER * 255,
                wheelSpeeds[3] * MOTOR_4_MULTIPLIER * 255,
                wheelSpeeds[1] * MOTOR_3_MULTIPLIER * 255
            );
            motors->flush();
        }

        void stop() {
            motors->stopMotors();
            motors->flush();
        }
};