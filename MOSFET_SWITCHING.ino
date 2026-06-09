void setup() {
  pinMode(6, OUTPUT);        // pin 6 as output
  Serial.begin(9600);        // open serial monitor
}

void loop() {
  // Turn MOSFET ON
  digitalWrite(6, HIGH);
  Serial.println("MOSFET: ON — measure gate voltage now");
  delay(5000);               // stays ON for 5 seconds

  // Turn MOSFET OFF
  digitalWrite(6, LOW);
  Serial.println("MOSFET: OFF — measure gate voltage now");
  delay(5000);               // stays OFF for 5 seconds
}
