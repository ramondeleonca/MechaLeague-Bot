#ifndef ALLIANCE_LED_H
#define ALLIANCE_LED_H

#include <Arduino.h>
#include <lib/mechalib/game/Alliance.h>

class AllianceLED {
    private:
        Alliance alliance = Alliance::NO_ALLIANCE;
        int redPin = 5;
        int bluePin = 18;
        bool blinking = false;
        int blinkingPeriod = 500;
        int lastBlinkTime;
        bool lastBlinkState;

    public:
        AllianceLED() {}
        AllianceLED(int redPin, int bluePin) {
            this->redPin;
            this-bluePin;
        }

        void begin() {
            pinMode(redPin, OUTPUT);
            pinMode(bluePin, OUTPUT); 
            writeAlliance();   
        }

        void writeAlliance() {
            if (alliance == Alliance::RED_ALLIANCE) {
                digitalWrite(redPin, HIGH);
                digitalWrite(bluePin, LOW);
            } else if (alliance == Alliance::BLUE_ALLIANCE) {
                digitalWrite(redPin, LOW);
                digitalWrite(bluePin, HIGH);
            } else {
                digitalWrite(redPin, HIGH);
                digitalWrite(bluePin, HIGH);
            }
        }

        void writeOff() {
            digitalWrite(redPin, LOW);
            digitalWrite(bluePin, LOW);
        }

        void setAlliance(Alliance alliance) {
            this->alliance = alliance;
        }

        void update() {
            if (blinking) {
                if (millis() - lastBlinkTime > blinkingPeriod) {
                    lastBlinkTime = millis();
                    lastBlinkState = !lastBlinkState;
                    if (lastBlinkState) writeAlliance();
                    else writeOff();
                }
            } else {
                writeAlliance();
            }
        }

        void setBlinking(bool blinking) {
            this->blinking = blinking;
        }

        void setBlinkingPeriod(int blinkingPeriod) {
            this->blinkingPeriod = blinkingPeriod;
        }
};

#endif