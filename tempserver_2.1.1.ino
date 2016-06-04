/* 
 *  TODO:
 *  DATABASE  
 *  ADD USER VIA LOGIN PAGE
 *  LOGIN FROM DATABASE
 *  GET USER PROFILE DATA
 *  GET OMDB WORKING
 *  MAKE HOME PAGE
 *  SEND TEMPS TO SPREADSHEET
 *  SEND FROM SPREADSHEET TO EMAIL
 *  ADD MULTIPLE TEMPSENSORS FUNCTIONALITY
*/
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <ESP8266WiFiMesh.h>
#include <WiFiUdp.h>
#include <OneWire.h>
#include <DallasTemperature.h>

#define ONE_WIRE_BUS 4

String manageRequest(String request);

OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);
DeviceAddress deviceAddress;
ESP8266WiFiMesh mesh_node = ESP8266WiFiMesh(ESP.getChipId(), manageRequest);

unsigned long previousMillis = 0;
const long interval = 10000;
String Temp, javaScript, XML, bootStrap, addTo, nav1, nav2, nav3, OmdbJquery, login, Home, Omdb, Profile, tijd, tempBuffer, uur, Weather, Buffer, Email, datumTijd, theDate, Head;
float temperatuur;

String manageRequest(String request)
{
  Serial.print("received: ");
  Serial.println(request);
}


const char* ssid = "";
const char* password = "";

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
  //Buffer += "";
  Buffer = "<html>";
  Buffer += "<body>";
  Buffer += tempBuffer;
  Buffer += "</body>";
  Buffer += "</html>";
}

void buildHome() {
  //Home += "";
  clockString();
  navbar1();
  navbar2();
  navbar3();
  buildHead();
  Home = "<!DOCTYPE html>\n";
  Home += Head;
  Home += addTo;
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
  Home += "  <div class=\"container text-center\">\n";
  Home += "    <div class=\"jumbotron\">\n";
  Home += "      <h1>HOME ONDER CONSTRUCTIE...</h1>\n";
  Home += "    </div>\n";
  Home += "  </div>\n";
  /*Home += "  <footer class=\"footer\">\n";
    Home += "    <div class=\"container\"><br>\n";
    Home += "      <p class =\"pull-left\" id=\"clockbox\"></p>\n";
    Home += "    </div>\n";
    Home += "  </footer>\n";*/
  Home += "</body>\n";
  Home += "</html>\n";
}

void buildprofile() {
  clockString();
  navbar1();
  navbar2();
  navbar3();
  buildHead();
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
  //Omdb += "";
  clockString();
  navbar1();
  navbar2();
  navbar3();
  OmdbJavascript();
  buildHead();
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
  //test HTML
  Omdb += "   <div id=\"movieInfo\">\n";
  Omdb += "   <p>Title:</p>\n";
  Omdb += "   <div class=\"title\"></div>\n";
  Omdb += "   <p>Released:</p>\n";
  Omdb += "   <div class=\"released\"></div>\n";
  Omdb += "   <p>Duration:</p>\n";
  Omdb += "   <div class=\"runtime\"></div>\n";
  Omdb += "   <p>Genre:</p>\n";
  Omdb += "   <div class=\"genre\"></div>\n";
  Omdb += "   <p>Director:</p>\n";
  Omdb += "   <div class=\"director\"></div>\n";
  Omdb += "   <p>Actors</p>\n";
  Omdb += "   <div class=\"actors\"></div>\n";
  Omdb += "   <p>Plot</p>\n";
  Omdb += "   <div class=\"plot\"></div>\n";
  Omdb += "   <p>Poster:</p>\n";
  Omdb += "   <div class=\"poster\"></div>\n";
  Omdb += "   <p>Rating on IMDB</p>\n";
  Omdb += "   <div class=\"imdbrating\"></div>\n";
  Omdb += "   </div>\n";
  Omdb += "   <form>\n";
  Omdb += "     <label>\n";
  Omdb += "       <input type=\"text\" name=\"movie\" placeholder=\"movie title\" />\n";
  Omdb += "     </label>\n";
  Omdb += "   <button>go!</button>\n";
  Omdb += " </form>\n";
  //test HTML
  /*Omdb += "  <footer class=\"footer\">\n";
    Omdb += "    <div class=\"container\"><br>\n";
    Omdb += "      <p class =\"pull-left\" id=\"clockbox\"></p>\n";
    Omdb += "    </div>\n";
    Omdb += "  </footer>\n";*/
  Omdb += "</body>\n";
  Omdb += "</html>\n";
}

