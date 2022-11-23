##ESP-01, Wi-Fi dan Internet

Perkakasan dan Perisian yg diperlukan
Perisian Arduino IDE
Modul Wi-Fi ESP-01
Papan Pengaturcara (programmer board) ESP-01
atau USB-ESP01 Adapter board yang telah diubahsuai 
Wi-Fi Router / Access Point
SSID dan password (untuk login ke Wi-Fi)

Sketch P2: IP address
/*******************************************************************************
  Sketch P02 

  Using ESP-01 to connects to the Internet via Wifi + Access Point
  
/********************************************************************/

#include <ESP8266WiFi.h>

// Wifi credentials
const char *ssid = "xxxxxxxxx";
const char *password = "yyyyyyy";

void setup() 
{
  Serial.begin(115200);           // Progress can be monitored via Serial Monitor
  delay(100);
  Serial.println();

  WiFi.begin(ssid, password);     // Connect to Wifi AP
  Serial.print("Connecting to ");
  Serial.print(ssid);
  
  do {                            // Wait until connection established
    Serial.print("..");
    delay(3000);
  } while( WiFi.status() != WL_CONNECTED );

  Serial.println("\nConnected!");   // We are connected successfully
  Serial.print("IP address: ");     
  Serial.println(WiFi.localIP());   // What is our IP address?
  Serial.println();
}

void loop() 
{
  Serial.print("WiFi AP Signal strength = ");
  long rssi = WiFi.RSSI();
  Serial.println(rssi);
  delay(1000);
}

