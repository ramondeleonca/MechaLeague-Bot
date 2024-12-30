#include <Arduino.h>
#include <lib/DRV8871.h>

DRV8871 m1(15, 2);
DRV8871 m2(4, 5);
DRV8871 m3(18, 19);
DRV8871 m4(13, 12);

void setup() {
    m1.begin();
    m2.begin();
    m3.begin();
    m4.begin();

    m1.set(0.15);
    m2.set(0.15);
    m3.set(0.15);
    m4.set(0.15);
}

void loop() {
    
}