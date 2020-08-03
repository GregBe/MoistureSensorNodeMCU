#include "Arduino.h"

uint8_t moistureSensorPin = A0;
int sensorValue = 0;

void setup()
{
  Serial.begin(9600);
  Serial.println("Initializing");
}

// the loop function runs over and over again forever

/**
 * SensorValue interpretation:
 * Dry: (520 430]
 * Wet: (430 350]
 * Water: (350 260]
 * https://wiki.dfrobot.com/Capacitive_Soil_Moisture_Sensor_SKU_SEN0193
 * */
void loop()
{
  sensorValue = analogRead(moistureSensorPin);
  Serial.println(sensorValue);
  delay(100);
}
