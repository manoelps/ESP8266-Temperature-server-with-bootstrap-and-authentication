#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <WiFiUdp.h>
#include <OneWire.h>
#include <DallasTemperature.h>

#define ONE_WIRE_BUS 4

OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);
DeviceAddress deviceAddress;

unsigned long previousMillis = 0;
const long interval = 10000;
String Temp, javaScript, XML, bootStrap, login, Home, Omdb, Profile, tijd, tempBuffer, uur, Weather, Buffer, Email, datumTijd, theDate;
int uren, minuten, seconden;
float temperatuur;

const char* ssid = "telenet-69233";
const char* password = "ZGnUXZchGPWF";

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
  Home = "<!DOCTYPE html>\n";
  Home += "<html>\n";
  Home += "<head>\n";
  /*Home = "<script>\n";
  Home += "var data_js = {\"access_token\": \"abf4cs9o1lnow0g2itk9fafp\"};\n";
  //Thomas:
  //Home += "var data_js = {\"access_token\": \"abf4cs9o1lnow0g2itk9fafp\"};\n";
  Home += "function sendEmail() {\n";
  Home += "var request = new XMLHttpRequest();\n";
  Home += "var subject = \"Nieuw temperatuuroverzicht!\";\n";
  Home += "var message = \"";
  Home += tempBuffer;
  Home += "\";\n";
  Home += "data_js['subject'] = subject;\n";
  Home += "data_js['text'] = message;\n";
  Home += "var params = toParams(data_js);";
  Home += "request.open(\"POST\", \"https://postmail.invotes.com/send\", true);\n";
  Home += "request.setRequestHeader(\"Content-type\", \"application/x-www-form-urlencoded\");\n";
  Home += "request.send(params);\n";
  Home += "makeInterval();}\n";
  Home += "function toParams(data_js) {\n";
  Home += "var form_data = [];\n";
  Home += "for (var key in data_js) {\n";
  Home += "form_data.push(encodeURIComponent(key) + \"=\" + encodeURIComponent(data_js[key]));}\n";
  Home += "return form_data.join(\"&\");}\n";
  Home += "function makeInterval() {\n";
  Home += "var d = new Date();\n";
  Home += "var min = d.getMinutes();\n";
  Home += "var sec = d.getSeconds();\n";
  Home += "if ((min == '0') && (sec == '0')) {\n";
  Home += "sendEmail();\n";
  Home += "} else {\n";
  Home += "setTimeout(makeInterval, 900);\n";
  Home += "}}\n";
  Home += "makeInterval();\n";
  Home += "</script>\n";*/
  Home += "  <meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\">\n";
  Home += "  <title>Overzicht van de temperaturen</title>\n";
  //Home += "  <link rel=\"icon\" type=\"image/png\" href=\"https://image.freepik.com/free-icon/ssl-encryption_318-32078.png\">";
  Home += "  <link rel=\"stylesheet\" href=\"https://maxcdn.bootstrapcdn.com/bootstrap/3.3.6/css/bootstrap.min.css\" integrity=\"sha384-1q8mTJOASx8j1Au+a5WDVnPi2lkFfwwEAa8hDDdjZlpLegxhjVME1fgjWPGmkzs7\" crossorigin=\"anonymous\">\n";
  Home += "  <script src=\"https://ajax.googleapis.com/ajax/libs/jquery/1.12.2/jquery.min.js\"></script>\n";
  Home += "  <script src=\"http://maxcdn.bootstrapcdn.com/bootstrap/3.3.6/js/bootstrap.min.js\"></script>\n";
  Home += "  <style>.footer {position: absolute; bottom: 0; width: 100%; height: 60px; background-color: #f5f5f5;}</style>";
  Home += "  <script type=\"text/javascript\">\n";
  Home += "    function GetClock(){\n";
  Home += "      var d=new Date();\n";
  Home += "      var nmonth=d.getMonth(),ndate=d.getDate(),nyear=d.getYear();\n";
  Home += "      if(nyear<1000) nyear+=1900;\n";
  Home += "      var nhour=d.getHours(),nmin=d.getMinutes(),nsec=d.getSeconds();\n";
  Home += "      if(nmin<=9) nmin=\"0\"+nmin\n";
  Home += "      if(nsec<=9) nsec=\"0\"+nsec;\n";
  Home += "      document.getElementById('clockbox').innerHTML=\"\"+ndate+\"/\"+(nmonth+1)+\"/\"+nyear+\"     \"+nhour+\":\"+nmin+\":\"+nsec+\"\";\n";
  Home += "    }\n";
  Home += "    window.onload=function(){\n";
  Home += "      GetClock();\n";
  Home += "      setInterval(GetClock,1000);\n";
  Home += "    }\n";
  Home += "</script>\n";
  Home += "</head>\n";
  Home += "<body>\n";
  Home += "  <nav class=\"navbar navbar-inverse navbar-fixed-top\">\n";
  Home += "    <div class=\"container-fluid\">\n";
  Home += "      <div class=\"navbar-header\">\n";
  Home += "        <button type=\"button\" class=\"navbar-toggle\" data-toggle=\"collapse\" data-target=\"#myNavbar\">\n";
  Home += "          <span class=\"icon-bar\"></span>\n";
  Home += "          <span class=\"icon-bar\"></span>\n";
  Home += "          <span class=\"icon-bar\"></span>\n";
  Home += "        </button>\n";
  Home += "        <a class=\"navbar-brand\" href=\"#\"><span class=\"glyphicon glyphicon-cloud\"></span></a>\n";
  Home += "      </div>\n";
  Home += "      <div class=\"collapse navbar-collapse\" id=\"myNavbar\">\n";
  Home += "        <ul class=\"nav navbar-nav\">\n";
  Home += "          <li class=\"active\"><a href=\"/\">Home</a></li>\n";
  Home += "          <li><a href=\"temp\">Temperatuursoverzicht</a></li>\n";
  Home += "          <li><a href=\"omdb\">OMDB</a></li>\n";
  Home += "          <li><a href=\"weather\">weather</a></li>\n";
  Home += "        </ul>\n";
  Home += "        <form class=\"navbar-form navbar-right\" role=\"search\">\n";
  Home += "          <div class=\"form-group input-group\">\n";
  Home += "            <input type=\"text\" class=\"form-control\" placeholder=\"Search...\">\n";
  Home += "            <span class=\"input-group-btn\">\n";
  Home += "              <button class=\"btn btn-default\" type=\"button\">\n";
  Home += "                <span class=\"glyphicon glyphicon-search\"></span>\n";
  Home += "              </button>\n";
  Home += "            </span>\n";
  Home += "          </div>\n";
  Home += "        </form>\n";
  Home += "        <ul class=\"nav navbar-nav navbar-right\">\n";
  Home += "          <li><a href=\"/profile\"><span class=\"glyphicon glyphicon-user\"></span> My Account</a></li>\n";
  Home += "          <li><a href=\"/login?disconnect=yes\">Afmelden</a></li>\n";
  Home += "        </ul>\n";
  Home += "      </div>\n";
  Home += "    </div>\n";
  Home += "  </nav>\n";
  Home += "  <br><br><br>\n";
  Home += "  <div class=\"container text-center\">\n";
  Home += "    <div class=\"jumbotron\">\n";
  Home += "      <h1>HOME ONDER CONSTRUCTIE...</h1>\n";
  Home += "    </div>\n";
  Home += "  </div>\n";
  Home += "  <footer class=\"footer\">\n";
  Home += "    <div class=\"container\"><br>\n";
  Home += "      <p class =\"pull-left\" id=\"clockbox\"></p>\n";
  Home += "    </div>\n";
  Home += "  </footer>\n";
  Home += "</body>\n";
  Home += "</html>\n";
}

