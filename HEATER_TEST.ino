// HEATER TEST — Digital Enclosure
// Turns heater ON for 60 seconds then automatically shuts off

const int HEATER_PIN = 12;     // MOSFET gate

void setup() {
  Serial.begin(9600); 
  delay(3000);  // 3 second window to open Serial Monitor
  pinMode(HEATER_PIN, OUTPUT);

  // safe start
  digitalWrite(HEATER_PIN, LOW);

  Serial.println("--- HEATER TEST ---");
  delay(2000);

  Serial.println("Heater → ON");
  digitalWrite(HEATER_PIN, HIGH);

  // 60 second countdown
  for (int i = 200; i > 0; i--) {
    Serial.print("Heater ON — ");
    Serial.print(i);
    Serial.println(" seconds remaining — touch surface to feel warmth");
    delay(1000);
  }

  digitalWrite(HEATER_PIN, LOW);
  Serial.println("Heater → OFF");
  Serial.println("--- HEATER TEST COMPLETE ---");
}

void loop() {
  // nothing — test runs once in setup
}