#include "Arduino.h"
#include <ESP8266WiFi.h> //https://github.com/esp8266/Arduino
#include <DNSServer.h>
#include <ESP8266WebServer.h>
#include <WiFiManager.h> //https://github.com/tzapu/WiFiManager​ https://circuitdigest.com/microcontroller-projects/using-wifi-manager-on-nodemcu-to-scan-and-connect-wifi-networks
#include <IFTTTMaker.h>
#include <WiFiClientSecure.h>
#include "arduino_secrets.h"

#define KEY IFTTT_KEY  // Get it from this page https://ifttt.com/services/maker/settings
#define EVENT_NAME_DEVICE_ONLINE IFTTT_EVENT_DEVICE_ONLINE // Name of your event name, set when you are creating the applet
#define EVENT_NAME_INFO IFTTT_EVENT_INFO // Name of your event name, set when you are creating the applet


WiFiClientSecure client;
IFTTTMaker ifttt(KEY, client);
uint8_t moistureSensorPin = A0;
int sensorValue = 0;

void setup()
{
  client.setInsecure(); //needed to not update the fingerprint for ifttt
  WiFiManager wifiManager;

  Serial.begin(115200);
  Serial.println("Initializing");

  //set static ip
  if (!wifiManager.autoConnect("ESP_MOISTURE"))
  {
    Serial.println("failed to connect and hit timeout");
    delay(3000);
    //reset and try again, or maybe put it to deep sleep
    ESP.reset();
    delay(5000);
  }
  Serial.println("connected");

  if(ifttt.triggerEvent(EVENT_NAME_DEVICE_ONLINE)){
    Serial.println("Successfully sent");
  } else
  {
    Serial.println("Failed!");
  }
}

/**
 * SensorValue interpretation:
 * Dry: (590 450]
 * Wet: (450 310]
 * Water: (310 170]
 * https://wiki.dfrobot.com/Capacitive_Soil_Moisture_Sensor_SKU_SEN0193
 * */
void loop()
{
  sensorValue = analogRead(moistureSensorPin);
  Serial.println(sensorValue);

  if(sensorValue>450){
    ifttt.triggerEvent(EVENT_NAME_INFO,"Zu trocken", "Bitte gießen.");
  }else if(sensorValue>310){
    ifttt.triggerEvent(EVENT_NAME_INFO,"Genau richtig");
  }else if(sensorValue <310 && sensorValue > 50){
    ifttt.triggerEvent(EVENT_NAME_INFO,"Zu feucht", "Bitte weniger gießen.");
  } 
   ESP.deepSleep(300e6); 
}
