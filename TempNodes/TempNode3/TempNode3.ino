#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <OneWire.h>
#include <DallasTemperature.h>
#include "FS.h"

ESP8266WebServer server(80);
IPAddress ip(192, 168, 10, 109);
IPAddress gw(192, 168, 10, 1);
IPAddress sn(255, 255, 255, 0);
OneWire oneWire(4);
DallasTemperature sensors(&oneWire);

unsigned long previousMillis = 0;
const long interval = 60000;
String temperatuur, graphBuffer, line;

void temp() {
  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;
    sensors.requestTemperatures();
    temperatuur = sensors.getTempCByIndex(0);
    graphBuffer = "[";
    graphBuffer += millis() / 1000;
    graphBuffer += ", ";
    graphBuffer += temperatuur;
    graphBuffer += "],";
    File tempbuffer = SPIFFS.open("/tempbuffer.txt", "a");
    tempbuffer.print(graphBuffer);
    tempbuffer.close();
    graphBuffer = "";
  }
}

void handleTemp() {
  sensors.requestTemperatures();
  temperatuur = sensors.getTempCByIndex(0);
  File graphbuffer = SPIFFS.open("/tempbuffer.txt", "r");
  while (graphbuffer.available()) {
    line = graphbuffer.readStringUntil('\n');
    graphBuffer += line;
  }
  graphbuffer.close();
  temperatuur += graphBuffer;
  server.send(200, "text/plain", temperatuur);
  temperatuur = "";
  graphBuffer = "";
}

void setup(void) {
  Serial.begin(115200);
  SPIFFS.begin();
  SPIFFS.remove("/tempbuffer.txt");
  WiFi.config(ip, gw, sn);

  WiFi.begin("SSID", "PSK");
  Serial.println("");

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println(WiFi.localIP());

  server.on("/", handleTemp);
  server.begin();
}

void loop(void) {
  server.handleClient();
  temp();
}