void buildWeather() {
  clockString();
  navbar1();
  navbar2();
  navbar3();
  buildHead();
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
  Weather += "  <div class=\"container text-center\">\n";
  Weather += "    <div class=\"jumbotron\">\n";
  Weather += "      <h1>Overzicht van de temperaturen</h1>\n";
  Weather += "      <p>Welkom op de pagina met de verschillende temperaturen!</p>\n";
  Weather += "    </div>\n";
  Weather += "    <div class=\"col-sm-4\">\n";
  Weather += "      <div class=\"well\">\n";
  Weather += "        <h3>Zwijndrecht:</h3>\n";
  Weather += "        <a href=\"http://www.accuweather.com/nl/be/zwijndrecht/26971/weather-forecast/26971\" class=\"aw-widget-legal\"></a><div id=\"awcc1464426505520\" class=\"aw-widget-current\"  data-locationkey=\"26971\" data-unit=\"c\" data-language=\"en-us\" data-useip=\"false\" data-uid=\"awcc1464426505520\"></div><script type=\"text/javascript\" src=\"http://oap.accuweather.com/launch.js\"></script>";
  Weather += "      </div>\n";
  Weather += "      <div class=\"well\">\n";
  Weather += "        <h3>Bornem:</h3>\n";
  Weather += "        <a href=\"http://www.accuweather.com/nl/be/bornem/964215/weather-forecast/964215\" class=\"aw-widget-legal\"></a><div id=\"awcc1464259420008\" class=\"aw-widget-current\"  data-locationkey=\"964215\" data-unit=\"c\" data-language=\"en-us\" data-useip=\"false\" data-uid=\"awcc1464259420008\"></div><script type=\"text/javascript\" src=\"http://oap.accuweather.com/launch.js\"></script>";
  Weather += "      </div>\n";
  Weather += "    </div>\n";
  Weather += "    <div class=\"col-sm-4\">\n";
  Weather += "      <div class=\"well\">\n";
  Weather += "        <h3>Mechelen:</h3>\n";
  Weather += "        <a href=\"http://www.accuweather.com/nl/be/mechelen/27047/weather-forecast/27047\" class=\"aw-widget-legal\"></a><div id=\"awcc1464426662522\" class=\"aw-widget-current\"  data-locationkey=\"27047\" data-unit=\"c\" data-language=\"en-us\" data-useip=\"false\" data-uid=\"awcc1464426662522\"></div><script type=\"text/javascript\" src=\"http://oap.accuweather.com/launch.js\"></script>";
  Weather += "      </div>\n";
  Weather += "      <div class=\"well\">\n";
  Weather += "        <h3>Willebroek:</h3>\n";
  Weather += "        <a href=\"http://www.accuweather.com/nl/be/willebroek/954598/weather-forecast/954598\" class=\"aw-widget-legal\"></a><div id=\"awcc1464426778796\" class=\"aw-widget-current\"  data-locationkey=\"954598\" data-unit=\"c\" data-language=\"en-us\" data-useip=\"false\" data-uid=\"awcc1464426778796\"></div><script type=\"text/javascript\" src=\"http://oap.accuweather.com/launch.js\"></script>";
  Weather += "      </div>\n";
  Weather += "    </div>\n";
  Weather += "    <div class=\"col-sm-4\">\n";
  Weather += "      <div class=\"well\">\n";
  Weather += "        <h3>New York:</h3>\n";
  Weather += "        <a href=\"http://www.accuweather.com/nl/us/new-york-ny/10007/weather-forecast/349727\" class=\"aw-widget-legal\"></a><div id=\"awcc1464426868692\" class=\"aw-widget-current\"  data-locationkey=\"349727\" data-unit=\"c\" data-language=\"en-us\" data-useip=\"false\" data-uid=\"awcc1464426868692\"></div><script type=\"text/javascript\" src=\"http://oap.accuweather.com/launch.js\"></script>";
  Weather += "      </div>\n";
  Weather += "      <div class=\"well\">\n";
  Weather += "        <h3>Cuba:</h3>\n";
  Weather += "        <a href=\"http://www.accuweather.com/nl/cu/havana/122438/weather-forecast/122438\" class=\"aw-widget-legal\"></a><div id=\"awcc1464427011561\" class=\"aw-widget-current\"  data-locationkey=\"122438\" data-unit=\"c\" data-language=\"en-us\" data-useip=\"false\" data-uid=\"awcc1464427011561\"></div><script type=\"text/javascript\" src=\"http://oap.accuweather.com/launch.js\"></script>";
  Weather += "      </div>\n";
  Weather += "    </div>\n";
  Weather += "  </div>\n";
  /*Weather += "  <footer class=\"footer\">\n";
    Weather += "    <div class=\"container\"><br>\n";
    Weather += "      <p class =\"pull-left\" id=\"clockbox\"></p>\n";
    Weather += "    </div>\n";
    Weather += "  </footer>\n";*/
  Weather += "</body>\n";
  Weather += "</html>\n";
}

