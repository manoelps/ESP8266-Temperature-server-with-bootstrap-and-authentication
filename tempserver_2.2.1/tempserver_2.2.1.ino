/*
    TODO:
    ADD HOMEPAGE WITH GRAPHS
    DATABASE
    ADD USER VIA LOGIN PAGE
    LOGIN FROM DATABASE
    GET USER PROFILE DATA
    ADD GRAPHS FOR 1H, 12H, 1d, 1w, 1m, all
    SEND TEMPS TO SPREADSHEET
    SEND FROM SPREADSHEET TO EMAIL
    ADD MULTIPLE TEMPSENSORS FUNCTIONALITY
*/
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <WiFiUdp.h>
#include <OneWire.h>
#include <DallasTemperature.h>
#include "FS.h"

#define ONE_WIRE_BUS 4
#define EIOT_CLOUD_INSTANCE_PARAM_ID    "56886051cf045c6956809602/pFeQEfmny1hT7IFd"
#define EIOT_CLOUD_ADDRESS     "cloud.iot-playground.com"
#define EIOT_CLOUD_PORT        40404

OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);
DeviceAddress deviceAddress;
ESP8266WebServer server(80);

unsigned long previousMillis = 0;
const long interval = 60000;
String Temp, XML, login, Home, Omdb, Profile, tijd, tempBuffer, graphBuffer, Graph, uur, Weather, Buffer, datumTijd, theDate, msg;
float temperatuur;

