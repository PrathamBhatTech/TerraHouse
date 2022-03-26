// Import required libraries
#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <Hash.h>
#include <ESPAsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <Adafruit_Sensor.h>
#include <DHT.h>

#include <Servo.h>
Servo WINDOW; // Create server object to control the servo

// Replace with your network credentials
const char* ssid = "SSID";
const char* password = "PASSWORD";

#define DHTPIN 16     // Digital pin connected to the DHT sensor
#define SoilMoisturePower 5 // Used to power on the sensor when required
#define SoilMoisture 4 // Digital pin connected to the Soil moisture sensor
#define LDR 0      // Digital pin connected to the LDR - USE 10k RESISTOR IN SERIES
#define Gas 12  // Digital pin connected to the MQ2 Gas sensor
#define pH A0 // Analog pin connected to the pH sensor

#define WaterPump 2 // Digital pin connected to the relay operating the water pump
#define ArtificialSun 14 // Digital pin connected to the Sunlight LED

void setup()  {
  // Serial port at baud rate 115200 for debugging purposes
  Serial.begin(115200);

  // Set the pins the sensors are connected to input
  pinMode(SoilMoisture, INPUT);
  pinMode(LDR, INPUT);
  pinMode(Gas, INPUT);
  pinMode(pH, INPUT);

  // Set the pins the actuators are connected to output
  pinMode(WaterPump, OUTPUT);
  pinMode(ArtificialSun, OUTPUT);

  // Attach the servo on pin 10 to the servo object
  WINDOW.attach(10);
  
}

void loop() {
  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis >= interval) {
    // save the last time you updated the DHT values
    previousMillis = currentMillis;

    // Collect data from sensors and store in global variables
//     check_temperature();
//     check_humidity();
//     check_soil_moisture();
//     check_light();
//     check_fire();
//     check_pH();

    // Use data from sensors in global variables to keep physical conditions within range
//     maintain_conditions();
  }
}