void buildTemp() {
  //Temp += "";
  clockString();
  navbar1();
  navbar2();
  navbar3();
  OmdbJavascript();
  buildHead();
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
  Temp += "  <div class=\"container text-center\">\n";
  Temp += "    <div class=\"jumbotron\">\n";
  Temp += "      <h1>Overzicht van de temperaturen</h1>\n";
  Temp += "      <p>Welkom op de pagina met de verschillende temperaturen!</p>\n";
  Temp += "    </div>\n";
  Temp += "    <div class=\"col-sm-4\">\n";
  Temp += "      <div class=\"well\">\n";
  Temp += "        <h3>Kamer Thomas</h3>\n";
  Temp += "        <p id='TEMP'>Temperatuur wordt opgevraagd...</p>\n";
  Temp += "      </div>\n";
  Temp += "      <div class=\"well\">\n";
  Temp += "        <h3>Keuken</h3>\n";
  Temp += "        <p>Lorem ipsum dolor ...</p>\n";
  Temp += "      </div>\n";
  Temp += "    </div>\n";
  Temp += "    <div class=\"col-sm-4\">\n";
  Temp += "      <div class=\"well\">\n";
  Temp += "        <h3>Kamer Marc</h3>\n";
  Temp += "        <p>Lorem ipsum dolor ...</p>\n";
  Temp += "      </div>\n";
  Temp += "      <div class=\"well\">\n";
  Temp += "        <h3>Woonkamer</h3>\n";
  Temp += "        <p>Lorem ipsum dolor ...</p>\n";
  Temp += "      </div>\n";
  Temp += "    </div>\n";
  Temp += "    <div class=\"col-sm-4\">\n";
  Temp += "      <div class=\"well\">\n";
  Temp += "        <h3>Badkamer</h3>\n";
  Temp += "        <p>Lorem ipsum dolor ...</p>\n";
  Temp += "      </div>\n";
  Temp += "      <div class=\"well\">\n";
  Temp += "        <h3>Berging</h3>\n";
  Temp += "        <p>Lorem ipsum dolor ...</p>\n";
  Temp += "      </div>\n";
  Temp += "    </div>\n";
  Temp += "  </div>\n";
  /*Temp += "  <footer class=\"footer\">\n";
    Temp += "    <div class=\"container\"><br>\n";
    Temp += "      <p class =\"pull-left\" id=\"clockbox\"></p>\n";
    Temp += "    </div>\n";
    Temp += "  </footer>\n";*/
  Temp += "</body>\n";
  Temp += "</html>\n";
}

void navbar1() {
  //nav1 = "";
  nav1 = "  <nav class=\"navbar navbar-inverse navbar-fixed-top\">\n";
  nav1 += "    <div class=\"container-fluid\">\n";
  nav1 += "      <div class=\"navbar-header\">\n";
  nav1 += "        <button type=\"button\" class=\"navbar-toggle\" data-toggle=\"collapse\" data-target=\"#myNavbar\">\n";
  nav1 += "          <span class=\"icon-bar\"></span>\n";
  nav1 += "          <span class=\"icon-bar\"></span>\n";
  nav1 += "          <span class=\"icon-bar\"></span>\n";
  nav1 += "        </button>\n";
  nav1 += "        <a class=\"navbar-brand\" href=\"#\"><span class=\"glyphicon glyphicon-cloud\"></span></a>\n";
  nav1 += "      </div>\n";
  nav1 += "      <div class=\"collapse navbar-collapse\" id=\"myNavbar\">\n";
  nav1 += "        <ul class=\"nav navbar-nav\">\n";
}

void navbar2() {
  //nav2 = "";
  nav2 = "        </ul>\n";
  nav2 += "        <form class=\"navbar-form navbar-right\" role=\"search\">\n";
  nav2 += "          <div class=\"form-group input-group\">\n";
  nav2 += "            <input type=\"text\" class=\"form-control\" placeholder=\"Search...\">\n";
  nav2 += "            <span class=\"input-group-btn\">\n";
  nav2 += "              <button class=\"btn btn-default\" type=\"button\">\n";
  nav2 += "                <span class=\"glyphicon glyphicon-search\"></span>\n";
  nav2 += "              </button>\n";
  nav2 += "            </span>\n";
  nav2 += "          </div>\n";
  nav2 += "        </form>\n";
  nav2 += "        <ul class=\"nav navbar-nav navbar-right\">\n";
}

