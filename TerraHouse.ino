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

int plant_id = 0; // id no. of the plant

int dry_wet = 1; // current moisture in soil| 0 - too dry, 1 - perfect, 2 - too wet
bool bright_dark = true;  // check if there is sun or not| true - sun, false - no sun
int fire = 0; // No fire - 0, fire - 1

// current temperature & humidity
float t = 0.0;
float h = 0.0;

float pH_calibration_value = 21.34 - 0.7;
int pH_val = 7;

// Structure to store the physical condition range for plants.
struct PlantConditions {
  int soilWet;   // Define max value we consider soil 'wet'
  int soilDry;  // Define min value we consider soil 'dry'

  int hot; // in celsius

  int humid;  // humidity level
};

// Find values for atleaset 3 plants.
struct PlantConditions plant[3];

// Generally, you should use "unsigned long" for variables that hold time
// The value will quickly become too large for an int to store
unsigned long previousMillis = 0;    // will store last time DHT was updated

// Updates sensor readings every 10 seconds
const long interval = 10000;  

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
  
  // Set optimum conditions for different plants
  // Plant 1 conditions
  plant[0].soilWet = 500;
  plant[0].soilDry = 750;
  plant[0].hot = 40;
  plant[0].humid = 0;
  
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
