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

#define ONE_WIRE_BUS 4
#define DEBUG_ESP_HTTP_SERVER true

OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);
DeviceAddress deviceAddress;

unsigned long previousMillis = 0;
const long interval = 10000;
String Temp, XML, login, Home, Omdb, Profile, tijd, tempBuffer, graphBuffer, Graph, uur, Weather, Buffer, datumTijd, theDate;
float temperatuur;

const char Head[] PROGMEM = {"<html>\n<head>\n<meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\"><title>Server</title><link rel=\"icon\" type=\"image/png\" href=\"https://image.freepik.com/free-icon/ssl-encryption_318-32078.png\"><link rel=\"stylesheet\" href=\"https://maxcdn.bootstrapcdn.com/bootstrap/3.3.6/css/bootstrap.min.css\" integrity=\"sha384-1q8mTJOASx8j1Au+a5WDVnPi2lkFfwwEAa8hDDdjZlpLegxhjVME1fgjWPGmkzs7\" crossorigin=\"anonymous\">\n<style>.footer {position: absolute; bottom: 0; width: 100%; height: 60px; background-color: #f5f5f5;} .well{ background-color: #99d6ff;}</style>\n<script src=\"https://ajax.googleapis.com/ajax/libs/jquery/1.12.2/jquery.min.js\"></script>\n<script src=\"http://maxcdn.bootstrapcdn.com/bootstrap/3.3.6/js/bootstrap.min.js\"></script>\n"};
const char javaScript[] PROGMEM = {"<script>\n var xmlHttp=createXmlHttpObject();\n function createXmlHttpObject(){\n if(window.XMLHttpRequest){\n xmlHttp=new XMLHttpRequest();\n }else{\n xmlHttp=new ActiveXObject('Microsoft.XMLHTTP');\n }\n return xmlHttp;\n }\n function process(){\n if(xmlHttp.readyState==0 || xmlHttp.readyState==4){\n xmlHttp.open('PUT','xml',true);\n xmlHttp.onreadystatechange=handleServerResponse;\n xmlHttp.send(null);\n }\n setTimeout('process()',1000);\n }\n function handleServerResponse(){\n if(xmlHttp.readyState==4 && xmlHttp.status==200){\n xmlResponse=xmlHttp.responseXML;\n xmldoc = xmlResponse.getElementsByTagName('ThomasTEMP');\n message = xmldoc[0].firstChild.nodeValue;\n  document.getElementById('TEMP').innerHTML=message;\n }\n }\n </script>\n"};
const char nav1[] PROGMEM = {"<nav class=\"navbar navbar-inverse navbar-fixed-top\">\n <div class=\"container-fluid\">\n <div class=\"navbar-header\">\n <button type=\"button\" class=\"navbar-toggle\" data-toggle=\"collapse\" data-target=\"#myNavbar\">\n <span class=\"icon-bar\"></span>\n <span class=\"icon-bar\"></span>\n <span class=\"icon-bar\"></span>\n </button>\n <a class=\"navbar-brand\" href=\"#\"><span class=\"glyphicon glyphicon-cloud\"></span></a>\n </div>\n <div class=\"collapse navbar-collapse\" id=\"myNavbar\">\n <ul class=\"nav navbar-nav\">\n"};
const char nav2[] PROGMEM = {"</ul>\n <form class=\"navbar-form navbar-right\" role=\"search\">\n <div class=\"form-group input-group\">\n <input type=\"text\" class=\"form-control\" placeholder=\"Search...\">\n <span class=\"input-group-btn\">\n <button class=\"btn btn-default\" type=\"button\">\n <span class=\"glyphicon glyphicon-search\"></span>\n </button>\n </span>\n </div>\n </form>\n <ul class=\"nav navbar-nav navbar-right\">\n"};
const char nav3[] PROGMEM = {"<li><a href=\"/login?disconnect=yes\">Afmelden</a></li>\n </ul>\n </div>\n </div>\n </nav>\n <br><br><br>\n"};
const char addTo[] PROGMEM = {"<script>\n function GetClock(){\n var d=new Date();\n var nmonth=d.getMonth(),ndate=d.getDate(),nyear=d.getYear();\n if(nyear<1000) nyear+=1900;\n var nhour=d.getHours(),nmin=d.getMinutes(),nsec=d.getSeconds();\n if(nmin<=9) nmin=\"0\"+nmin\n if(nsec<=9) nsec=\"0\"+nsec;\n document.getElementById('clockbox').innerHTML=\"\"+ndate+\"/\"+(nmonth+1)+\"/\"+nyear+\"     \"+nhour+\":\"+nmin+\":\"+nsec+\"\";\n }\n window.onload=function(){\n GetClock();\n setInterval(GetClock,1000);\n }\n </script>\n"};
const char OmdbJquery[] PROGMEM = {"<script>\n $(document).ready(function() {\n var $Form = $('form'),$Container = $('#movieInfo');\n $Container.hide();\n $Form.on('submit', function(p_oEvent) {\n var sUrl, x, oData;\n p_oEvent.preventDefault();\n x = $Form.find('#movieTitle').val();\n sUrl = 'http://www.omdbapi.com/?t=' + x + '&type=movie';\n $.ajax(sUrl, {\n complete: function(p_oXHR, p_sStatus) {\n oData = $.parseJSON(p_oXHR.responseText);\n $Container.find('.title').text(oData.Title);\n $Container.find('.plot').text(oData.Plot);\n $Container.find('.poster').html('<img src=\"' + oData.Poster + '\"/>');\n $Container.find('.released').text(oData.Released);\n $Container.find('.runtime').text(oData.Duration);\n $Container.find('.genre').text(oData.Genre);\n $Container.find('.director').text(oData.Director);\n $Container.find('.actors').text(oData.Actors);\n $Container.find('.imdbrating').text(oData.imdbrating);\n $Container.find('.year').text(oData.Year);\n $Container.show();\n }\n});\n});\n});\n"};
const char login1[] PROGMEM = {" <!DOCTYPE html>\n <html>\n <head>\n <meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\">\n <title>Log In</title>\n <link rel=\"icon\" type=\"image/png\" href=\"https://image.freepik.com/free-icon/ssl-encryption_318-32078.png\"> <link rel=\"stylesheet\" href=\"https://maxcdn.bootstrapcdn.com/bootstrap/3.3.6/css/bootstrap.min.css\" integrity=\"sha384-1q8mTJOASx8j1Au+a5WDVnPi2lkFfwwEAa8hDDdjZlpLegxhjVME1fgjWPGmkzs7\" crossorigin=\"anonymous\">\n <script src=\"https://ajax.googleapis.com/ajax/libs/jquery/1.12.2/jquery.min.js\"></script>\n <script src=\"http://maxcdn.bootstrapcdn.com/bootstrap/3.3.6/js/bootstrap.min.js\"></script>\n </head>\n <body>\n <div id=\"loginModal\" class=\"modal show\" tabindex=\"-1\" role=\"dialog\" aria-hidden=\"true\">\n <div class=\"modal-dialog\">\n <div class=\"modal-content\">\n <div class=\"modal-header\">\n <h1 class=\"text-center\">Login</h1>\n </div>\n <div class=\"modal-body\">\n <form class=\"form center-block\" action=\"/login\" method=\"POST\">\n <div class=\"form-group\">\n <input type=\"text\" class=\"form-control input-lg\" placeholder=\"Username\" name=\"USERNAME\">\n </div>\n <div class=\"form-group\">\n <input type=\"password\" class=\"form-control input-lg\" placeholder=\"Password\" name=\"PASSWORD\">\n </div>\n <div class=\"form-group\">\n <button class=\"btn btn-primary btn-lg btn-block\" name=\"SUBMIT\">Log In</button>\n </div>\n </form>\n </div>\n <div class=\"modal-footer\">\n"};
const char login2[] PROGMEM = {"<div class=\"col-md-12\">\n </div>\n </div>\n </div>\n </div>\n </div>\n </body>\n </html>\n"};
const char graph1[] PROGMEM = {"<script>\ngoogle.charts.load('current', {packages: ['corechart', 'line']});\n google.charts.setOnLoadCallback(drawBackgroundColor);\n function drawBackgroundColor() {\n var data = new google.visualization.DataTable();\n data.addColumn('number', 'time');\n data.addColumn('number', 'graden celsius');\n data.addRows([\n"};
const char graph2[] PROGMEM = {"]);\n var options = {\n hAxis: {title: 'Time'},vAxis: {title: 'Temperature'},backgroundColor: '#0ff0ff','width':300,'height':300};\n var chart = new google.visualization.LineChart(document.getElementById('chart_div'));\n chart.draw(data, options);}\n </script>\n"};
const char movie[] PROGMEM = {"<div id=\"movieInfo\"> <p>Title:</p> <div class=\"title\"></div> <p>Released:</p>\n <div class=\"released\"></div> <p>Duration:</p> <div class=\"runtime\"></div> <p>Genre:</p> <div class=\"genre\"></div> <p>Director:</p> <div class=\"director\"></div> <p>Actors</p> <div class=\"actors\"></div> <p>Plot</p> <div class=\"plot\"></div> <p>Poster:</p>\n <div class=\"poster\"></div>\n <p>Rating on IMDB</p> <div class=\"imdbrating\"></div> </div>\n <form>\n <label>\n <input id=\"movieTitle\"type=\"text\" name=\"movie\" placeholder=\"movie title\" />\n </label>\n <button>go!</button>\n </form>\n"};
const char movie[] PROGMEM = {"<div class=\"container text-center\">\n <div class=\"jumbotron\">\n <h1>Overzicht van de temperaturen</h1>\n <p>Welkom op de pagina met de verschillende temperaturen!</p>\n </div>\n <div class=\"col-sm-4\">\n <div class=\"well\">\n <h3>Zwijndrecht:</h3>\n <a href=\"http://www.accuweather.com/nl/be/zwijndrecht/26971/weather-forecast/26971\" class=\"aw-widget-legal\"></a><div id=\"awcc1464426505520\" class=\"aw-widget-current\"  data-locationkey=\"26971\" data-unit=\"c\" data-language=\"en-us\" data-useip=\"false\" data-uid=\"awcc1464426505520\"></div><script type=\"text/javascript\" src=\"http://oap.accuweather.com/launch.js\"></script> </div>\n <div class=\"well\">\n <h3>Bornem:</h3>\n <a href=\"http://www.accuweather.com/nl/be/bornem/964215/weather-forecast/964215\" class=\"aw-widget-legal\"></a><div id=\"awcc1464259420008\" class=\"aw-widget-current\"  data-locationkey=\"964215\" data-unit=\"c\" data-language=\"en-us\" data-useip=\"false\" data-uid=\"awcc1464259420008\"></div><script type=\"text/javascript\" src=\"http://oap.accuweather.com/launch.js\"></script> </div>\n </div>\n <div class=\"col-sm-4\">\n <div class=\"well\">\n  <h3>Mechelen:</h3>\n <a href=\"http://www.accuweather.com/nl/be/mechelen/27047/weather-forecast/27047\" class=\"aw-widget-legal\"></a><div id=\"awcc1464426662522\" class=\"aw-widget-current\"  data-locationkey=\"27047\" data-unit=\"c\" data-language=\"en-us\" data-useip=\"false\" data-uid=\"awcc1464426662522\"></div><script type=\"text/javascript\" src=\"http://oap.accuweather.com/launch.js\"></script> </div>\n <div class=\"well\">\n <h3>Willebroek:</h3>\n <a href=\"http://www.accuweather.com/nl/be/willebroek/954598/weather-forecast/954598\" class=\"aw-widget-legal\"></a><div id=\"awcc1464426778796\" class=\"aw-widget-current\"  data-locationkey=\"954598\" data-unit=\"c\" data-language=\"en-us\" data-useip=\"false\" data-uid=\"awcc1464426778796\"></div><script type=\"text/javascript\" src=\"http://oap.accuweather.com/launch.js\"></script> </div>\n </div>\n <div class=\"col-sm-4\">\n <div class=\"well\">\n <h3>New York:</h3>\n <a href=\"http://www.accuweather.com/nl/us/new-york-ny/10007/weather-forecast/349727\" class=\"aw-widget-legal\"></a><div id=\"awcc1464426868692\" class=\"aw-widget-current\"  data-locationkey=\"349727\" data-unit=\"c\" data-language=\"en-us\" data-useip=\"false\" data-uid=\"awcc1464426868692\"></div><script type=\"text/javascript\" src=\"http://oap.accuweather.com/launch.js\"></script> </div>\n <div class=\"well\">\n <h3>Cuba:</h3>\n <a href=\"http://www.accuweather.com/nl/cu/havana/122438/weather-forecast/122438\" class=\"aw-widget-legal\"></a><div id=\"awcc1464427011561\" class=\"aw-widget-current\"  data-locationkey=\"122438\" data-unit=\"c\" data-language=\"en-us\" data-useip=\"false\" data-uid=\"awcc1464427011561\"></div><script type=\"text/javascript\" src=\"http://oap.accuweather.com/launch.js\"></script> </div>\n </div>\n </div>\n"};
const char temp[] PROGMEM = {"<div class=\"container text-center\">\n <div class=\"jumbotron\">\n <h1>Overzicht van de temperaturen</h1>\n <p>Welkom op de pagina met de verschillende temperaturen!</p>\n </div>\n <div class=\"col-sm-4\">\n <div class=\"well\">\n <h3>Kamer Thomas</h3>\n <p id='TEMP'>Temperatuur wordt opgevraagd...</p>\n </div>\n <div class=\"well\">\n <h3>Keuken</h3>\n <p>Lorem ipsum dolor ...</p>\n </div>\n </div>\n <div class=\"col-sm-4\">\n <div class=\"well\">\n <h3>Kamer Marc</h3>\n <p>Lorem ipsum dolor ...</p>\n </div>\n <div class=\"well\">\n <h3>Woonkamer</h3>\n <p>Lorem ipsum dolor ...</p>\n </div>\n </div>\n <div class=\"col-sm-4\">\n <div class=\"well\">\n <h3>Badkamer</h3>\n <p>Lorem ipsum dolor ...</p>\n </div>\n <div class=\"well\">\n <h3>Berging</h3>\n <p>Lorem ipsum dolor ...</p>\n </div>\n </div>\n </div>\n"};

