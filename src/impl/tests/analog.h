#include <Arduino.h>

const int m1 = 13;
const int m2 = 12;
const int m3 = 14;
const int m4 = 27;

const int s1 = 26;
const int s2 = 25;

void setup() {
    pinMode(m1, OUTPUT);
    pinMode(m2, OUTPUT);
    pinMode(m3, OUTPUT);
    pinMode(m4, OUTPUT);

    analogWrite(m1, 0);
    analogWrite(m2, 0);
    analogWrite(m3, 0);
    analogWrite(m4, 0);

    pinMode(s1, OUTPUT);
    pinMode(s2, OUTPUT);

    analogWrite(s1, 0);
    analogWrite(s2, 0);

    analogWrite(m1, 50);
}

void loop() {

}