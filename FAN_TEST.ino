// FAN INCREMENT TEST — Analog Enclosure
// Uses FanController library (same as capstone code)
// Increments: 20% → 50% → 70% → 100%

#include <FanController.h>

#define SENSOR_PIN       4     // Tach wire (green)
#define SENSOR_THRESHOLD 1000
#define PWM_PIN          3     // PWM wire (blue)

FanController fan(SENSOR_PIN, SENSOR_THRESHOLD, PWM_PIN);

void setup() {
  Serial.begin(9600);
  fan.begin();

  Serial.println("--- FAN INCREMENT TEST ---");

  Serial.println("Fan → 20%");
  fan.setDutyCycle(20);
  delay(4000);

  Serial.println("Fan → 50%");
  fan.setDutyCycle(50);
  delay(4000);

  Serial.println("Fan → 70%");
  fan.setDutyCycle(70);
  delay(4000);

  Serial.println("Fan → 100%");
  fan.setDutyCycle(100);
  delay(4000);

  Serial.println("--- TEST COMPLETE ---");
  fan.setDutyCycle(20); // back to minimum, never fully off
}

void loop() {
  // empty — test runs once in setup()
}