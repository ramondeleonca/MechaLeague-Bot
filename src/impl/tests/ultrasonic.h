#include <Arduino.h>

const int trigPin = 15;   // Trig pin
const int echoPin = 2;  // Echo pin

void setup() {
  Serial.begin(115200);
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
}

void loop() {
  // Send a 10 µs pulse to trigger the sensor
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  // Measure the time for the echo to return
  long duration = pulseIn(echoPin, HIGH);

  // Calculate the distance in cm
  float distance = duration * 0.034 / 2;

  // Print the distance
  Serial.print("Distance: ");
  Serial.print(distance);
  Serial.println(" cm");

  delay(1000);
}