const char* ssid = "";
const char* password = "";
//const char* password = "9Hu4MX3S6821";

ESP8266WebServer server(80);

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
  /*Home += "  <footer class = \"footer\">\n";
    Home += "    <div class=\"container\"><br>\n";
    Home += "      <p class =\"pull-left\" id=\"clockbox\"></p>\n";
    Home += "    </div>\n";
    Home += "  </footer>\n"; */
  Home += "</body>\n";
  Home += "</html>\n";
}

void buildprofile() {
  Profile = "<!DOCTYPE html>\n";
  Profile += Head;
  Profile += addTo;
  Profile += "</head>\n";
  Profile += "<body>\n";
  Profile += nav1;
  Profile += "          <li><a href=\"/\">Home</a></li>\n";
  Profile += "          <li><a href=\"temp\">Temperatuursoverzicht</a></li>\n";
  Profile += "          <li><a href=\"omdb\">OMDB</a></li>\n";
  Profile += "          <li><a href=\"weather\">weather</a></li>\n";
  Profile += nav2;
  Profile += "          <li class=\"active\"><a href=\"profile\"><span class=\"glyphicon glyphicon-user\"></span> My Account</a></li>\n";
  Profile += nav3;
  Profile += "  <div class=\"container text-center\">\n";
  Profile += "    <div class=\"jumbotron\">\n";
  Profile += "      <h1>PROFILE ONDER CONSTRUCTIE...</h1>\n";
  Profile += "    </div>\n";
  Profile += "  </div>\n";
  /*Profile += "  <footer class=\"footer\">\n";
    Profile += "    <div class=\"container\"><br>\n";
    Profile += "      <p class =\"pull-left\" id=\"clockbox\"></p>\n";
    Profile += "    </div>\n";
    Profile += "  </footer>\n";*/
  Profile += "</body>\n";
  Profile += "</html>\n";
}