void navbar3() {
  //nav3 = "";
  nav3 = "          <li><a href=\"/login?disconnect=yes\">Afmelden</a></li>\n";
  nav3 += "        </ul>\n";
  nav3 += "      </div>\n";
  nav3 += "    </div>\n";
  nav3 += "  </nav>\n";
  nav3 += "  <br><br><br>\n";
}

void clockString() {
  addTo = "  <script>\n";
  addTo += "    function GetClock(){\n";
  addTo += "      var d=new Date();\n";
  addTo += "      var nmonth=d.getMonth(),ndate=d.getDate(),nyear=d.getYear();\n";
  addTo += "      if(nyear<1000) nyear+=1900;\n";
  addTo += "      var nhour=d.getHours(),nmin=d.getMinutes(),nsec=d.getSeconds();\n";
  addTo += "      if(nmin<=9) nmin=\"0\"+nmin\n";
  addTo += "      if(nsec<=9) nsec=\"0\"+nsec;\n";
  addTo += "      document.getElementById('clockbox').innerHTML=\"\"+ndate+\"/\"+(nmonth+1)+\"/\"+nyear+\"     \"+nhour+\":\"+nmin+\":\"+nsec+\"\";\n";
  addTo += "    }\n";
  addTo += "    window.onload=function(){\n";
  addTo += "      GetClock();\n";
  addTo += "      setInterval(GetClock,1000);\n";
  addTo += "    }\n";
  addTo += "</script>\n";
}

void OmdbJavascript() {
  OmdbJquery += "<script>\n";
  OmdbJquery += "var $Form = $('form'),$Container = $('#movieInfo');\n";
  OmdbJquery += "$Container.hide();\n";
  OmdbJquery += "$Form.on('submit', function(p_oEvent) {\n";
  OmdbJquery += "var sUrl, x, oData;\n";
  OmdbJquery += "p_oEvent.preventDefault();\n";
  OmdbJquery += "x = $Form.find('input').val();\n";
  OmdbJquery += "sUrl = 'http://www.omdbapi.com/?t=' + x + '&type=movie'\n";
  OmdbJquery += "$.ajax(sUrl, {\n";
  OmdbJquery += "complete: function(p_oXHR, p_sStatus) {\n";
  OmdbJquery += "oData = $.parseJSON(p_oXHR.responseText);\n";
  OmdbJquery += "$Container.find('.title').text(oData.Title);\n";
  OmdbJquery += "$Container.find('.plot').text(oData.Plot);\n";
  OmdbJquery += "$Container.find('.poster').html('<img src=\"' + oData.Poster + '\"/>');\n";
  OmdbJquery += "$Container.find('.released').text(oData.Released);\n";
  OmdbJquery += "$Container.find('.runtime').text(oData.Duration);\n";
  OmdbJquery += "$Container.find('.genre').text(oData.Genre);\n";
  OmdbJquery += "$Container.find('.director').text(oData.Director);\n";
  OmdbJquery += "$Container.find('.actors').text(oData.Actors);\n";
  OmdbJquery += "$Container.find('.imdbrating').text(oData.imdbrating);\n";
  OmdbJquery += "$Container.find('.year').text(oData.Year);\n";
  OmdbJquery += "$Container.show();\n";
  OmdbJquery += "}\n});\n});\n";
}

void buildHead() {
  Head = "<html>\n";
  Head += "<head>\n";
  Head += "  <meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\">\n";
  Head += "  <title>Server</title>\n";
  //Head += "  <link rel=\"icon\" type=\"image/png\" href=\"https://image.freepik.com/free-icon/ssl-encryption_318-32078.png\">";
  Head += "  <link rel=\"stylesheet\" href=\"https://maxcdn.bootstrapcdn.com/bootstrap/3.3.6/css/bootstrap.min.css\" integrity=\"sha384-1q8mTJOASx8j1Au+a5WDVnPi2lkFfwwEAa8hDDdjZlpLegxhjVME1fgjWPGmkzs7\" crossorigin=\"anonymous\">\n";
  Head += "  <style>.footer {position: absolute; bottom: 0; width: 100%; height: 60px; background-color: #f5f5f5;} .well{ background-color: #99d6ff;}</style>";
  Head += "  <script src=\"https://ajax.googleapis.com/ajax/libs/jquery/1.12.2/jquery.min.js\"></script>\n";
  Head += "  <script src=\"http://maxcdn.bootstrapcdn.com/bootstrap/3.3.6/js/bootstrap.min.js\"></script>\n";
}