const char* Head = {"<html>\n<head>\n<meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\"><title>Server</title><link rel=\"icon\" type=\"image/png\" href=\"https://image.freepik.com/free-icon/ssl-encryption_318-32078.png\"><link rel=\"stylesheet\" href=\"https://maxcdn.bootstrapcdn.com/bootstrap/3.3.6/css/bootstrap.min.css\" integrity=\"sha384-1q8mTJOASx8j1Au+a5WDVnPi2lkFfwwEAa8hDDdjZlpLegxhjVME1fgjWPGmkzs7\" crossorigin=\"anonymous\">\n<style>.footer {position: absolute; bottom: 0; width: 100%; height: 60px; background-color: #f5f5f5;} .well{ background-color: #99d6ff;}</style>\n<script src=\"https://ajax.googleapis.com/ajax/libs/jquery/1.12.2/jquery.min.js\"></script>\n<script src=\"http://maxcdn.bootstrapcdn.com/bootstrap/3.3.6/js/bootstrap.min.js\"></script>\n"};
const char* nav1 = {"<nav class=\"navbar navbar-inverse navbar-fixed-top\">\n <div class=\"container-fluid\">\n <div class=\"navbar-header\">\n <button type=\"button\" class=\"navbar-toggle\" data-toggle=\"collapse\" data-target=\"#myNavbar\">\n <span class=\"icon-bar\"></span>\n <span class=\"icon-bar\"></span>\n <span class=\"icon-bar\"></span>\n </button>\n <a class=\"navbar-brand\" href=\"#\"><span class=\"glyphicon glyphicon-cloud\"></span></a>\n </div>\n <div class=\"collapse navbar-collapse\" id=\"myNavbar\">\n <ul class=\"nav navbar-nav\">\n"};
const char* nav2 = {"</ul>\n <form class=\"navbar-form navbar-right\" role=\"search\">\n <div class=\"form-group input-group\">\n <input type=\"text\" class=\"form-control\" placeholder=\"Search...\">\n <span class=\"input-group-btn\">\n <button class=\"btn btn-default\" type=\"button\">\n <span class=\"glyphicon glyphicon-search\"></span>\n </button>\n </span>\n </div>\n </form>\n <ul class=\"nav navbar-nav navbar-right\">\n"};
const char* nav3 = {"<li><a href=\"/login?disconnect=yes\">Afmelden</a></li>\n </ul>\n </div>\n </div>\n </nav>\n <br><br><br>\n"};
const char* addTo = {"<script>\n function GetClock(){\n var d=new Date();\n var nmonth=d.getMonth(),ndate=d.getDate(),nyear=d.getYear();\n if(nyear<1000) nyear+=1900;\n var nhour=d.getHours(),nmin=d.getMinutes(),nsec=d.getSeconds();\n if(nmin<=9) nmin=\"0\"+nmin\n if(nsec<=9) nsec=\"0\"+nsec;\n document.getElementById('clockbox').innerHTML=\"\"+ndate+\"/\"+(nmonth+1)+\"/\"+nyear+\"     \"+nhour+\":\"+nmin+\":\"+nsec+\"\";\n }\n window.onload=function(){\n GetClock();\n setInterval(GetClock,1000);\n }\n </script>\n"};
const char* login1 = {"<!DOCTYPE html>\n <html>\n <head>\n <meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\">\n <title>Log In</title>\n <link rel=\"icon\" type=\"image/png\" href=\"https://image.freepik.com/free-icon/ssl-encryption_318-32078.png\"> <link rel=\"stylesheet\" href=\"https://maxcdn.bootstrapcdn.com/bootstrap/3.3.6/css/bootstrap.min.css\" integrity=\"sha384-1q8mTJOASx8j1Au+a5WDVnPi2lkFfwwEAa8hDDdjZlpLegxhjVME1fgjWPGmkzs7\" crossorigin=\"anonymous\">\n <script src=\"https://ajax.googleapis.com/ajax/libs/jquery/1.12.2/jquery.min.js\"></script>\n <script src=\"http://maxcdn.bootstrapcdn.com/bootstrap/3.3.6/js/bootstrap.min.js\"></script>\n </head>\n <body>\n <div id=\"loginModal\" class=\"modal show\" tabindex=\"-1\" role=\"dialog\" aria-hidden=\"true\">\n <div class=\"modal-dialog\">\n <div class=\"modal-content\">\n <div class=\"modal-header\">\n <h1 class=\"text-center\">Login</h1>\n </div>\n <div class=\"modal-body\">\n <form class=\"form center-block\" action=\"/login\" method=\"POST\">\n <div class=\"form-group\">\n <input type=\"text\" class=\"form-control input-lg\" placeholder=\"Username\" name=\"USERNAME\">\n </div>\n <div class=\"form-group\">\n <input type=\"password\" class=\"form-control input-lg\" placeholder=\"Password\" name=\"PASSWORD\">\n </div>\n <div class=\"form-group\">\n <button class=\"btn btn-primary btn-lg btn-block\" name=\"SUBMIT\">Log In</button>\n </div>\n </form>\n </div>\n <div class=\"modal-footer\">\n"};
const char* login2 = {"<div class=\"col-md-12\">\n </div>\n </div>\n </div>\n </div>\n </div>\n </body>\n </html>\n"};
const char* graph1 = {"<script>\n google.charts.load('current', {packages: ['corechart', 'line']});\n google.charts.setOnLoadCallback(drawBackgroundColor);\n function drawBackgroundColor() {\n var data = new google.visualization.DataTable();\n data.addColumn('number', 'time');\n data.addColumn('number', 'graden celsius');\n data.addRows([\n"};
const char* graph2 = {"]);\n var options = {\n hAxis: {title: 'Time'},vAxis: {title: 'Temperature'},backgroundColor: '#0ff0ff','width':300,'height':300};\n var chart = new google.visualization.LineChart(document.getElementById('chart_div'));\n chart.draw(data, options);}\n </script>\n"};

const char* ssid = "";
const char* password = "";

bool is_authentified() {
  if (server.hasHeader("Cookie")) {
    String cookie = server.header("Cookie");
    if (cookie.indexOf("ESPSESSIONID=1") != -1) {
      return true;
    }
  }
  return false;
}

void buildbuffer() {
  Buffer = "<html>";
  Buffer += "<body>";
  Buffer += tempBuffer;
  Buffer += "</body>";
  Buffer += "</html>";
}

