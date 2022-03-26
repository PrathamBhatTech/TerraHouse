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
const char* ssid = "PBTRON";
const char* password = "bowdog123";

#define DHTPIN 5     // Digital pin connected to the DHT sensor
#define SoilMoisturePower 9 // Used to power on the sensor when required
#define SoilMoisture A0 // Digital pin connected to the Soil moisture sensor
//#define LDR 10      // Digital pin connected to the LDR - USE 10k RESISTOR IN SERIES
//#define Gas 16  // Digital pin connected to the MQ2 Gas sensor
//#define pH 12 // Analog pin connected to the pH sensor

#define WaterPump 12 // Digital pin connected to the relay operating the water pump
//#define ArtificialSun 11 // Digital pin connected to the Sunlight LED
//#define Alarm 6
#define ServoPin 13


// Uncomment the type of sensor in use:
#define DHTTYPE    DHT11     // DHT 11
//#define DHTTYPE    DHT22     // DHT 22 (AM2302)
//#define DHTTYPE    DHT21     // DHT 21 (AM2301)

DHT dht(DHTPIN, DHTTYPE);

int plant_id = 0; // id no. of the plant

int dry_wet = 1; // current moisture in soil| 1 - too dry, 0 - wet
bool bright_dark = true;  // check if there is sun or not| true - sun, false - no sun
int fire = 0; // No fire - 0, fire - 1

// current temperature & humidity
float t = 0.0;
float h = 0.0;

float pH_calibration_value = 21.34 - 0.7;
float pH_val = 6.2;

int fire_threshold = 600;

int soilmoisture  = 100;

// Structure to store the physical condition range for plants.
struct PlantConditions {
  int soilDry;  // Define min value percentage we consider soil 'dry'

  float hot; // in celsius

  int humid;  // humidity level
};

// Find values for atleaset 3 plants.
struct PlantConditions plant[3];

// Create AsyncWebServer object on port 80
AsyncWebServer server(80);

// Generally, you should use "unsigned long" for variables that hold time
// The value will quickly become too large for an int to store
unsigned long previousMillis = 0;    // will store last time DHT was updated

// Updates sensor readings every 10 seconds
const long interval = 2000;

const char index_html[] PROGMEM = R"rawliteral(
  <!DOCTYPE HTML>
<html>
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
    <style>
        /* Style the tab */
        .tab {
            overflow: hidden;
            border: 1px solid #ccc;
            background-color: #f1f1f1;
        }

        /* Style the buttons inside the tab */
        .tab button {
            background-color: inherit;
            float: left;
            border: none;
            outline: none;
            cursor: pointer;
            padding: 14px 16px;
            transition: 0.3s;
            font-size: 17px;
        }

        /* Change background color of buttons on hover */
        .tab button:hover {
            background-color: #ddd;
        }

        /* Create an active/current tablink class */
        .tab button.active {
            background-color: #ccc;
        }

        /* Style the tab content */
        .tabcontent {
            display: none;
            padding: 6px 12px;
            border: 1px solid #ccc;
            border-top: none;
        }
    </style>

    </head>

    <body>
        <h2>ESP8266 DHT Server</h2>

        <div class="tab">
            <button class="tablinks" onclick="openTab(event, 'sensordata')">sensor data</button>
            <button class="tablinks" onclick="openTab(event, 'sensordata')">choose plant</button>
        </div>

        <div id="sensordata" class="tabcontent"></div>
            <p>
                <i class="fas fa-thermometer-half" style="color:#059e8a;"></i> 
                <span class="dht-labels">Temperature</span> 
                <span id="temperature">%TEMPERATURE%</span>
                <span class="units">&deg;C</span>
            </p>
            <p>
                <i class="fas fa-tint" style="color:#00add6;"></i> 
                <span class="dht-labels">Humidity</span>
                <span id="humidity">%HUMIDITY%</span>
                <span class="units">%</span>
            </p>

            <p>
                <span class="dht-labels">Soil Moisture </span>
                <span id="soilmoisture">%SOILMOISTURE%</span>
                <span class="units">%</span>
            </p>

            <p>
                <span class="dht-labels">pH  </span>
                <span id="ph">%PH%</span>
                <span class="units"></span>
            </p>
        </div>

    </body>

    <script>
        function openTab(evt, tabName)  {
            var i, tabcontent, tablinks;
            tabcontent = document.getElementsByClassName("tabcontent");
            for (i = 0; i < tabcontent.length; i++) {
                tabcontent[i].style.display = "none";
            }
            tablinks = document.getElementsByClassName("tablinks");
            for (i = 0; i < tablinks.length; i++) {
                tablinks[i].className = tablinks[i].className.replace(" active", "");
            }
            document.getElementById(tabName).style.display = "block";
            evt.currentTarget.className += " active";
        }
    </script>

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
        }, 2000 ) ;

        setInterval(function ( ) {
        var xhttp = new XMLHttpRequest();
        xhttp.onreadystatechange = function() {
            if (this.readyState == 4 && this.status == 200) {
            document.getElementById("humidity").innerHTML = this.responseText;
            }
        };
        xhttp.open("GET", "/humidity", true);
        xhttp.send();
        }, 2000 ) ;

        setInterval(function ( ) {
        var xhttp = new XMLHttpRequest();
        xhttp.onreadystatechange = function() {
            if (this.readyState == 4 && this.status == 200) {
            document.getElementById("soilmoisture").innerHTML = this.responseText;
            }
        };
        xhttp.open("GET", "/soilmoisture", true);
        xhttp.send();
        }, 2000 ) ;

        setInterval(function ( ) {
        var xhttp = new XMLHttpRequest();
        xhttp.onreadystatechange = function() {
            if (this.readyState == 4 && this.status == 200) {
            document.getElementById("ph").innerHTML = this.responseText;
            }
        };
        xhttp.open("GET", "/ph", true);
        xhttp.send();
        }, 10000 ) ;
    </script>