void buildOmdb() {
  Omdb = "<!DOCTYPE html>\n";
  Omdb += Head;
  Omdb += addTo;
  Omdb += OmdbJquery;
  Omdb += "</script>\n";
  Omdb += "</head>\n";
  Omdb += "<body>\n";
  Omdb += nav1;
  Omdb += "          <li><a href=\"/\">Home</a></li>\n";
  Omdb += "          <li><a href=\"temp\">Temperatuursoverzicht</a></li>\n";
  Omdb += "          <li class=\"active\"><a href=\"omdb\">OMDB</a></li>\n";
  Omdb += "          <li><a href=\"weather\">weather</a></li>\n";
  Omdb += nav2;
  Omdb += "          <li><a href=\"profile\"><span class=\"glyphicon glyphicon-user\"></span> My Account</a></li>\n";
  Omdb += nav3;
  Omdb += "  <div class=\"container text-center\">\n";
  Omdb += "    <div class=\"jumbotron\">\n";
  Omdb += "      <h1>OMDB ONDER CONSTRUCTIE...</h1>\n";
  Omdb += "    </div>\n";
  Omdb += "  </div>\n";
  Omdb += movie;
  Omdb += "  <footer class=\"footer\">\n";
  Omdb += "    <div class=\"container\"><br>\n";
  Omdb += "      <p class =\"pull-left\" id=\"clockbox\"></p>\n";
  Omdb += "    </div>\n";
  Omdb += "  </footer>\n";
  Omdb += "</body>\n";
  Omdb += "</html>\n";
}