void buildHome() {
  formatGraph();
  Home = "<!DOCTYPE html>\n";
  Home += Head;
  Home += addTo;
  Home += "<script> if (typeof(fg_widgets)===\"undefined\") fg_widgets = new Array();fg_widgets.push(\"fgid_a0ec8b91fc8c802cd5f649b8f\");</script>";
  Home += "<script src=\"http://www.feedgrabbr.com/widget/fgwidget.js\"></script>";
  Home += "<script type=\"text/javascript\" src=\"https://www.gstatic.com/charts/loader.js\"></script>";
  Home += Graph;
  Home += "</head>\n";
  Home += "<body>\n";
  Home += nav1;
  Home += "          <li class=\"active\"><a href=\"/\">Home</a></li>\n";
  Home += "          <li><a href=\"temp\">Temperatuursoverzicht</a></li>\n";
  Home += "          <li><a href=\"omdb\">OMDB</a></li>\n";
  Home += "          <li><a href=\"weather\">weather</a></li>\n";
  Home += nav2;
  Home += "          <li><a href=\"/profile\"><span class=\"glyphicon glyphicon-user\"></span> My Account</a></li>\n";
  Home += nav3;
  Home += "  <div class=\"container-fluid\">\n";
  Home += "    <div class=\"jumbotron text-center\">\n";
  Home += "      <h1>HOME ONDER CONSTRUCTIE...</h1>\n";
  Home += "    </div>\n";
  Home += "    <div class=\"row-fluid\">\n";
  Home += "       <div class=\"span2 feedgrabbr_widget\" id=\"fgid_a0ec8b91fc8c802cd5f649b8f\"></div>\n";
  Home += "       <div class=\"span10\">\n";
  Home += "         <div id=\"chart_div\"></div>\n";
  Home += "       </div>\n";
  Home += "    </div>\n";
  Home += " </div>\n";
  Home += "</body>\n";
  Home += "</html>\n";
  Serial.println(Home);
  /*
    File home = SPIFFS.open("/home.txt", "r");
    while (home.available()) {
    Home = profile.readStringUntil('\n');
    home.close();
    }
  */
}

void buildprofile() {
  File profile = SPIFFS.open("/profile.txt", "r");
  while (profile.available()) {
    Profile = profile.readStringUntil('\n');
    profile.close();
  }
}

void buildOmdb() {
  File omdb = SPIFFS.open("/omdb.txt", "r");
  while (omdb.available()) {
    Omdb = omdb.readStringUntil('\n');
    omdb.close();
  }
}

void buildWeather() {
  File weather = SPIFFS.open("/weather.txt", "r");
  while (weather.available()) {
    Weather = weather.readStringUntil('\n');
    weather.close();
  }
}

void buildTemp() {
  File temp = SPIFFS.open("/temp.txt", "r");
  while (temp.available()) {
    Temp = temp.readStringUntil('\n');
    temp.close();
  }
}

void buildXML() {
  XML = "<?xml version='1.0'?>";
  XML += "<Donnees>";
  XML += "<ThomasTEMP>";
  sensors.requestTemperatures();
  XML += sensors.getTempCByIndex(0);
  XML += "</ThomasTEMP>";
  XML += "</Donnees>";
}

void handleLogin() {
  if (server.hasHeader("Cookie")) {
    String cookie = server.header("Cookie");
  }
  if (server.hasArg("disconnect")) {
    String header = "HTTP/1.1 301 OK\r\nSet-Cookie: ESPSESSIONID=0\r\nLocation: /login\r\nCache-Control: no-cache\r\n\r\n";
    server.sendContent(header);
    return;
  }
  if (server.hasArg("USERNAME") && server.hasArg("PASSWORD")) {
    if (server.arg("USERNAME") == "Thomas" &&  server.arg("PASSWORD") == "Geudens" ) {
      String header = "HTTP/1.1 301 OK\r\nSet-Cookie: ESPSESSIONID=1\r\nLocation: /\r\nCache-Control: no-cache\r\n\r\n";
      server.sendContent(header);
      return;
    }
    msg = "<div class=\"alert alert-danger\">";
    msg += "<a href=\"#\" class=\"close\" data-dismiss=\"alert\" aria-label=\"close\">&times;</a>";
    msg += "<p class=\"text-center\" >Verkeerde gebruikersnaam of wachtwoord</p>";
    msg += "</div>";
  }

  login = login1;
  login += msg;
  login += login2;
  server.send(200, "text/html", login);
  login = "";
}

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
              }
            }
          }
        }
      }
    }
  }
}

