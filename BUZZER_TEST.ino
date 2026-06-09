void setup() {
  pinMode(10, OUTPUT);        // change 8 to whatever pin your buzzer is on
  Serial.begin(9600);
}

void loop() {
  Serial.println("Buzzer: ON");
  analogWrite(10, HIGH);
  delay(1000);               // beeps for 1 second

  Serial.println("Buzzer: OFF");
  digitalWrite(10, LOW);
  delay(1000);               // off for 1 second
}
