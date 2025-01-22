#include <Arduino.h>
#include <WiFi.h>
#include <WiFiClient.h>
#include <WebServer.h>
#include <Dezibot.h>

Dezibot dezibot = Dezibot();

// Set webserver access
const char* SSID = "Debug-Server";
const char* PSK = "PW4studProj";

WebServer server(80);

IPAddress local_ip(192,168,1,1);
IPAddress gateway(192,168,1,1);
IPAddress subnet(255,255,255,0);

//index.h
const char MAIN_page[] PROGMEM = R"=====(
<!DOCTYPE html>
<html>
<style>
.card{
    max-width: 400px;
     min-height: 250px;
     background: #02b875;
     padding: 30px;
     box-sizing: border-box;
     color: #FFF;
     margin:20px;
     box-shadow: 0px 2px 18px -4px rgba(0,0,0,0.75);
}
</style>
<body>

<div class="card">
  <h4>The ESP32 Update web page without refresh</h4><br>
  <h1>Sensor Value:<span id="ADCValue">0</span></h1><br>
  <h1>My Number:<span id="NumberValue">0</span></h1><br>
</div>
<script>
function getData() {
  var xhttp = new XMLHttpRequest();
  xhttp.onreadystatechange = function() {
    if (xhttp.readyState == XMLHttpRequest.DONE && xhttp.status == 200) {
      document.getElementById("ADCValue").innerHTML = xhttp.responseText;
    }
  };
  xhttp.open("GET", "readADC", true);
  xhttp.send();
}

  var mynumber = 0;
  setInterval(function() {
    mynumber++;
    document.getElementById("NumberValue").innerHTML = mynumber;
    getData();
  }, 2000);
</script>
</body>
</html>
)=====";


void handleRoot() {
    String s = MAIN_page;
    server.send(200, "text/html", s);
}
int a = 10;
void handleADC() {
    a++;
    String adcValue = String(a);
    Serial.println(adcValue);
    server.send(200, "text/plane", adcValue);
}

void setup() {

    Serial.begin(115200);
    WiFi.softAP(SSID, PSK);
    WiFi.softAPConfig(local_ip, gateway, subnet);

    server.on("/", handleRoot);
    server.on("/readADC", handleADC);
    server.begin();
    Serial.println("Setup complete!");
}

void loop() {
    server.handleClient();
    delay(1);
}

