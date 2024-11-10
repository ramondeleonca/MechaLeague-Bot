#ifndef GYROMETER_H
#define GYROMETER_H

#include <BMI160Gen.h>
#include <QMC5883LCompass.h>

class Gyrometer {
    private:
        // * Fusion filter
        const float ALPHA = 0.78;

        // * Gyro
        const float GYRO_MAX_RAW_VALUE = 32768.0;
        const float GYRO_RANGE = 250.0;

        float convertRawGyro(int gRaw) {
            return (gRaw * GYRO_RANGE) / GYRO_MAX_RAW_VALUE;
        }

        // Gyro integration
        float gix, giy, giz;

        // * Track time
        long lastTime = -1;

        // * Final angles
        float roll, pitch, yaw;

        // * Compass
        float initialMagHeading;
        bool initialMagHeadingSet = false;

        // * Yaw smoothing buffer
        static const int YAW_BUFFER_SIZE = 10;
        float yawBuffer[YAW_BUFFER_SIZE] = {0};
        int yawBufferIndex = 0;
        int yawSampleCount = 0;

    public:
        QMC5883LCompass* compass;
        BMI160GenClass* gyro;

    public:
        Gyrometer() {
            this->compass = new QMC5883LCompass();
            this->gyro = new BMI160GenClass();
        }

        Gyrometer(QMC5883LCompass* compass, BMI160GenClass* gyro) {
            this->compass = compass;
            this->gyro = gyro;
        }

        void begin() {
            // Compass
            this->compass->init();
            this->compass->setCalibrationOffsets(378.00, -8114.00, 1561.00);
            this->compass->setCalibrationScales(3.05, 0.45, 2.32);
            this->compass->setMagneticDeclination(4, 40);

            // Gyro
            this->gyro->begin(BMI160GenClass::I2C_MODE, 0x68);
            this->gyro->setGyroRange(GYRO_RANGE);
        }

        /**
         * Update without delta time
         */
        void update() {
            if (lastTime == -1) lastTime = millis();
            long currentTime = millis();
            long dt = currentTime - lastTime;
            update(dt);
            lastTime = currentTime;
        }

        /**
         * Update with delta time
         */
        void update(long dtMs) {
            // Delta in seconds
            float dt = dtMs / 1000.0;

            // * Gyroscope
            // Read gyro
            int gxRaw, gyRaw, gzRaw;
            float gx, gy, gz;
            this->gyro->readGyro(gxRaw, gyRaw, gzRaw);

            // Convert raw gyro data to degrees/second
            gx = convertRawGyro(gxRaw);
            gy = convertRawGyro(gyRaw);
            gz = convertRawGyro(gzRaw);

            // Integrate gyro
            gix += gx * dt;
            giy += gy * dt;
            giz += gz * dt;

            // Read compass values
            this->compass->read();
            float mx = this->compass->getX();
            float my = this->compass->getY();
            float magHeading = atan2(my, mx) * 180 / PI; // Calculate magnetic heading in degrees

            // Initial mag heading
            if (!initialMagHeadingSet) {
                initialMagHeading = magHeading;
                initialMagHeadingSet = true;
                magHeading -= initialMagHeading;
            }

            // * Fusion filter
            // Complementary filter
            yaw = (ALPHA * giz) + ((1 - ALPHA) * magHeading);

            // * Normalize yaw
            if (yaw < 0) yaw += 360;
            else if (yaw >= 360) yaw -= 360;

            // * Update yaw buffer for smoothing
            yawBuffer[yawBufferIndex] = yaw;
            yawBufferIndex = (yawBufferIndex + 1) % YAW_BUFFER_SIZE;
            if (yawSampleCount < YAW_BUFFER_SIZE) yawSampleCount++;

            // Calculate the average yaw from the buffer
            float smoothedYaw = 0;
            for (int i = 0; i < yawSampleCount; i++) {
                smoothedYaw += yawBuffer[i];
            }
            yaw = smoothedYaw / yawSampleCount;
        }

        float getYaw() {
            return yaw;
        }
};

#endif
