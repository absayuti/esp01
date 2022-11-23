/*******************************************************************************
  Sketch P02 

  Using ESP-01 to connects to the Internet via Wifi + Access Point
  
/******************************************************************************/

#include <ESP8266WiFi.h>

// Wifi credentials
const char *ssid = "xxxxxxxxxxx";
const char *password = "yyyyyyyyyy";

void setup() 
{
  pinMode(LED_BUILTIN, OUTPUT);   // Initialize the LED_BUILTIN pin as an output
  Serial.begin(115200);           // Progress can be monitored via Serial Monitor
  delay(100);
  Serial.println();

  Serial.print("Connecting to ");
  Serial.print(ssid);
  
  WiFi.begin(ssid, password);     // Connect to Wifi AP
  do {                            // Wait until connection established
    Serial.print("..");
    digitalWrite(LED_BUILTIN, LOW);   // LOW = ON
    delay(2000);
    digitalWrite(LED_BUILTIN, HIGH);  // HIGH = OFF
    delay(2000);
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
