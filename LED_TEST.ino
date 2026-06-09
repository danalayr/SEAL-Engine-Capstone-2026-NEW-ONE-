const int LED_PIN = 8; // Change to whatever pin your LED is on

void setup() {
  pinMode(LED_PIN, OUTPUT);
}

void loop() {
  digitalWrite(LED_PIN, HIGH); // Turn on
  delay(500);
  digitalWrite(LED_PIN, LOW);  // Turn off
  delay(500);
}
