// #include WiFi Library
#include <ESP8266WiFi.h>
#include <ESP8266Ping.h>

/* Serial Baud Rate */
#define SERIAL_BAUD       9600
/* Delay paramter for connection. */
#define WIFI_DELAY        500
/* Max SSID octets. */
#define MAX_SSID_LEN      32
/* Wait this much until device gets IP. */
#define MAX_CONNECT_TIME  30000

/* SSID that to be stored to connect. */
char ssid[MAX_SSID_LEN] = "COS_PUB";
const char* remote_host = "www.google.com";

/* Scan available networks and sort them in order to their signal strength. */
void scanAndSort() {
  memset(ssid, 0, MAX_SSID_LEN);
  int n = WiFi.scanNetworks();

  // Array of network indices used to get the name from WiFi object (strongest signal)
  int networkIndices[n];
  for (int i = 0; i < n; i++) {
      networkIndices[i] = i;
  }
  
  Serial.println("Scan complete!");
  if (n == 0) {
    Serial.println("No networks available.");
  } else {
    Serial.print(n);
    Serial.println(" networks discovered.");

    // Order the indices by strongest signal
    for (int i = 0; i < n; i++) {
      for (int j = i + 1; j < n; j++) {
        if (WiFi.RSSI(networkIndices[j]) > WiFi.RSSI(networkIndices[i])) {
          std::swap(networkIndices[i], networkIndices[j]);
        }
      }
    }
    
    for (int i = 0; i < n; ++i) {
      Serial.println("The strongest open network is:");
      Serial.print(WiFi.SSID(networkIndices[i]));
      Serial.print(" ");
      Serial.print(WiFi.RSSI(networkIndices[i]));
      Serial.print(" ");
      Serial.print(WiFi.encryptionType(networkIndices[i]));
      Serial.println();
      if (WiFi.encryptionType(networkIndices[i]) == ENC_TYPE_NONE) {
        memset(ssid, 0, MAX_SSID_LEN);
        strncpy(ssid, WiFi.SSID(networkIndices[i]).c_str(), MAX_SSID_LEN);
        break;
      }
    }
  }
}

void setup() {
  // Scan, Sort, and Connect to WiFi
  Serial.begin(SERIAL_BAUD);
  Serial.println("Scanning for open networks...");
  if (WiFi.status() != WL_CONNECTED) {
    /* Clear previous modes. */
    WiFi.softAPdisconnect();
    WiFi.disconnect();
    WiFi.mode(WIFI_STA);
    delay(WIFI_DELAY);
    /* Scan for networks to find open guy. */
    // scanAndSort();
    // delay(WIFI_DELAY);
    /* Global ssid param need to be filled to connect. */
    if (strlen(ssid) > 0) {
      Serial.print("Connecting to ");
      Serial.println(ssid);
      /* No password for WiFi. We are looking for non-encrypted connections. */
      WiFi.begin(ssid);
      unsigned short try_count = 0;
      /* Wait until WiFi connection but do not exceed MAX_CONNECT_TIME */
      while (WiFi.status() != WL_CONNECTED && try_count < MAX_CONNECT_TIME / WIFI_DELAY) {
        delay(WIFI_DELAY);
        Serial.print(".");
        try_count++;
      }
      if (WiFi.status() == WL_CONNECTED) {
        Serial.println("");
        Serial.println("Connection Successful!");
        Serial.println("Your device IP address is ");
        Serial.println(WiFi.localIP());
        Serial.print("Pinging host ");
        Serial.println(remote_host);

        if (Ping.ping(remote_host)) {
          Serial.println("Success!!");
        } else {
          Serial.println("Error :(");
        }

      } else {
        Serial.println("Connection FAILED");
      }
    } else {
      Serial.println("No open networks available. :-(");
    }
  }
}

void loop ()  {
}