void buildJavascript() {
  javaScript = "<script>\n";
  javaScript += "var xmlHttp=createXmlHttpObject();\n";
  javaScript += "function createXmlHttpObject(){\n";
  javaScript += " if(window.XMLHttpRequest){\n";
  javaScript += "    xmlHttp=new XMLHttpRequest();\n";
  javaScript += " }else{\n";
  javaScript += "    xmlHttp=new ActiveXObject('Microsoft.XMLHTTP');\n";
  javaScript += " }\n";
  javaScript += " return xmlHttp;\n";
  javaScript += "}\n";
  javaScript += "function process(){\n";
  javaScript += " if(xmlHttp.readyState==0 || xmlHttp.readyState==4){\n";
  javaScript += "   xmlHttp.open('PUT','xml',true);\n";
  javaScript += "   xmlHttp.onreadystatechange=handleServerResponse;\n";
  javaScript += "   xmlHttp.send(null);\n";
  javaScript += " }\n";
  javaScript += " setTimeout('process()',1000);\n";
  javaScript += "}\n";
  javaScript += "function handleServerResponse(){\n";
  javaScript += " if(xmlHttp.readyState==4 && xmlHttp.status==200){\n";
  javaScript += "   xmlResponse=xmlHttp.responseXML;\n";
  javaScript += "   xmldoc = xmlResponse.getElementsByTagName('ThomasTEMP');\n";
  javaScript += "   message = xmldoc[0].firstChild.nodeValue;\n";
  javaScript += "   document.getElementById('TEMP').innerHTML=message;\n";
  javaScript += " }\n";
  javaScript += "}\n";
  javaScript += "</script>\n";
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

  //login += "";
  login = "<!DOCTYPE html>\n";
  login += "<html>\n";
  login += "<head>\n";
  login += "  <meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\">\n";
  login += "  <title>Log In</title>\n";
  login += "  <link rel=\"icon\" type=\"image/png\" href=\"https://image.freepik.com/free-icon/ssl-encryption_318-32078.png\">";
  login += "  <link rel=\"stylesheet\" href=\"https://maxcdn.bootstrapcdn.com/bootstrap/3.3.6/css/bootstrap.min.css\" integrity=\"sha384-1q8mTJOASx8j1Au+a5WDVnPi2lkFfwwEAa8hDDdjZlpLegxhjVME1fgjWPGmkzs7\" crossorigin=\"anonymous\">\n";
  login += "  <script src=\"https://ajax.googleapis.com/ajax/libs/jquery/1.12.2/jquery.min.js\"></script>\n";
  login += "  <script src=\"http://maxcdn.bootstrapcdn.com/bootstrap/3.3.6/js/bootstrap.min.js\"></script>\n";
  login += "</head>\n";
  login += "<body>\n";
  login += "  <div id=\"loginModal\" class=\"modal show\" tabindex=\"-1\" role=\"dialog\" aria-hidden=\"true\">\n";
  login += "    <div class=\"modal-dialog\">\n";
  login += "      <div class=\"modal-content\">\n";
  login += "        <div class=\"modal-header\">\n";
  login += "          <h1 class=\"text-center\">Login</h1>\n";
  login += "        </div>\n";
  login += "        <div class=\"modal-body\">\n";
  login += "          <form class=\"form center-block\" action=\"/login\" method=\"POST\">\n";
  login += "            <div class=\"form-group\">\n";
  login += "              <input type=\"text\" class=\"form-control input-lg\" placeholder=\"Username\" name=\"USERNAME\">\n";
  login += "            </div>\n";
  login += "            <div class=\"form-group\">\n";
  login += "              <input type=\"password\" class=\"form-control input-lg\" placeholder=\"Password\" name=\"PASSWORD\">\n";
  login += "            </div>\n";
  login += "            <div class=\"form-group\">\n";
  login += "              <button class=\"btn btn-primary btn-lg btn-block\" name=\"SUBMIT\">Log In</button>\n";
  login += "          </div>\n";
  login += "        </form>\n";
  login += "      </div>\n";
  login += "      <div class=\"modal-footer\">\n";
  login +=          msg;
  login += "        <div class=\"col-md-12\">\n";
  login += "        </div>\n";
  login += "      </div>\n";
  login += "    </div>\n";
  login += "  </div>\n";
  login += "</div>\n";
  login += "</body>\n";
  login += "</html>\n";

  server.send(200, "text/html", login);
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
  
  mesh_node.begin();
  
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
    if (tempBuffer.length() > 20000) {
      tempBuffer = "";
    }
  }
  server.handleClient();
  mesh_node.acceptRequest();
}