void buildprofile() {
  //Profile +="";
  Profile = "<!DOCTYPE html>\n";
  Profile += "<html>\n";
  Profile += "<head>\n";
  /*Profile = "<script>";
  Profile += "var data_js = {\"access_token\": \"5prj7k8j2dp1r1xqjhxrcaz8\"};";
  //Profile += "var data_js = {\"access_token\": \"abf4cs9o1lnow0g2itk9fafp\"};";
  Profile += " function sendEmail() {";
  Profile += "var request = new XMLHttpRequest();";
  Profile += "request.onreadystatechange = function() {";
  Profile += "if (request.readyState == 4 && request.status == 200) {";
  Profile += "js_onSuccess();";
  Profile += "} else if (request.readyState == 4) {";
  Profile += "js_onError(request.response);";
  Profile += "}};";
  Profile += "var subject = \"Nieuw temperatuuroverzicht!\";";
  Profile += "var message = \"";
  Profile += tempBuffer;
  Profile += "\";";
  Profile += "data_js['subject'] = subject;";
  Profile += "data_js['text'] = message;";
  Profile += "var params = toParams(data_js);";
  Profile += "request.open(\"POST\", \"https://postmail.invotes.com/send\", true);";
  Profile += "request.setRequestHeader(\"Content-type\", \"application/x-www-form-urlencoded\");";
  Profile += "request.send(params);";
  Profile += "makeInterval();}";
  Profile += "function toParams(data_js) {";
  Profile += "var form_data = [];";
  Profile += "for (var key in data_js) {";
  Profile += "form_data.push(encodeURIComponent(key) + \"=\" + encodeURIComponent(data_js[key]));}";
  Profile += "return form_data.join(\"&\");}";
  Profile += "function makeInterval() {";
  Profile += "var d = new Date();";
  Profile += "var min = d.getMinutes();";
  Profile += "var sec = d.getSeconds();";
  Profile += "if ((min == '0') && (sec == '0')) {";
  Profile += "sendEmail();";
  Profile += "} else {";
  Profile += "setTimeout(makeInterval, 900);";
  Profile += "}}";
  Profile += "makeInterval();";
  Profile += "</script>";*/
  Profile += "  <meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\">\n";
  Profile += "  <title>Overzicht van de temperaturen</title>\n";
  //Profile += "  <link rel=\"icon\" type=\"image/png\" href=\"https://image.freepik.com/free-icon/ssl-encryption_318-32078.png\">";
  Profile += "  <link rel=\"stylesheet\" href=\"https://maxcdn.bootstrapcdn.com/bootstrap/3.3.6/css/bootstrap.min.css\" integrity=\"sha384-1q8mTJOASx8j1Au+a5WDVnPi2lkFfwwEAa8hDDdjZlpLegxhjVME1fgjWPGmkzs7\" crossorigin=\"anonymous\">\n";
  Profile += "  <script src=\"https://ajax.googleapis.com/ajax/libs/jquery/1.12.2/jquery.min.js\"></script>\n";
  Profile += "  <script src=\"http://maxcdn.bootstrapcdn.com/bootstrap/3.3.6/js/bootstrap.min.js\"></script>\n";
  Profile += "  <style>.footer {position: absolute; bottom: 0; width: 100%; height: 60px; background-color: #f5f5f5;}</style>";
  Profile += "  <script type=\"text/javascript\">\n";
  Profile += "    function GetClock(){\n";
  Profile += "      var d=new Date();\n";
  Profile += "      var nmonth=d.getMonth(),ndate=d.getDate(),nyear=d.getYear();\n";
  Profile += "      if(nyear<1000) nyear+=1900;\n";
  Profile += "      var nhour=d.getHours(),nmin=d.getMinutes(),nsec=d.getSeconds();\n";
  Profile += "      if(nmin<=9) nmin=\"0\"+nmin\n";
  Profile += "      if(nsec<=9) nsec=\"0\"+nsec;\n";
  Profile += "      document.getElementById('clockbox').innerHTML=\"\"+ndate+\"/\"+(nmonth+1)+\"/\"+nyear+\"     \"+nhour+\":\"+nmin+\":\"+nsec+\"\";\n";
  Profile += "    }\n";
  Profile += "    window.onload=function(){\n";
  Profile += "      GetClock();\n";
  Profile += "      setInterval(GetClock,1000);\n";
  Profile += "    }\n";
  Profile += "</script>\n";
  Profile += "</head>\n";
  Profile += "<body>\n";
  Profile += "  <nav class=\"navbar navbar-inverse navbar-fixed-top\">\n";
  Profile += "    <div class=\"container-fluid\">\n";
  Profile += "      <div class=\"navbar-header\">\n";
  Profile += "        <button type=\"button\" class=\"navbar-toggle\" data-toggle=\"collapse\" data-target=\"#myNavbar\">\n";
  Profile += "          <span class=\"icon-bar\"></span>\n";
  Profile += "          <span class=\"icon-bar\"></span>\n";
  Profile += "          <span class=\"icon-bar\"></span>\n";
  Profile += "        </button>\n";
  Profile += "        <a class=\"navbar-brand\" href=\"#\"><span class=\"glyphicon glyphicon-cloud\"></span></a>\n";
  Profile += "      </div>\n";
  Profile += "      <div class=\"collapse navbar-collapse\" id=\"myNavbar\">\n";
  Profile += "        <ul class=\"nav navbar-nav\">\n";
  Profile += "          <li><a href=\"/\">Home</a></li>\n";
  Profile += "          <li><a href=\"temp\">Temperatuursoverzicht</a></li>\n";
  Profile += "          <li><a href=\"omdb\">OMDB</a></li>\n";
  Profile += "          <li><a href=\"weather\">weather</a></li>\n";
  Profile += "        </ul>\n";
  Profile += "        <form class=\"navbar-form navbar-right\" role=\"search\">\n";
  Profile += "          <div class=\"form-group input-group\">\n";
  Profile += "            <input type=\"text\" class=\"form-control\" placeholder=\"Search...\">\n";
  Profile += "            <span class=\"input-group-btn\">\n";
  Profile += "              <button class=\"btn btn-default\" type=\"button\">\n";
  Profile += "                <span class=\"glyphicon glyphicon-search\"></span>\n";
  Profile += "              </button>\n";
  Profile += "            </span>\n";
  Profile += "          </div>\n";
  Profile += "        </form>\n";
  Profile += "        <ul class=\"nav navbar-nav navbar-right\">\n";
  Profile += "          <li class=\"active\"><a href=\"profile\"><span class=\"glyphicon glyphicon-user\"></span> My Account</a></li>\n";
  Profile += "          <li><a href=\"/login?disconnect=yes\">Afmelden</a></li>\n";
  Profile += "        </ul>\n";
  Profile += "      </div>\n";
  Profile += "    </div>\n";
  Profile += "  </nav>\n";
  Profile += "  <br><br><br>\n";
  Profile += "  <div class=\"container text-center\">\n";
  Profile += "    <div class=\"jumbotron\">\n";
  Profile += "      <h1>PROFILE ONDER CONSTRUCTIE...</h1>\n";
  Profile += "    </div>\n";
  Profile += "  </div>\n";
  Profile += "  <footer class=\"footer\">\n";
  Profile += "    <div class=\"container\"><br>\n";
  Profile += "      <p class =\"pull-left\" id=\"clockbox\"></p>\n";
  Profile += "    </div>\n";
  Profile += "  </footer>\n";
  Profile += "</body>\n";
  Profile += "</html>\n";
}

