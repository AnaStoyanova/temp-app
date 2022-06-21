#include <WiFi.h>
#include <WiFiClient.h>
#include <WebServer.h>
#include <ESPmDNS.h>
#include <ArduinoJson.h>

// Includes for BME280 (temperature sensor)

#include <Wire.h>
#include <SPI.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>

// Values for temperature sensor

#define SEALEVELPRESSURE_HPA (1013.25)
#define SDA_PIN 16 // corresponding to RX2 on the ESP32
#define SCL_PIN 17 // corresponding to TX2 on the ESP32

Adafruit_BME280 bme; // I2C


const char* ssid = "A1_A1718E";
const char* password = "1d8a7242";

WebServer server(80);

const int led = 13;
StaticJsonDocument<250> jsonDocument;


int minValue = -5;
int maxValue = 50;

void currentTemp() {
  digitalWrite(led, 1);
  server.send(200, "text/plain", String(bme.readTemperature()));
  digitalWrite(led, 0);
}

void handleSetValues() {
  if (server.hasArg("plain") == false) {
    //handle error here
  }
  String body = server.arg("plain");
  deserializeJson(jsonDocument, body);
  
  // Get RGB components
  minValue = jsonDocument["minValue"];
  maxValue = jsonDocument["maxValue"];
  Serial.print("MinValue: ");
  Serial.print(minValue);
  Serial.print(" MaxValue: ");
  Serial.println(maxValue);
  
  // Respond to the client
  server.send(200, "application/json", "Success!");
}

void handleGetMinMaxValues() {
  const int capacity = JSON_OBJECT_SIZE(2);
  StaticJsonDocument<capacity> jsonDocument;

  jsonDocument["minValue"] = minValue;
  jsonDocument["maxValue"] = maxValue;

  String responseString = String();

  serializeJson(jsonDocument, responseString);

  server.send(200, "application/json", responseString);
}

void setup(void) {
  pinMode(led, OUTPUT);
  digitalWrite(led, 0);
  Serial.begin(115200);
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  Serial.println("");

  // Wait for connection
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  if (MDNS.begin("esp32")) {
    Serial.println("MDNS responder started");
  }

  //Routing
  server.on("/curr-temp", currentTemp);
  server.on("/set-values", HTTP_POST, handleSetValues);
  server.on("/get-values", handleGetMinMaxValues);

  //Start
  server.begin();
  server.enableCORS(true); //DON'T REMOVE, REPEAT DON'T REMOVE
  Serial.println("HTTP server started");

  // Setup temperature sensor
    
  Wire.begin(SDA_PIN, SCL_PIN);

  unsigned status = bme.begin(0x76);  

  if (!status) {
      Serial.println("Could not find a valid BME280 sensor!");
      while (1) delay(10);
  }
}

void loop(void) {
  server.handleClient();
  delay(2);//allow the cpu to switch to other tasks
}