</html>
  )rawliteral";


// Replaces placeholder with DHT values
String processor(const String& var) {
  //Serial.println(var);
  if (var == "TEMPERATURE") {
    return String(t);
  }
  else if (var == "HUMIDITY") {
    return String(h);
  }
  return String();
}


void setup()  {
  // Serial port at baud rate 115200 for debugging purposes
  Serial.begin(115200);

  // Set the pins the sensors are connected to input
  pinMode(SoilMoisture, INPUT);
  //  pinMode(LDR, INPUT);
  //  pinMode(Gas, INPUT);
  //   pinMode(pH, INPUT);

  // Set the pins the actuators are connected to output
  pinMode(WaterPump, OUTPUT);
  digitalWrite(WaterPump, HIGH);
  //  pinMode(ArtificialSun, OUTPUT);
  //  pinMode(Alarm, OUTPUT);

  // Attach the servo on pin 10 to the servo object
  WINDOW.attach(ServoPin);


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
  server.on("/", HTTP_GET, [](AsyncWebServerRequest * request) {
    request->send_P(200, "text/html", index_html, processor);
  });
  server.on("/temperature", HTTP_GET, [](AsyncWebServerRequest * request) {
    request->send_P(200, "text/plain", String(t).c_str());
  });
  server.on("/humidity", HTTP_GET, [](AsyncWebServerRequest * request) {
    request->send_P(200, "text/plain", String(h).c_str());
  });
  server.on("/soilmoisture", HTTP_GET, [](AsyncWebServerRequest * request) {
    request->send_P(200, "text/plain", String(soilmoisture).c_str());
  });
  server.on("/ph", HTTP_GET, [](AsyncWebServerRequest * request) {
    request->send_P(200, "text/plain", String(pH_val).c_str());
  });

  // Start server
  server.begin();

  // Set optimum conditions for different plants
  // Plant 1 conditions
  plant[0].soilDry = 20;  // Percentage of water in the soil
  plant[0].hot = 25.5;
  plant[0].humid = 30;

  // potato plant 2
  plant[0].soilDry = 50;  // Percentage of water in the soil
  plant[0].hot = 22;
  plant[0].humid = 60;

  // plant 3 carrot
  plant[0].soilDry = 40;  // Percentage of water in the soil
  plant[0].hot = 20;
  plant[0].humid = 55;

  // plant 4 chillis
  plant[0].soilDry = 15;  // Percentage of water in the soil
  plant[0].hot = 32;
  plant[0].humid = 5;
}