void buildOmdb() {
  //Omdb += "";
  Omdb = "<!DOCTYPE html>\n";
  Omdb += "<html>\n";
  Omdb += "<head>\n";
  Omdb += "  <meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\">\n";
  Omdb += "  <title>Overzicht van de temperaturen</title>\n";
  //Omdb += "  <link rel=\"icon\" type=\"image/png\" href=\"https://image.freepik.com/free-icon/ssl-encryption_318-32078.png\">";
  Omdb += "  <link rel=\"stylesheet\" href=\"https://maxcdn.bootstrapcdn.com/bootstrap/3.3.6/css/bootstrap.min.css\" integrity=\"sha384-1q8mTJOASx8j1Au+a5WDVnPi2lkFfwwEAa8hDDdjZlpLegxhjVME1fgjWPGmkzs7\" crossorigin=\"anonymous\">\n";
  Omdb += "  <script src=\"https://ajax.googleapis.com/ajax/libs/jquery/1.12.2/jquery.min.js\"></script>\n";
  Omdb += "  <script src=\"http://maxcdn.bootstrapcdn.com/bootstrap/3.3.6/js/bootstrap.min.js\"></script>\n";
  Omdb += "  <style>.footer {position: absolute; bottom: 0; width: 100%; height: 60px; background-color: #f5f5f5;}</style>\n";
  Omdb += "  <script>\n";
  Omdb += "    function GetClock(){\n";
  Omdb += "      var d=new Date();\n";
  Omdb += "      var nmonth=d.getMonth(),ndate=d.getDate(),nyear=d.getYear();\n";
  Omdb += "      if(nyear<1000) nyear+=1900;\n";
  Omdb += "      var nhour=d.getHours(),nmin=d.getMinutes(),nsec=d.getSeconds();\n";
  Omdb += "      if(nmin<=9) nmin=\"0\"+nmin\n";
  Omdb += "      if(nsec<=9) nsec=\"0\"+nsec;\n";
  Omdb += "      document.getElementById('clockbox').innerHTML=\"\"+ndate+\"/\"+(nmonth+1)+\"/\"+nyear+\"     \"+nhour+\":\"+nmin+\":\"+nsec+\"\";\n";
  Omdb += "    }\n";
  Omdb += "    window.onload=function(){\n";
  Omdb += "      GetClock();\n";
  Omdb += "      setInterval(GetClock,1000);\n";
  Omdb += "    }\n";
  /*Omdb += "var data_js = {\"access_token\": \"5prj7k8j2dp1r1xqjhxrcaz8\"};\n";
  //Omdb += "var data_js = {\"access_token\": \"abf4cs9o1lnow0g2itk9fafp\"};\n";
  Omdb += " function sendEmail() {\n";
  Omdb += "var request = new XMLHttpRequest();\n";
  Omdb += "request.onreadystatechange = function() {\n";
  Omdb += "if (request.readyState == 4 && request.status == 200) {\n";
  Omdb += "js_onSuccess();\n";
  Omdb += "} else if (request.readyState == 4) {\n";
  Omdb += "js_onError(request.response);\n";
  Omdb += "}};\n";
  Omdb += "var subject = \"Nieuw temperatuuroverzicht!\";\n";
  Omdb += "var message = \"";
  Omdb += tempBuffer;
  Omdb += "\";\n";
  Omdb += "data_js['subject'] = subject;\n";
  Omdb += "data_js['text'] = message;\n";
  Omdb += "var params = toParams(data_js);\n";
  Omdb += "request.open(\"POST\", \"https://postmail.invotes.com/send\", true);\n";
  Omdb += "request.setRequestHeader(\"Content-type\", \"application/x-www-form-urlencoded\");\n";
  Omdb += "request.send(params);\n";
  Omdb += "makeInterval();}\n";
  Omdb += "function toParams(data_js) {\n";
  Omdb += "var form_data = [];\n";
  Omdb += "for (var key in data_js) {\n";
  Omdb += "form_data.push(encodeURIComponent(key) + \"=\" + encodeURIComponent(data_js[key]));}\n";
  Omdb += "return form_data.join(\"&\");}\n";
  Omdb += "function makeInterval() {\n";
  Omdb += "var d = new Date();\n";
  Omdb += "var min = d.getMinutes();\n";
  Omdb += "var sec = d.getSeconds();\n";
  Omdb += "if ((min == '0') && (sec == '0')) {\n";
  Omdb += "sendEmail();\n";
  Omdb += "} else {\n";
  Omdb += "setTimeout(makeInterval, 900);\n";
  Omdb += "}}\n";
  Omdb += "makeInterval();\n";*/
  //test
  Omdb += "function validateForm() {\n";
  Omdb += "var x = document.forms[\"myForm\"][\"title\"].value;\n";
  Omdb += "var xmlhttp = new XMLHttpRequest();\n";
  Omdb += "var url = \"http://www.omdbapi.com/?t=\" + x + \"&y=&plot=long&r=json\";\n";
  Omdb += "xmlhttp.onreadystatechange = function() {\n";
  Omdb += "if (xmlhttp.readyState == 4 && xmlhttp.status == 200) {\n";
  Omdb += "var myArr = JSON.parse(xmlhttp.responseText);\n";
  Omdb += "if (myArr. Response == \"True\"){\n";
  Omdb += "document.getElementById(\"title\").innerHTML = myArr.Title;\n";
  Omdb += "document.getElementById(\"year\").innerHTML = myArr.Year;\n";
  Omdb += "document.getElementById(\"released\").innerHTML = myArr.Released;\n";
  Omdb += "document.getElementById(\"runtime\").innerHTML = myArr.Runtime;\n";
  Omdb += "document.getElementById(\"genre\").innerHTML = myArr.Genre;\n";
  Omdb += "document.getElementById(\"director\").innerHTML = myArr.Director;\n";
  Omdb += "document.getElementById(\"actors\").innerHTML = myArr.Actors;\n";
  Omdb += "document.getElementById(\"plot\").innerHTML = myArr.Plot;\n";
  Omdb += "document.getElementById(\"poster\").innerHTML =\"<img src=\" + myArr.Poster + \"</img>\";\n";
  Omdb += "document.getElementById(\"imdbrating\").innerHTML = myArr.imdbRating;\n";
  Omdb += "}}};\n";
  Omdb += "xmlhttp.open(\"GET\", url, true);\n";
  Omdb += "xmlhttp.send();\n";
  Omdb += "}\n";  
  //test
  Omdb += "</script>\n";
  Omdb += "</head>\n";
  Omdb += "<body>\n";
  Omdb += "  <nav class=\"navbar navbar-inverse navbar-fixed-top\">\n";
  Omdb += "    <div class=\"container-fluid\">\n";
  Omdb += "      <div class=\"navbar-header\">\n";
  Omdb += "        <button type=\"button\" class=\"navbar-toggle\" data-toggle=\"collapse\" data-target=\"#myNavbar\">\n";
  Omdb += "          <span class=\"icon-bar\"></span>\n";
  Omdb += "          <span class=\"icon-bar\"></span>\n";
  Omdb += "          <span class=\"icon-bar\"></span>\n";
  Omdb += "        </button>\n";
  Omdb += "        <a class=\"navbar-brand\" href=\"#\"><span class=\"glyphicon glyphicon-cloud\"></span></a>\n";
  Omdb += "      </div>\n";
  Omdb += "      <div class=\"collapse navbar-collapse\" id=\"myNavbar\">\n";
  Omdb += "        <ul class=\"nav navbar-nav\">\n";
  Omdb += "          <li><a href=\"/\">Home</a></li>\n";
  Omdb += "          <li><a href=\"temp\">Temperatuursoverzicht</a></li>\n";
  Omdb += "          <li class=\"active\"><a href=\"omdb\">OMDB</a></li>\n";
  Omdb += "          <li><a href=\"weather\">weather</a></li>\n";
  Omdb += "        </ul>\n";
  Omdb += "        <form class=\"navbar-form navbar-right\" role=\"search\">\n";
  Omdb += "          <div class=\"form-group input-group\">\n";
  Omdb += "            <input type=\"text\" class=\"form-control\" placeholder=\"Search...\">\n";
  Omdb += "            <span class=\"input-group-btn\">\n";
  Omdb += "              <button class=\"btn btn-default\" type=\"button\">\n";
  Omdb += "                <span class=\"glyphicon glyphicon-search\"></span>\n";
  Omdb += "              </button>\n";
  Omdb += "            </span>\n";
  Omdb += "          </div>\n";
  Omdb += "        </form>\n";
  Omdb += "        <ul class=\"nav navbar-nav navbar-right\">\n";
  Omdb += "          <li><a href=\"profile\"><span class=\"glyphicon glyphicon-user\"></span> My Account</a></li>\n";
  Omdb += "          <li><a href=\"/login?disconnect=yes\">Afmelden</a></li>\n";
  Omdb += "        </ul>\n";
  Omdb += "      </div>\n";
  Omdb += "    </div>\n";
  Omdb += "  </nav>\n";
  Omdb += "  <br><br><br>\n";
  Omdb += "  <div class=\"container text-center\">\n";
  Omdb += "    <div class=\"jumbotron\">\n";
  Omdb += "      <h1>OMDB ONDER CONSTRUCTIE...</h1>\n";
  Omdb += "    </div>\n";
  Omdb += "  </div>\n";
  //test
  Omdb += "   <div id = movieInfo>\n";
  Omdb += "   <p>Title:</p>\n";
  Omdb += "   <div id=\"title\"></div>\n";
  Omdb += "   <p>Released:</p>\n";
  Omdb += "   <div id=\"released\"></div>\n";
  Omdb += "   <p>Duration:</p>\n";
  Omdb += "   <div id=\"runtime\"></div>\n";
  Omdb += "   <p>Genre:</p>\n";
  Omdb += "   <div id=\"genre\"></div>\n";
  Omdb += "   <p>Director:</p>\n";
  Omdb += "   <div id=\"director\"></div>\n";
  Omdb += "   <p>Actors</p>\n";
  Omdb += "   <div id=\"actors\"></div>\n";
  Omdb += "   <p>Plot</p>\n";
  Omdb += "   <div id=\"plot\"></div>\n";
  Omdb += "   <p>Poster:</p>\n";
  Omdb += "   <div id=\"poster\"></div>\n";
  Omdb += "   <p>Rating on IMDB</p>\n";
  Omdb += "   <div id=\"imdbrating\"></div>\n";
  Omdb += "   <div id =\"form\">\n";
  Omdb += "   <form name=\"myForm\" onsubmit=\"validateForm()\" method=\"post\">\n";
  Omdb += "   Title: <input type=\"text\" name=\"title\">\n";
  Omdb += "   <input type=\"submit\" value=\"Submit\">\n";
  Omdb += "   </form>\n";
  Omdb += "   </div>\n";
  Omdb += "   </div>";
  //test
  Omdb += "  <footer class=\"footer\">\n";
  Omdb += "    <div class=\"container\"><br>\n";
  Omdb += "      <p class =\"pull-left\" id=\"clockbox\"></p>\n";
  Omdb += "    </div>\n";
  Omdb += "  </footer>\n";
  Omdb += "</body>\n";
  Omdb += "</html>\n";
}

