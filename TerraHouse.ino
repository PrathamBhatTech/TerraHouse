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

const char index_html[] PROGMEM = R"rawliteral(
  <!DOCTYPE HTML><html>
  <head>
    <meta name="viewport" content="width=device-width, initial-scale=1">
    <link rel="stylesheet" href="https://use.fontawesome.com/releases/v5.7.2/css/all.css" integrity="sha384-fnmOCqbTlWIlj8LyTjo7mOUStjsKC4pOpQbqyi7RrhN7udi9RwhKkMHpvLbHG9Sr" crossorigin="anonymous">
    <style>
      html {
       font-family: Arial;
       display: inline-block;
       margin: 0px auto;
       text-align: center;
      }
      h2 { font-size: 3.0rem; }
      p { font-size: 3.0rem; }
      .units { font-size: 1.2rem; }
      .dht-labels{
        font-size: 1.5rem;
        vertical-align:middle;
        padding-bottom: 15px;
      }
    </style>
  </head>
  <body>
    <h2>ESP8266 DHT Server</h2>
    <p>
      <i class="fas fa-thermometer-half" style="color:#059e8a;"></i> 
      <span class="dht-labels">Temperature</span> 
      <span id="temperature">%TEMPERATURE%</span>
      <sup class="units">&deg;C</sup>
    </p>
    <p>
      <i class="fas fa-tint" style="color:#00add6;"></i> 
      <span class="dht-labels">Humidity</span>
      <span id="humidity">%HUMIDITY%</span>
      <sup class="units">%</sup>
    </p>
  </body>
  <script>
  setInterval(function ( ) {
    var xhttp = new XMLHttpRequest();
    xhttp.onreadystatechange = function() {
      if (this.readyState == 4 && this.status == 200) {
        document.getElementById("temperature").innerHTML = this.responseText;
      }
    };
    xhttp.open("GET", "/temperature", true);
    xhttp.send();
  }, 10000 ) ;
  
  setInterval(function ( ) {
    var xhttp = new XMLHttpRequest();
    xhttp.onreadystatechange = function() {
      if (this.readyState == 4 && this.status == 200) {
        document.getElementById("humidity").innerHTML = this.responseText;
      }
    };
    xhttp.open("GET", "/humidity", true);
    xhttp.send();
  }, 10000 ) ;
  </script>
  </html>)rawliteral";
  

// Replaces placeholder with DHT values
String processor(const String& var){
  //Serial.println(var);
  if(var == "TEMPERATURE"){
    return String(t);
  }
  else if(var == "HUMIDITY"){
    return String(h);
  }
  return String();
}

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
  
  dht.begin();
  
  // Connect to Wi-Fi
  WiFi.begin(ssid, password);
  Serial.println("Connecting to WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println(".");
  }

  // Print ESP8266 Local IP Address
  Serial.println(WiFi.localIP());

  // TODO: Get data from text menu to put into 

  // Route for root / web page
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/html", index_html, processor);
  });
  server.on("/temperature", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/plain", String(t).c_str());
  });
  server.on("/humidity", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/plain", String(h).c_str());
  });

  // Start server
  server.begin();
  
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
