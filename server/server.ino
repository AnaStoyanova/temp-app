#include <WiFi.h>
#include <WiFiClient.h>
#include <WebServer.h>
#include <ESPmDNS.h>
#include <ArduinoJson.h>

const char* ssid = "A1_A1E4B1";
const char* password = "12e1c3a2";

WebServer server(80);

const int led = 13;
StaticJsonDocument<250> jsonDocument;


String temp = "50";
int minValue = -5;
int maxValue = 50;

void currentTemp() {
  digitalWrite(led, 1);
  server.send(200, "text/plain", temp);
  digitalWrite(led, 0);
}

void handlePost() {
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
  server.on("/set-values", HTTP_POST, handlePost);  

  //Start
  server.begin();
  server.enableCORS(true); //DON'T REMOVE, REPEAT DON'T REMOVE
  Serial.println("HTTP server started");
}

void loop(void) {
  server.handleClient();
  delay(2);//allow the cpu to switch to other tasks
}
