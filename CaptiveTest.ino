//SRC and instructions from https://yoursunny.com/t/2017/ESP8266-captive-login/
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include "ChangeMac.hpp"

const char* ssid     = "UniversityArms-WiFi";
const char* password = "<the-password>";

bool captiveLogin() {
  static const char* LOCATION = "Location";
  static const char* SET_COOKIE = "Set-Cookie";
  static const char* HEADER_NAMES[] = {LOCATION, SET_COOKIE};

  String uri;
  {
    HTTPClient http;
    http.begin("http://captive.apple.com/");
    http.collectHeaders(HEADER_NAMES, 2);
    int httpCode = http.GET();
    if (httpCode == 200) {
      return true;
    }
    if (httpCode != 302 || !http.hasHeader(LOCATION)) {
      return false;
    }
    uri = http.header(LOCATION);
    Serial.print("portal=");
    Serial.println(uri);
    delay(2000);
  }

  String cookie;
  {
    HTTPClient http;
    http.begin(uri);
    http.collectHeaders(HEADER_NAMES, 2);
    int httpCode = http.GET();
    if (httpCode != 200 || !http.hasHeader(SET_COOKIE)) {
      return false;
    }
    cookie = http.header(SET_COOKIE);
    Serial.print("cookie=");
    Serial.println(cookie);
    delay(3000);
  }

  {
    int pos = uri.lastIndexOf("/?");
    if (pos < 0) {
      return false;
    }
    HTTPClient http;
    http.begin(uri.substring(0, pos) + "/login");
    http.addHeader("Content-Type", "application/x-www-form-urlencoded");
    http.addHeader("Cookie", cookie);
    http.collectHeaders(HEADER_NAMES, 2);
    int httpCode = http.POST("connect=Connect");
    if (httpCode != 302 || !http.hasHeader(LOCATION)) {
      return false;
    }
    uri = http.header(LOCATION);
    cookie = http.header(SET_COOKIE);
    Serial.print("redirect=");
    Serial.println(uri);
    delay(500);
  }

  {
    HTTPClient http;
    http.begin(uri);
    int httpCode = http.GET();
    if (httpCode != 302) {
      return false;
    }
    delay(500);
  }

  {
    HTTPClient http;
    http.begin("http://captive.apple.com/");
    int httpCode = http.GET();
    if (httpCode == 200) {
      return true;
    }
  }

  {
    HTTPClient http;
    http.begin("http://portquiz.net:8080/");
    int httpCode = http.GET();
    return httpCode == 200;
  }
}

void setup() {
  Serial.begin(115200);
  Serial.println();
  Serial.println();

  WiFi.mode(WIFI_STA);
  WiFi.persistent(false);

  uint8_t mac[6];
  makeRandomMac(mac);
  changeMac(mac);
  Serial.print("MAC address is ");
  Serial.println(WiFi.macAddress());

  String hostname = "iPad-";
  hostname += random(10);
  hostname += random(10);
  hostname += random(10);
  hostname += random(10);
  WiFi.hostname(hostname);
  Serial.print("Hostname is ");
  Serial.println(hostname);

  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(WiFi.status());
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());

  if (!captiveLogin()) {
    ESP.restart();
  }
}

void loop() {
}
