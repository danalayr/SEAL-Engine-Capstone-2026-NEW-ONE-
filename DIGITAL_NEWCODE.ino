/* DIGITAL ENCLOSURE - Thermal and humidity control 

CAPSTONE SEAL Engine 2026

The following code will complete these tasks...

- The MLX90614 (temperature sensor) will read IR surface temperature of the hottest component
  and control the fan speed based on these parameters. Two fans will run simultaneously: FAN 1 will be the
  intake and will draw cool air in directly onto the heat sinks, then FAN 2 will push the hot air out
  of the enclosure.

- The SHT31 humidity sensor will read humidity levels and turn on a blinking RED LED ALARM to 
  urge the users to use passive measures of humidity control if the fans aren't doing enough.

- The adhesive heater will be activated via a MOSFET and should activate when temperatures fall below 10 degrees C.

- NOTE: The fan should never fully turn off in order to spread adhesive heater heat across the enclosure
  avoiding concentrated heat in one area which would be a fire hazard. 

+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
*/

// Adding libraries 
  #include <Wire.h>              // Library connected with arduino 
  #include <Adafruit_MLX90614.h> // Downloaded sensor 
  #include <Adafruit_SHT31.h>    // Downloaded sensor 
  #include <FanController.h>     // Downloaded library — handles 25kHz PWM signal required by Noctua fans
                                 // replaces analogWrite() which runs at wrong frequency for PC fans

// Adding sensor objects
Adafruit_MLX90614 temperatureSensor = Adafruit_MLX90614(); // Configuring library connection to physical sensor 
Adafruit_SHT31 humiditySensor = Adafruit_SHT31();          // Configuring library connection to physical sensor 

// Fan controller initialization
// SENSOR_PIN1/2: tach wire (green, pin 3 on Noctua) — required by library even if not reading RPM
// SENSOR_THRESHOLD: minimum ms between RPM readings — 1000ms gives smooth stable readings
// PWM_PIN1/2: PWM wire (blue, pin 4 on Noctua) — this is what controls fan speed
#define SENSOR_PIN1      7    // Tach wire for Fan 1 (intake)
#define SENSOR_PIN2      3    // Tach wire for Fan 2 (exhaust)
#define SENSOR_THRESHOLD 1000
#define PWM_PIN1         6    // PWM wire for Fan 1 — intake fan
#define PWM_PIN2         10   // PWM wire for Fan 2 — exhaust fan

FanController fan1(SENSOR_PIN1, SENSOR_THRESHOLD, PWM_PIN1); // Configuring library connection to Fan 1 (intake)
FanController fan2(SENSOR_PIN2, SENSOR_THRESHOLD, PWM_PIN2); // Configuring library connection to Fan 2 (exhaust)

// Pins defined
  const int HEATER_PIN = 12; // Digital pin that controls the MOSFET (gate) --> will control heaters ON/OFF state 
  const int LED_PIN    = 13; // Humidity alarm RED blink
  const int BUZZER_PIN = 2; // Humidity alarm buzzer

// Humidity threshold 
// If humidity goes above 60% it can start harming sensitive electronic components 
  const float HUMIDITY_ALARM = 60.0; // 60%

// Temperature thresholds (celsius) 
// The temperature sensor will read surrounding component temperature readings
  const float TEMP_EXTREMEHIGH = 50.0; // FAN 100%
  const float TEMP_HIGH        = 35.0; // FAN 70% 
  const float TEMP_ROOMTEMP    = 20.0; // FAN 40%
  const float TEMP_COLD        = 10.0; // FAN 20% & HEATER ON

// Fan speeds (0-100%) - FanController uses percentages instead of 0-255 analogWrite values
// The fan will also be set to 20% if the heater turns on
// to keep the heater's heat dispersed equally across the enclosure
  const int FAN_MAX     = 100; // 100% — critical cooling
  const int FAN_HIGH    =  70; // ~70% — high temp cooling
  const int FAN_MEDIUM  =  40; // ~40% — room temperature operation
  const int FAN_MINIMUM =  20; // ~20% — minimum speed (heater active / cold)

// LED blinking interval
  const int RED_BLINK = 500; // Half a second 

// State (on or off) variables 
  bool ledState = LOW;           // LED on or off?
  unsigned long lastBlink = 0;  // Used unsigned because this will never be negative