void buildWeather() {
  //Weather += "";
  Weather = "<!DOCTYPE html>\n";
  Weather += "<html>\n";
  Weather += "<head>\n";
  /*Weather = "<script>";
  Weather += "var data_js = {\"access_token\": \"5prj7k8j2dp1r1xqjhxrcaz8\"};";
  //Weather += "var data_js = {\"access_token\": \"abf4cs9o1lnow0g2itk9fafp\"};";
  Weather += " function sendEmail() {";
  Weather += "var request = new XMLHttpRequest();";
  Weather += "request.onreadystatechange = function() {";
  Weather += "if (request.readyState == 4 && request.status == 200) {";
  Weather += "js_onSuccess();";
  Weather += "} else if (request.readyState == 4) {";
  Weather += "js_onError(request.response);";
  Weather += "}};";
  Weather += "var subject = \"Nieuw temperatuuroverzicht!\";";
  Weather += "var message = \"Hello\";";
  Weather += "data_js['subject'] = subject;";
  Weather += "var message = \"";
  Weather += tempBuffer;
  Weather += "\";";
  Weather += "var params = toParams(data_js);";
  Weather += "request.open(\"POST\", \"https://postmail.invotes.com/send\", true);";
  Weather += "request.setRequestHeader(\"Content-type\", \"application/x-www-form-urlencoded\");";
  Weather += "request.send(params);";
  Weather += "makeInterval();}";
  Weather += "function toParams(data_js) {";
  Weather += "var form_data = [];";
  Weather += "for (var key in data_js) {";
  Weather += "form_data.push(encodeURIComponent(key) + \"=\" + encodeURIComponent(data_js[key]));}";
  Weather += "return form_data.join(\"&\");}";
  Weather += "function makeInterval() {";
  Weather += "var d = new Date();";
  Weather += "var min = d.getMinutes();";
  Weather += "var sec = d.getSeconds();";
  Weather += "if ((min == '0') && (sec == '0')) {";
  Weather += "sendEmail();";
  Weather += "} else {";
  Weather += "setTimeout(makeInterval, 900);";
  Weather += "}}";
  Weather += "makeInterval();";
  Weather += "</script>";*/
  Weather += "  <meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\">\n";
  Weather += "  <title>Overzicht van de temperaturen</title>\n";
  //Weather += "  <link rel=\"icon\" type=\"image/png\" href=\"https://image.freepik.com/free-icon/ssl-encryption_318-32078.png\">";
  Weather += "  <link rel=\"stylesheet\" href=\"https://maxcdn.bootstrapcdn.com/bootstrap/3.3.6/css/bootstrap.min.css\" integrity=\"sha384-1q8mTJOASx8j1Au+a5WDVnPi2lkFfwwEAa8hDDdjZlpLegxhjVME1fgjWPGmkzs7\" crossorigin=\"anonymous\">\n";
  Weather += "  <script src=\"https://ajax.googleapis.com/ajax/libs/jquery/1.12.2/jquery.min.js\"></script>\n";
  Weather += "  <script src=\"http://maxcdn.bootstrapcdn.com/bootstrap/3.3.6/js/bootstrap.min.js\"></script>\n";
  Weather += "  <style>.footer {position: absolute; bottom: 0; width: 100%; height: 60px; background-color: #f5f5f5;}</style>";
  Weather += "  <script type=\"text/javascript\">\n";
  Weather += "    function GetClock(){\n";
  Weather += "      var d=new Date();\n";
  Weather += "      var nmonth=d.getMonth(),ndate=d.getDate(),nyear=d.getYear();\n";
  Weather += "      if(nyear<1000) nyear+=1900;\n";
  Weather += "      var nhour=d.getHours(),nmin=d.getMinutes(),nsec=d.getSeconds();\n";
  Weather += "      if(nmin<=9) nmin=\"0\"+nmin\n";
  Weather += "      if(nsec<=9) nsec=\"0\"+nsec;\n";
  Weather += "      document.getElementById('clockbox').innerHTML=\"\"+ndate+\"/\"+(nmonth+1)+\"/\"+nyear+\"     \"+nhour+\":\"+nmin+\":\"+nsec+\"\";\n";
  Weather += "    }\n";
  Weather += "    window.onload=function(){\n";
  Weather += "      GetClock();\n";
  Weather += "      setInterval(GetClock,1000);\n";
  Weather += "    }\n";
  Weather += "</script>\n";
  Weather += "</head>\n";
  Weather += "<body>\n";
  Weather += "  <nav class=\"navbar navbar-inverse navbar-fixed-top\">\n";
  Weather += "    <div class=\"container-fluid\">\n";
  Weather += "      <div class=\"navbar-header\">\n";
  Weather += "        <button type=\"button\" class=\"navbar-toggle\" data-toggle=\"collapse\" data-target=\"#myNavbar\">\n";
  Weather += "          <span class=\"icon-bar\"></span>\n";
  Weather += "          <span class=\"icon-bar\"></span>\n";
  Weather += "          <span class=\"icon-bar\"></span>\n";
  Weather += "        </button>\n";
  Weather += "        <a class=\"navbar-brand\" href=\"#\"><span class=\"glyphicon glyphicon-cloud\"></span></a>\n";
  Weather += "      </div>\n";
  Weather += "      <div class=\"collapse navbar-collapse\" id=\"myNavbar\">\n";
  Weather += "        <ul class=\"nav navbar-nav\">\n";
  Weather += "          <li><a href=\"/\">Home</a></li>\n";
  Weather += "          <li><a href=\"temp\">Temperatuursoverzicht</a></li>\n";
  Weather += "          <li><a href=\"omdb\">OMDB</a></li>\n";
  Weather += "          <li class=\"active\"><a href=\"weather\">weather</a></li>\n";
  Weather += "        </ul>\n";
  Weather += "        <form class=\"navbar-form navbar-right\" role=\"search\">\n";
  Weather += "          <div class=\"form-group input-group\">\n";
  Weather += "            <input type=\"text\" class=\"form-control\" placeholder=\"Search...\">\n";
  Weather += "            <span class=\"input-group-btn\">\n";
  Weather += "              <button class=\"btn btn-default\" type=\"button\">\n";
  Weather += "                <span class=\"glyphicon glyphicon-search\"></span>\n";
  Weather += "              </button>\n";
  Weather += "            </span>\n";
  Weather += "          </div>\n";
  Weather += "        </form>\n";
  Weather += "        <ul class=\"nav navbar-nav navbar-right\">\n";
  Weather += "          <li><a href=\"profile\"><span class=\"glyphicon glyphicon-user\"></span> My Account</a></li>\n";
  Weather += "          <li><a href=\"/login?disconnect=yes\">Afmelden</a></li>\n";
  Weather += "        </ul>\n";
  Weather += "      </div>\n";
  Weather += "    </div>\n";
  Weather += "  </nav>\n";
  Weather += "  <br><br><br>\n";
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
  Weather += "  <footer class=\"footer\">\n";
  Weather += "    <div class=\"container\"><br>\n";
  Weather += "      <p class =\"pull-left\" id=\"clockbox\"></p>\n";
  Weather += "    </div>\n";
  Weather += "  </footer>\n";
  Weather += "</body>\n";
  Weather += "</html>\n";
}