void formatGraph() {
  File graphdata = SPIFFS.open("/graph.txt", "r");
  while (graphdata.available()) {
    graphBuffer = graphdata.readStringUntil('\n');
  }
  graphdata.close();
  Graph = graph1;
  Graph += graphBuffer;
  Graph += graph2;
  Serial.println(graphBuffer);
}

void graphData() {
  formatTempAndDate();
  graphBuffer += "[";
  graphBuffer += millis() / 1000;
  graphBuffer += ", ";
  graphBuffer += temperatuur;
  graphBuffer += "],";

  File graphdata = SPIFFS.open("/graph.txt", "a");
  graphdata.print(graphBuffer);
  graphdata.close();

  graphBuffer = "";
}

void handleProfile() {
  checkAuth();
  buildprofile();
  server.send(200, "text/html", Profile);
  Profile = "";
}

void handleTemp() {
  checkAuth();
  buildTemp();
  server.send(200, "text/html", Temp);
  Temp = "";
}

void handleWeather() {
  checkAuth();
  buildWeather();
  server.send(200, "text/html", Weather);
  Weather = "";
}

void handleXML() {
  checkAuth();
  buildXML();
  server.send(200, "text/xml", XML);
}

void handleHome() {
  checkAuth();
  buildHome();
  server.send(200, "text/html", Home);
}

void handleOmdb() {
  checkAuth();
  buildOmdb();
  server.send(200, "text/html", Omdb);
  Omdb = "";
}

void handleBuffer() {
  checkAuth();
  buildbuffer();
  server.send(200, "text/html", Buffer);
}

void handleNotFound() {
  String message = "404 File Not Found\n\n";
  server.send(404, "text/plain", message);
}

void checkAuth() {
  if (!is_authentified()) {
    String header = "HTTP/1.1 301 OK\r\nLocation: /login\r\nCache-Control: no-cache\r\n\r\n";
    server.sendContent(header);
    return;
  }
}

void sendTemperature(float temp)
{
  WiFiClient client;
  while (!client.connect(EIOT_CLOUD_ADDRESS, EIOT_CLOUD_PORT)) {
    Serial.println("connection failed");
  }
  String url = "";
  url += "/RestApi/SetParameter/" + String(EIOT_CLOUD_INSTANCE_PARAM_ID) + "/" + String(temp);
  Serial.println(url);
  client.print(String("POST ") + url + " HTTP/1.1\r\n" +
               "Host: " + String(EIOT_CLOUD_ADDRESS) + "\r\n" +
               "Connection: close\r\n" +
               "Content-Length: 0\r\n" +
               "\r\n");
  delay(100);
  while (client.available()) {
    String line = client.readStringUntil('\r');
  }
  Serial.println();
  Serial.println("Connection closed");
}

void setup(void) {
  Serial.begin(115200);

  sensors.begin();
  SPIFFS.begin();
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println(WiFi.localIP());

  server.on("/", handleHome);
  server.on("/temp", handleTemp);
  server.on("/omdb", handleOmdb);
  server.on("/xml", handleXML);
  server.on("/weather", handleWeather);
  server.on("/login", handleLogin);
  server.on("/profile", handleProfile);
  server.on("/tempbuffer", handleBuffer);
  server.onNotFound(handleNotFound);

  const char * headerkeys[] = {"User-Agent", "Cookie"} ;
  size_t headerkeyssize = sizeof(headerkeys) / sizeof(char*);
  server.collectHeaders(headerkeys, headerkeyssize );

  SPIFFS.remove("/graph.txt");

  server.begin();
}

void loop() {
  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;
    formatTempAndDate();
    tempBuffer += datumTijd;
    tempBuffer += "-->";
    tempBuffer += temperatuur;
    tempBuffer += "<br>";
    graphData();
    sendTemperature(temperatuur);
  }
  server.handleClient();
}
