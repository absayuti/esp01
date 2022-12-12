/*******************************************************************************
  Sketch P02B: ESP8266 ESP-01 Internet real time clock
  
  Using ESP-01 module to connect to the Internet via Wifi + Access Point...
  ... and get time from NNTP
  
  Ref: https://simple-circuit.com/esp8266-esp-01-internet-clock-wifi/
  
.*********.*********.*********.*********.*********.*********.*********.*********/

#include <ESP8266WiFi.h>            // ESP8266 WiFi module library
#include <WiFiUdp.h>                // Libary to use UDP packets
#include <NTPClient.h>              // NTPClient library
#include <TimeLib.h>                // Arduino Time library
#include <Wire.h>
#include <LiquidCrystal_I2C.h>      // LiquidCrystal_I2C library for ESP-01

// Configure LiquidCrystal_I2C library with 0x27 address, 16 columns and 2 rows
LiquidCrystal_I2C lcd(0x27, 16, 2);

// The following macro is to fix a bug in the library i.e. problem when printing 
// to the display if the codes is compiled using current/newer IDE.
// The write function supposed to return 1 when done/OK
#define printIIC(args) Wire.write(args)
inline size_t LiquidCrystal_I2C::write(uint8_t value) {
  send(value, Rs);
  return 1;
}

// Wifi credentials
const char *ssid = "RumahSaya@unifi";
const char *password = "myctsalwa";

WiFiUDP ntpUDP;                     // Create an instance of the WiFi UDP class

// Use 'time.nist.gov' serfver to fetch internet time with +8 hour offset 
// (8 hrs = 28800 seconds) with 60 seconds (60000 ms) update interval
NTPClient timeClient(ntpUDP, "pool.ntp.org", 28800, 60000);

// Variables to hold time data
char Time[] = "TIME:00:00:00"; 
char Date[] = "DATE:00/00/2000";
byte last_second, second_, minute_, hour_, day_, month_;
int year_;


//===============================================================================
void setup() 
{
  Serial.begin(115200);           // Progress can be monitored via Serial Monitor
  delay(200);
  Serial.print("Connecting to ");
  Serial.println(ssid);
  lcd.begin(0,2);                 // Initialize I2C LCD module (SDA = GPIO0, SCL = GPIO2)
  lcd.print("Connecting...");
  
  WiFi.begin(ssid, password);     // Connect to Wifi AP
  do {                            // Wait until connection established
    Serial.print(".");
    delay(1000);
  } while( WiFi.status() != WL_CONNECTED );

  Serial.println("\nConnected!");   // We are connected successfully
  Serial.print("IP address: ");     
  Serial.println(WiFi.localIP());   // What is our IP address?
  Serial.println();
  lcd.clear();
  lcd.print("Connected");
  lcd.setCursor(0,1);
  lcd.print(WiFi.localIP());

  timeClient.begin();
  delay(2000);
}

//===============================================================================
void loop() 
{
  get_time();
  // Send time and date to serial monitor
  Serial.println(Time);
  Serial.println(Date);

  // Display time and date on the 16x2 LCD
  lcd.clear();
  lcd.print(Time);
  lcd.setCursor(0, 1);
  lcd.print(Date);

  delay(1000);
}

//-------------------------------------------------------------------------------
void get_time()
{
  timeClient.update();
  // Get Unix epoch time from the NTP server
  unsigned long unix_epoch = timeClient.getEpochTime();
  second_ = second(unix_epoch);

  if (last_second != second_) {
    minute_ = minute(unix_epoch);
    hour_   = hour(unix_epoch);
    day_    = day(unix_epoch);
    month_  = month(unix_epoch);
    year_   = year(unix_epoch);

    Time[12] = second_ % 10 + 48;
    Time[11] = second_ / 10 + 48;
    Time[9]  = minute_ % 10 + 48;
    Time[8]  = minute_ / 10 + 48;
    Time[6]  = hour_   % 10 + 48;
    Time[5]  = hour_   / 10 + 48;

    Date[5]  = day_   / 10 + 48;
    Date[6]  = day_   % 10 + 48;
    Date[8]  = month_  / 10 + 48;
    Date[9]  = month_  % 10 + 48;
    Date[13] = (year_  / 10) % 10 + 48;
    Date[14] = year_   % 10 % 10 + 48;
  }

  last_second = second_;
}
