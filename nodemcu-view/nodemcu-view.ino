#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266HTTPClient.h>
#include <ArduinoJson.h>
#include "SevSeg.h"

SevSeg sevseg;
const char* wifiName = "ZUKU-c446";
const char* wifiPass = "b1a1003ea3c79";

//Web Server address to read/write from
const char *host = "http://CPU_MONITOR_IP_ADDRESS:3000";
//const char *host = "http://192.168.0.31:3000";
void setup() {

  Serial.begin(115200);
  delay(10);
  Serial.println();

  Serial.print("Connecting to ");
  Serial.println(wifiName);

  WiFi.begin(wifiName, wifiPass);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());   //You can get IP address assigned to ESP

  byte numDigits = 1;
  byte digitPins[] = {};
  byte segmentPins[] = {5, 4, 0, 2, 12, 13, 15, 14}; //Set your own segmentPins pins {A,B,C,D,E,F,G,DP}
  bool resistorsOnSegments = true;

  byte hardwareConfig = COMMON_CATHODE;
  sevseg.begin(hardwareConfig, numDigits, digitPins, segmentPins, resistorsOnSegments);
  sevseg.setBrightness(100);
}

void loop() {
  HTTPClient http;    //Declare object of class HTTPClient

  Serial.print("Request Link:");
  Serial.println(host);

  http.begin(host);     //Specify request destination

  int httpCode = http.GET();            //Send the request
  String payload = http.getString();    //Get the response payload from server

  Serial.print("Response Code:"); //200 is OK
  Serial.println(httpCode);   //Print HTTP return code

  Serial.print("Returned data from Server:");
  Serial.println(payload);    //Print request response payload

  if (httpCode >= 200)
  {
    // Allocate JsonBuffer
    // Use arduinojson.org/assistant to compute the capacity.
    const size_t capacity = JSON_OBJECT_SIZE(2) + 10;
    DynamicJsonDocument doc(capacity);

    // Parse JSON object
    auto error = deserializeJson(doc, payload);
    if (error) {
      Serial.print(F("deserializeJson() failed with code "));
      Serial.println(error.c_str());
      return;
    }

    // Decode JSON/Extract values
    int c = doc["c"];
    int d = doc["d"];
    Serial.println(F("Response:"));
    Serial.println(c);
    Serial.println(d);
    sevseg.setNumber(c);
    delay(2000);
    sevseg.refreshDisplay();
    sevseg.setNumber(d);
    delay(1000);
    sevseg.refreshDisplay();

    
  }
  else
  {
    Serial.println("Error in response");
    sevseg.blank();
  }

  http.end();  //Close connection

  delay(5000);  //GET Data at every 5 seconds
}
