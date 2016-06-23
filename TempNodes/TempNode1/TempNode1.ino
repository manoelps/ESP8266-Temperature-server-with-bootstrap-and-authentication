#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <OneWire.h>
#include <DallasTemperature.h>
#include "FS.h"

ESP8266WebServer server(80);
IPAddress ip(192, 168, 10, 111);
IPAddress gw(192, 168, 10, 1);
IPAddress sn(255, 255, 255, 0);
OneWire oneWire(4);
DallasTemperature sensors(&oneWire);

unsigned long previousMillis = 0;
const long interval = 60000;
String temperatuur, graphBuffer, line, theDate, datumTijd, uur, rest, tijd;
int intUur;

void formatTempAndDate() {
  //temperatuur
  sensors.requestTemperatures();
  temperatuur = sensors.getTempCByIndex(0);

  //datum en uur
  WiFiClient client;
  while (!!!client.connect("google.com", 80)) {
    Serial.println("connection failed, retrying...");
  }
  client.print("HEAD / HTTP/1.1\r\n\r\n");
  while (!!!client.available()) {
    yield();
  }

  while (client.available()) {
    if (client.read() == '\n') {
      if (client.read() == 'D') {
        if (client.read() == 'a') {
          if (client.read() == 't') {
            if (client.read() == 'e') {
              if (client.read() == ':') {
                client.read();
                theDate = client.readStringUntil('\r');
                client.stop();
                datumTijd = theDate.substring(5, 25);
                uur = datumTijd.substring(12, 14);
                rest = datumTijd.substring(14, 20);
                intUur = uur.toInt();
                intUur += 2;
                if (intUur == 24) {
                  intUur = 0;
                }
                if (intUur == 25) {
                  intUur = 1;
                }
                tijd = intUur + rest;
              }
            }
          }
        }
      }
    }
  }
}

void temp() {
  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;
    formatTempAndDate();
    sensors.requestTemperatures();
    temperatuur = sensors.getTempCByIndex(0);
    graphBuffer = "[";
    graphBuffer += tijd;
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

  //WiFi.begin("telenet-CC217", "9Hu4MX3S6821");
  WiFi.begin("telenet-69233", "ZGnUXZchGPWF");
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