void buildTemp() {
  //Temp += "";
  buildJavascript();
  Temp = "<!DOCTYPE html>\n";
  Temp += "<html>\n";
  Temp += "<head>\n";
  /*Temp = "<script>";
  Temp += "var data_js = {\"access_token\": \"5prj7k8j2dp1r1xqjhxrcaz8\"};";
  //Temp += "var data_js = {\"access_token\": \"abf4cs9o1lnow0g2itk9fafp\"};";
  Temp += " function sendEmail() {";
  Temp += "var request = new XMLHttpRequest();";
  Temp += "request.onreadystatechange = function() {";
  Temp += "if (request.readyState == 4 && request.status == 200) {";
  Temp += "js_onSuccess();";
  Temp += "} else if (request.readyState == 4) {";
  Temp += "js_onError(request.response);";
  Temp += "}};";
  Temp += "var subject = \"Nieuw temperatuuroverzicht!\";";
  Temp += "var message = \"";
  Temp += tempBuffer;
  Temp += "\";";
  Temp += "data_js['subject'] = subject;";
  Temp += "data_js['text'] = message;";
  Temp += "var params = toParams(data_js);";
  Temp += "request.open(\"POST\", \"https://postmail.invotes.com/send\", true);";
  Temp += "request.setRequestHeader(\"Content-type\", \"application/x-www-form-urlencoded\");";
  Temp += "request.send(params);";
  Temp += "makeInterval();}";
  Temp += "function toParams(data_js) {";
  Temp += "var form_data = [];";
  Temp += "for (var key in data_js) {";
  Temp += "form_data.push(encodeURIComponent(key) + \"=\" + encodeURIComponent(data_js[key]));}";
  Temp += "return form_data.join(\"&\");}";
  Temp += "function makeInterval() {";
  Temp += "var d = new Date();";
  Temp += "var min = d.getMinutes();";
  Temp += "var sec = d.getSeconds();";
  Temp += "if ((min == '0') && (sec == '0')) {";
  Temp += "sendEmail();";
  Temp += "} else {";
  Temp += "setTimeout(makeInterval, 900);";
  Temp += "}}";
  Temp += "makeInterval();";
  Temp += "</script>";*/
  Temp += "  <meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\">\n";
  Temp += "  <title>Overzicht van de temperaturen</title>\n";
  //Temp += "  <link rel=\"icon\" type=\"image/png\" href=\"https://image.freepik.com/free-icon/ssl-encryption_318-32078.png\">";
  Temp += "  <link rel=\"stylesheet\" href=\"https://maxcdn.bootstrapcdn.com/bootstrap/3.3.6/css/bootstrap.min.css\" integrity=\"sha384-1q8mTJOASx8j1Au+a5WDVnPi2lkFfwwEAa8hDDdjZlpLegxhjVME1fgjWPGmkzs7\" crossorigin=\"anonymous\">\n";
  Temp += "  <style>.footer {position: absolute; bottom: 0; width: 100%; height: 60px; background-color: #f5f5f5;} .well{ background-color: #99d6ff;}</style>";
  Temp += "  <script src=\"https://ajax.googleapis.com/ajax/libs/jquery/1.12.2/jquery.min.js\"></script>\n";
  Temp += "  <script src=\"http://maxcdn.bootstrapcdn.com/bootstrap/3.3.6/js/bootstrap.min.js\"></script>\n";
  Temp += "  <script type=\"text/javascript\">\n";
  Temp += "    function GetClock(){\n";
  Temp += "      var d=new Date();\n";
  Temp += "      var nmonth=d.getMonth(),ndate=d.getDate(),nyear=d.getYear();\n";
  Temp += "      if(nyear<1000) nyear+=1900;\n";
  Temp += "      var nhour=d.getHours(),nmin=d.getMinutes(),nsec=d.getSeconds();\n";
  Temp += "      if(nmin<=9) nmin=\"0\"+nmin\n";
  Temp += "      if(nsec<=9) nsec=\"0\"+nsec;\n";
  Temp += "      document.getElementById('clockbox').innerHTML=\"\"+ndate+\"/\"+(nmonth+1)+\"/\"+nyear+\"     \"+nhour+\":\"+nmin+\":\"+nsec+\"\";\n";
  Temp += "    }\n";
  Temp += "    window.onload=function(){\n";
  Temp += "      GetClock();\n";
  Temp += "      setInterval(GetClock,1000);\n";
  Temp += "    }\n";
  Temp += "</script>\n";
  Temp += "</head>\n";
  Temp += javaScript;
  Temp += "<body onload='process()'>\n";
  Temp += "  <nav class=\"navbar navbar-inverse navbar-fixed-top\">\n";
  Temp += "    <div class=\"container-fluid\">\n";
  Temp += "      <div class=\"navbar-header\">\n";
  Temp += "        <button type=\"button\" class=\"navbar-toggle\" data-toggle=\"collapse\" data-target=\"#myNavbar\">\n";
  Temp += "          <span class=\"icon-bar\"></span>\n";
  Temp += "          <span class=\"icon-bar\"></span>\n";
  Temp += "          <span class=\"icon-bar\"></span>\n";
  Temp += "        </button>\n";
  Temp += "        <a class=\"navbar-brand\" href=\"#\"><span class=\"glyphicon glyphicon-cloud\"></span></a>\n";
  Temp += "      </div>\n";
  Temp += "      <div class=\"collapse navbar-collapse\" id=\"myNavbar\">\n";
  Temp += "        <ul class=\"nav navbar-nav\">\n";
  Temp += "          <li><a href=\"/\">Home</a></li>\n";
  Temp += "          <li class=\"active\"><a href=\"/temp\">Temperatuursoverzicht</a></li>\n";
  Temp += "          <li><a href=\"omdb\">OMDB</a></li>\n";
  Temp += "          <li><a href=\"weather\">weather</a></li>\n";
  Temp += "        </ul>\n";
  Temp += "        <form class=\"navbar-form navbar-right\" role=\"search\">\n";
  Temp += "          <div class=\"form-group input-group\">\n";
  Temp += "            <input type=\"text\" class=\"form-control\" placeholder=\"Search...\">\n";
  Temp += "            <span class=\"input-group-btn\">\n";
  Temp += "              <button class=\"btn btn-default\" type=\"button\">\n";
  Temp += "                <span class=\"glyphicon glyphicon-search\"></span>\n";
  Temp += "              </button>\n";
  Temp += "            </span>\n";
  Temp += "          </div>\n";
  Temp += "        </form>\n";
  Temp += "        <ul class=\"nav navbar-nav navbar-right\">\n";
  Temp += "          <li><a href=\"profile\"><span class=\"glyphicon glyphicon-user\"></span> My Account</a></li>\n";
  Temp += "          <li><a href=\"/login?disconnect=yes\">Afmelden</a></li>\n";
  Temp += "        </ul>\n";
  Temp += "      </div>\n";
  Temp += "    </div>\n";
  Temp += "  </nav>\n";
  Temp += "  <br><br><br>\n";
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
  Temp += "  <footer class=\"footer\">\n";
  Temp += "    <div class=\"container\"><br>\n";
  Temp += "      <p class =\"pull-left\" id=\"clockbox\"></p>\n";
  Temp += "    </div>\n";
  Temp += "  </footer>\n";
  Temp += "</body>\n";
  Temp += "</html>\n";
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
  if (!is_authentified()) {
    String header = "HTTP/1.1 301 OK\r\nLocation: /login\r\nCache-Control: no-cache\r\n\r\n";
    server.sendContent(header);
    return;
  }
  buildprofile();
  server.send(200, "text/html", Profile);
}