void buildWeather() {
  Weather = "<!DOCTYPE html>\n";
  Weather += Head;
  Weather += addTo;
  Weather += "</head>\n";
  Weather += "<body>\n";
  Weather += nav1;
  Weather += "          <li><a href=\"/\">Home</a></li>\n";
  Weather += "          <li><a href=\"temp\">Temperatuursoverzicht</a></li>\n";
  Weather += "          <li><a href=\"omdb\">OMDB</a></li>\n";
  Weather += "          <li class=\"active\"><a href=\"weather\">weather</a></li>\n";
  Weather += nav2;
  Weather += "          <li><a href=\"profile\"><span class=\"glyphicon glyphicon-user\"></span> My Account</a></li>\n";
  Weather += nav3;
  Weather += weer;
  /*Weather += "  <footer class=\"footer\">\n";
    Weather += "    <div class=\"container\"><br>\n";
    Weather += "      <p class =\"pull-left\" id=\"clockbox\"></p>\n";
    Weather += "    </div>\n";
    Weather += "  </footer>\n";*/
  Weather += "</body>\n";
  Weather += "</html>\n";
}

void buildTemp() {
  Temp = "<!DOCTYPE html>\n";
  Temp += Head;
  Temp += addTo;
  Temp += "</head>\n";
  Temp += javaScript;
  Temp += "<body onload='process()'>\n";
  Temp += nav1;
  Temp += "          <li><a href=\"/\">Home</a></li>\n";
  Temp += "          <li class=\"active\"><a href=\"temp\">Temperatuursoverzicht</a></li>\n";
  Temp += "          <li><a href=\"omdb\">OMDB</a></li>\n";
  Temp += "          <li><a href=\"weather\">weather</a></li>\n";
  Temp += nav2;
  Temp += "          <li><a href=\"/profile\"><span class=\"glyphicon glyphicon-user\"></span> My Account</a></li>\n";
  Temp += nav3;
  Temp += temp;
  /*Temp += "  <footer class=\"footer\">\n";
    Temp += "    <div class=\"container\"><br>\n";
    Temp += "      <p class =\"pull-left\" id=\"clockbox\"></p>\n";
    Temp += "    </div>\n";
    Temp += "  </footer>\n";*/
  Temp += "</body>\n";
  Temp += "</html>\n";
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
  String msg;
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
}

void formatGraph() {
  Graph = graph1;
  Graph += graphBuffer;
  Graph += graph2;
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
}}}}}}}}

void handleProfile() {
  checkAuth();
  buildprofile();
  server.send(200, "text/html", Profile);
}

void handleTemp() {
  checkAuth();
  buildTemp();
  server.send(200, "text/html", Temp);
}

void handleWeather() {
  checkAuth();
  buildWeather();
  server.send(200, "text/html", Weather);
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

void setup(void) {
  Serial.begin(115200);
  digitalWrite(1, 0);
  sensors.begin();
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

  server.begin();
}

void loop() {
  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;
    formatTempAndDate();
    formatGraph();
    tempBuffer += datumTijd;
    tempBuffer += "-->";
    tempBuffer += temperatuur;
    tempBuffer += "<br>";
    if (tempBuffer.length() > 20000) {
      tempBuffer = "";
    }
    graphBuffer += "[";
    graphBuffer += millis()/1000;
    graphBuffer += ", ";
    graphBuffer += temperatuur;
    graphBuffer += "],";
    Serial.println(graphBuffer);
  }
  server.handleClient();
}
