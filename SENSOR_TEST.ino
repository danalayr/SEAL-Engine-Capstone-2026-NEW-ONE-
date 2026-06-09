// SENSOR TEST — Digital Enclosure
// Prints live readings from MLX90614 and SHT31 every 2 seconds

#include <Wire.h>
#include <Adafruit_MLX90614.h>
#include <Adafruit_SHT31.h>

Adafruit_MLX90614 mlx_TEMP      = Adafruit_MLX90614();
Adafruit_SHT31    sht31_HUMIDITY = Adafruit_SHT31();

void setup() {
  Serial.begin(9600);
  Serial.println("--- SENSOR TEST ---");

  if (mlx_TEMP.begin()) {
    Serial.println("MLX90614 — CONNECTED");
  } else {
    Serial.println("MLX90614 — NOT FOUND, check wiring!");
  }

  if (sht31_HUMIDITY.begin(0x44)) {
    Serial.println("SHT31 — CONNECTED");
  } else {
    Serial.println("SHT31 — NOT FOUND, check wiring!");
  }

  Serial.println("Reading sensors every 2 seconds...");
  Serial.println("-----------------------------------");
}

void loop() {
  float obj_Temp = mlx_TEMP.readObjectTempC();
  float air_Temp = sht31_HUMIDITY.readTemperature();
  float humidity  = sht31_HUMIDITY.readHumidity();

  Serial.print("Component Temp: ");
  Serial.print(obj_Temp);
  Serial.print(" C  |  Air Temp: ");        
  Serial.print(air_Temp);
  Serial.print(" C  |  Humidity: ");
  Serial.print(humidity);
  Serial.println(" %");

  delay(2000);
}