// setup will start when arduino powers on and sets the above parameters 
  void setup() { // Runs once

  Serial.begin(9600); // 9600 baud for debugging or finding syntax errors

// pinMode is for configuration 
// No pinMode needed for fan pins — FanController library handles that internally
  pinMode(LED_PIN,    OUTPUT); // Humidity sensor will be the input and the output will be the LED light 
  pinMode(HEATER_PIN, OUTPUT); // The heater will be the output, sensors will be the input 
  pinMode(BUZZER_PIN, OUTPUT); // Buzzer will sound when humidity exceeds alarm threshold

  Serial.println(" ++++ SYSTEM TEST ++++"); 

// fan.begin() initializes the hardware timer and sets the correct 25kHz PWM frequency
// Must be called before any fan.setDutyCycle() calls
  fan1.begin();
  fan2.begin();

// Both fans start at 20%
  fan1.setDutyCycle(FAN_MINIMUM); // Never want the fan fully off 
  fan2.setDutyCycle(FAN_MINIMUM); // Never want the fan fully off 

// Making sure the heater starts off 
  digitalWrite(HEATER_PIN, LOW);

  Serial.println(" ++++ SYSTEM TEST STARTING ++++"); 

  // Test MLX90614 temperature sensor — .begin() initializes a hardware component
  if (temperatureSensor.begin()) { 
    Serial.println("Temperature sensor component connected"); 
  } else { 
    Serial.println("Temperature sensor component NOT FOUND - please check wiring!"); 
  } 

  // Test humidity sensor, same as above — 0x44 is the sensor's address 
  if (humiditySensor.begin(0x44)) { 
    Serial.println(" Humidity sensor connected, success"); 
  } else { 
    Serial.println(" Humidity sensor NOT FOUND - please check wiring!"); 
  } 

  Serial.println(" ++++ SYSTEM TEST HAS ENDED ++++"); 

  } // END

// Function to setFanSpeed(), instead of having to write fan.setDutyCycle(speed) each time
// Now accepts 0-100% values instead of 0-255 analogWrite values
  void setFanSpeed(int percent) {
    fan1.setDutyCycle(percent);
    fan2.setDutyCycle(percent);
  }

// Function for fanControl(), the MLX90614 surface temperature will be read 
// The correct fan speed will be set according to the temperature 
// sensed by the sensor device. NOTE: below 10 deg C the heater will turn on!
  float fanControl() {

    float objTemp     = temperatureSensor.readObjectTempC();
    float ambientTemp = humiditySensor.readTemperature();

    float hotTemp  = max(objTemp, ambientTemp);
    float coldTemp = min(objTemp, ambientTemp);

    // Cold check FIRST — if either sensor is cold, heater wins regardless
    if (coldTemp < TEMP_COLD) {
      setFanSpeed(FAN_MINIMUM);
      digitalWrite(HEATER_PIN, HIGH); // HIGH = MOSFET gate opens = heater ON
      Serial.print("[TEMP] COLD ---- Fan: 20% ---- Heater: ON  ");

    } else if (hotTemp > TEMP_EXTREMEHIGH) {
      setFanSpeed(FAN_MAX);
      digitalWrite(HEATER_PIN, LOW);
      Serial.print("[TEMP] EXTREMELY HOT ---- Fan: 100% ---- Heater: OFF  ");

    } else if (hotTemp > TEMP_HIGH) {
      setFanSpeed(FAN_HIGH);
      digitalWrite(HEATER_PIN, LOW);
      Serial.print("[TEMP] HIGH ---- Fan: 70% ---- Heater: OFF  ");

    } else if (hotTemp > TEMP_ROOMTEMP) {
      setFanSpeed(FAN_MEDIUM);
      digitalWrite(HEATER_PIN, LOW);
      Serial.print("[TEMP] ROOMTEMP ---- Fan: 40% ---- Heater: OFF  ");

    } else {
      setFanSpeed(FAN_MINIMUM);
      digitalWrite(HEATER_PIN, LOW);
      Serial.print("[TEMP] COOL MINIMUM ---- Fan: 20% ---- Heater: OFF  ");
    }

    return hotTemp; // This will allow the reading to loop()
  }


  float humidityControl() {
  float humidity = humiditySensor.readHumidity();

  if (humidity > HUMIDITY_ALARM) {
    unsigned long currentTime = millis();
    if (currentTime - lastBlink >= RED_BLINK) {
      ledState = !ledState;
      digitalWrite(LED_PIN, ledState);
      if (ledState) {
        tone(BUZZER_PIN, 1000); // 1000 Hz tone — change pitch here
      } else {
        noTone(BUZZER_PIN);     // Buzzer blinks in sync with LED
      }
      lastBlink = currentTime;
    }
    Serial.print("LIGHT ON, ALARM  ");

  } else {
    digitalWrite(LED_PIN, LOW); // LED off when humidity is normal
    noTone(BUZZER_PIN);         // Buzzer off when humidity is normal
    ledState = LOW;
    Serial.print("LIGHT OFF, NORMAL ");
  }
  return humidity;
}

// LOOP will run forever and will ensure the sensors keep reading data
// The helper functions are called here and readings are printed every two seconds! 
  void loop() {
  // Run temperature control — returns the higher of MLX and SHT readings for printing
  float hotTemp = fanControl();

  // Run humidity control — returns the humidity reading for printing
  float humidity = humidityControl();

  // Print both sensor readings 
  Serial.print("MLX Temp: ");
  Serial.print(temperatureSensor.readObjectTempC());
  Serial.print("C  SHT Temp: ");
  Serial.print(humiditySensor.readTemperature());
  Serial.print("C  Control Temp: ");
  Serial.print(hotTemp);
  Serial.print("C  Humidity: ");
  Serial.print(humidity);
  Serial.println("%");

  delay(2000); // Wait 2 seconds before reading again
}