void handleTemp() {
  if (!is_authentified()) {
    String header = "HTTP/1.1 301 OK\r\nLocation: /login\r\nCache-Control: no-cache\r\n\r\n";
    server.sendContent(header);
    return;
  }
  buildTemp();
  server.send(200, "text/html", Temp);
}

void handleWeather() {
  if (!is_authentified()) {
    String header = "HTTP/1.1 301 OK\r\nLocation: /login\r\nCache-Control: no-cache\r\n\r\n";
    server.sendContent(header);
    return;
  }
  buildWeather();
  server.send(200, "text/html", Weather);
}

void handleXML() {
  if (!is_authentified()) {
    String header = "HTTP/1.1 301 OK\r\nLocation: /login\r\nCache-Control: no-cache\r\n\r\n";
    server.sendContent(header);
    return;
  }
  buildXML();
  server.send(200, "text/xml", XML);
}

void handleHome() {
  if (!is_authentified()) {
    String header = "HTTP/1.1 301 OK\r\nLocation: /login\r\nCache-Control: no-cache\r\n\r\n";
    server.sendContent(header);
    return;
  }
  buildHome();
  server.send(200, "text/html", Home);
}

void handleOmdb() {
  if (!is_authentified()) {
    String header = "HTTP/1.1 301 OK\r\nLocation: /login\r\nCache-Control: no-cache\r\n\r\n";
    server.sendContent(header);
    return;
  }
  buildOmdb();
  server.send(200, "text/html", Omdb);
}

void handleBuffer() {
  if (!is_authentified()) {
    String header = "HTTP/1.1 301 OK\r\nLocation: /login\r\nCache-Control: no-cache\r\n\r\n";
    server.sendContent(header);
    return;
  }
  buildbuffer();
  server.send(200, "text/html", Buffer);
}

void handleNotFound() {
  String message = "404 File Not Found\n\n";
  server.send(404, "text/plain", message);
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
  digitalWrite(5, HIGH);
  delay(500);
  digitalWrite(5, LOW);
  Serial.println(WiFi.localIP());

  //server.on("/", handle);
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

void loop(void) {
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
}
