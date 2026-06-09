// System Test Sketch for the DIGITAL ENCLOSURE

#include <Wire.h>              //Library connected with arduino 
#include <Adafruit_MLX90614.h> //downloaded sensor 
#include <Adafruit_SHT31.h>    //downloaded sensor 
#include <FanController.h>     //downloaded library — handles 25kHz PWM signal required by Noctua fans
                               //replaces analogWrite() which runs at wrong frequency for PC fans

Adafruit_MLX90614 mlx_TEMP = Adafruit_MLX90614();    //configuring library connection to physical sensor 
Adafruit_SHT31 sht31_HUMIDITY = Adafruit_SHT31();    //configuring library connection to physical sensor 

// Fan controller initialization
// SENSOR_PIN1/2: tach wire (green, pin 3 on Noctua) — required by library even if not reading RPM
// SENSOR_THRESHOLD: minimum ms between RPM readings — 1000ms gives smooth stable readings
// PWM_PIN1/2: PWM wire (blue, pin 4 on Noctua) — this is what controls fan speed
#define SENSOR_PIN1      2    // tach wire for Fan 1 (intake)
#define SENSOR_PIN2      3    // tach wire for Fan 2 (exhaust)
#define SENSOR_THRESHOLD 1000
#define PWM_PIN1         9    // PWM wire for Fan 1 — intake fan
#define PWM_PIN2         10   // PWM wire for Fan 2 — exhaust fan

FanController fan1(SENSOR_PIN1, SENSOR_THRESHOLD, PWM_PIN1); //configuring library connection to Fan 1 (intake)
FanController fan2(SENSOR_PIN2, SENSOR_THRESHOLD, PWM_PIN2); //configuring library connection to Fan 2 (exhaust)

const int LED_RED = 13; //to control the LED 

void setup() { //runs once

  Serial.begin(9600); //9600 baud for debugging or finding syntax errors

  // No pinMode needed for fan pins — FanController library handles that internally
  pinMode(LED_RED, OUTPUT); //Lets make sure the LED is working

  // fan.begin() initializes the hardware timer and sets the correct 25kHz PWM frequency
  // must be called before any fan.setDutyCycle() calls
  fan1.begin();
  fan2.begin();

  Serial.println("---SYSTEM TEST ---"); 

  // Test MLX90614 temperature sensor .begin initializes a hardware component 
  if (mlx_TEMP.begin()) { 
    Serial.println(" Temperature sensor component connected"); 
  } else { 
    Serial.println("Temperature sensor component NOT FOUND - please Check wiring!"); 
  } 

  // Test Humidity sensor, same as above and 0x44 is the sensors address 
  if (sht31_HUMIDITY.begin(0x44)) { 
    Serial.println(" Humidity sensor connected, succsess"); 
  } else { 
    Serial.println(" Humidity sensor NOT FOUND please Check wiring!"); 
  } 

  // Test fan control FAN #1
  fan1.setDutyCycle(50); // 50% speed
  delay(2000); //2 Sec

  fan1.setDutyCycle(100); // 100% speed
  delay(2000);  //2sec

  fan1.setDutyCycle(20); // minimum speed (20%) — Noctua won't fully stop
  Serial.println("Fan #1 is functioning properley!!!"); 

  // Test fan control FAN #2
  fan2.setDutyCycle(50); // 50% speed
  delay(2000); //2 Sec

  fan2.setDutyCycle(100); // 100% speed
  delay(2000);  //2sec

  fan2.setDutyCycle(20); // minimum speed (20%) — Noctua won't fully stop
  Serial.println("Fan #2 is functioning properley!!!");

  //test for LED control 
  digitalWrite(LED_RED, HIGH);
  delay(1000); //1 Sec

  digitalWrite(LED_RED, LOW);
  delay(1000); //1 Sec

  digitalWrite(LED_RED, HIGH);
  delay(1000); //1 Sec

  digitalWrite(LED_RED, LOW);
  delay(1000); //1 Sec

  Serial.println(" LED WORKS!!!");
} 

void loop() { //runs forever
//live sensor data that will repeat indefenitely 
  float obj_Temp = mlx_TEMP.readObjectTempC(); 

  float air_Temp = sht31_HUMIDITY.readTemperature(); 

  float humidity = sht31_HUMIDITY.readHumidity(); 

//print all readings, line to line 
  Serial.print("Component Temp: "); 

  Serial.print(obj_Temp); 

  Serial.print("°C | Air Temp: "); 

  Serial.print(air_Temp); 

  Serial.print("°C | Humidity: "); 

  Serial.print(humidity); 

  Serial.println("%"); 

  delay(2000); 
}