void check_temperature()  {
  // Read temperature as Celsius (the default)
  float newT = dht.readTemperature();
  // Read temperature as Fahrenheit (isFahrenheit = true)
  // float newT = dht.readTemperature(true);
  // if temperature read failed, don't change t value
  if (isnan(newT)) {
    Serial.println("Failed to read from DHT sensor!");
  }
  else {
    t = newT;
    Serial.println("Temperature: " + (String)t);
  }
}

void check_humidity() {
  // Read Humidity
  float newH = dht.readHumidity();
  // if humidity read failed, don't change h value
  if (isnan(newH)) {
    Serial.println("Failed to read from DHT sensor!");
  }
  else {
    h = newH;
    Serial.println("Humidity: " + (String)h);
  }
}

// Returns 0 - Too dry, 1 - Perfect, 2 - Too wet
void check_soil_moisture()  {
  digitalWrite(SoilMoisturePower, HIGH);    // Turn the sensor ON
  delay(10);                               // Allow power to settle
  soilmoisture = 100 - (analogRead(SoilMoisture) / 1024) * 100; // Read the analog value form sensor and convert the values to percentage
  digitalWrite(SoilMoisturePower, LOW);  // Turn the sensor OFF

  Serial.print("Soil Moisture: " + (String)soilmoisture + "\n");

  // Determine status of our soil
  if (soilmoisture < plant[plant_id].soilDry) {
    Serial.println("Status: Soil is too dry - time to water!");
    dry_wet = 0;
  }
  else  {
    dry_wet = 1;
  }
}


// false - dark, true - light
//void check_light()  {
//  int light = analogRead(LDR) * 4; // Since NodeMCU has 10-bit ADC (0-1023)
//  Serial.print("Light: " + (String)light + "\n");  // Print the values to the serial monitor for creating database
//
//  if (light < 1400)
//    bright_dark = false; // dark
//  else
//    bright_dark = true; // bright
//}

//void check_fire() {
//   int fire_sensor_data = analogRead(Gas);
//   Serial.print("fire data: " + (String)fire_sensor_data);
//
//   if(fire_sensor_data > fire_threshold)  {
//    fire = 1;
//   }
//   else {
//    fire = 0;
//   }
//}
//
//void check_pH() {
//  pH_val = analogRead(pH);  // Read the voltage value from the analog pin
//
//  float volt = (float)pH_val*5.0/1024/6;
//  pH_val = -5.70 * volt + pH_calibration_value;  // Convert the value read from the analog pin to pH value using formulas
//
//  Serial.print("pH: " + (String)pH_val);
//}

// open - true, close - false
void open_close_window(bool pos) {
  if (pos)  // Set window to open
    WINDOW.write(0);
  else  // Set window to close
    WINDOW.write(90);
}
//
//// True is on and False off
//void artificial_sun(bool on_off)  {
//  if (on_off)
//    digitalWrite(ArtificialSun, HIGH);
//  else
//    digitalWrite(ArtificialSun, LOW);
//}

// open - 1, close - 0
void irrigate(bool on_off) {
  if (on_off)
    digitalWrite(WaterPump, HIGH);
  else  {
    digitalWrite(WaterPump, LOW);
    delay(1000);
    digitalWrite(WaterPump, HIGH);
    delay(1000);
    digitalWrite(WaterPump, LOW);
  }
}

// Checks all the values from the sensors and maintains them within the range
// by running the corresponding actuators
void maintain_conditions()  {
  // maintain Temperature
  if (t > plant[plant_id].hot)
    open_close_window(true);
  else
    open_close_window(false);

  // maintain humidity
  // probably use the same code as temperature
  // cause only way to maintain is open window and a humidifier or something

  // maintain soil moisture
  if (dry_wet)
    irrigate(true);
  else
    irrigate(false);

  // check if its bright or not
  //  if(!bright_dark)
  //    artificial_sun(true);
  //  else
  //    artificial_sun(false);
}

void loop() {
  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis >= interval) {
    // save the last time you updated the DHT values
    previousMillis = currentMillis;

    // Collect data from sensors and store in global variables
    check_temperature();
    check_humidity();
    check_soil_moisture();
    //    check_light();
    //    check_fire();
    //    check_pH();

    //    if(fire)  {
    //      Serial.write("FIRE!!!");
    //      digitalWrite(Alarm, HIGH);
    //      delay(1000);
    //      digitalWrite(Alarm, LOW);
    //    }
    //
    //    // Use data from sensors in global variables to keep physical conditions within range
    maintain_conditions();
  }
}
