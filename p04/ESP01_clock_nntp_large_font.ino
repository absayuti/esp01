/*******************************************************************************
  Sketch P04b: ESP8266 ESP-01 Internet real time clock with large fonts
  
  Using ESP-01 module to connect to the Internet via Wifi + Access Point...
  ... and get time from NNTP
  
  Ref: https://simple-circuit.com/esp8266-esp-01-internet-clock-wifi/

  Code for Large fonts
  https://techzeero.com/arduino-tutorials/custom-characters-on-16x2-lcd/
  
  Dec 2022

.*********.*********.*********.*********.*********.*********.*********.*********/

#include <ESP8266WiFi.h>            // ESP8266 WiFi module library
#include <WiFiUdp.h>                // Libary to use UDP packets
#include <NTPClient.h>              // NTPClient library
#include <TimeLib.h>                // Arduino Time library
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
const char *ssid = "***********";
const char *password = "********";

WiFiUDP ntpUDP;                     // Create an instance of the WiFi UDP class

// Use 'time.nist.gov' serfver to fetch internet time with +8 hour offset 
// (8 hrs = 28800 seconds) with 60 seconds (60000 ms) update interval
NTPClient timeClient(ntpUDP, "pool.ntp.org", 28800, 60000);

// Variables to hold time data
char Time[] = "TIME:00:00:00"; 
char Date[] = "DATE:00/00/2000";
byte last_second, second_, minute_, hour_, day_, month_;
int year_;

// Variables and constants for large fonts
byte glyphs[5][8] = {
  {B11111, B11111, B00000, B00000, B00000, B00000, B00000, B00000},
  {B00000, B00000, B00000, B00000, B00000, B00000, B11111, B11111},
  {B11111, B11111, B00000, B00000, B00000, B00000, B11111, B11111},
  {B11111, B11111, B11111, B11111, B11111, B11111, B11111, B11111},
  {B00000, B00000, B00000, B00000, B00000, B01110, B01110, B01110}
};

const int digitWidth = 3;
const char bigDigitsTop[10][digitWidth] = {3, 0, 3, 0, 3, 32, 2, 2, 3, 0, 2, 3, 3, 1, 3, 3, 2, 2, 3, 2, 2, 0, 0, 3, 3, 2, 3, 3, 2, 3};
const char bigDigitsBot[10][digitWidth] = {3, 1, 3, 1, 3, 1, 3, 1, 1, 1, 1, 3, 32, 32, 3, 1, 1, 3, 3, 1, 3, 32, 32, 3, 3, 1, 3, 1, 1, 3};
char buffer[12];  //used to convert a number into a string

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

  // Create custom glyphs for the large font
  for (int i = 0; i < 5; i++)
    lcd.createChar(i, glyphs[i]);

  lcd.clear();
}

//===============================================================================
void loop() 
{
  timeClient.update();
  // Get Unix epoch time from the NTP server
  unsigned long unix_epoch = timeClient.getEpochTime();
  //setTime(unix_epoch);
  
  second_ = second(unix_epoch);
  if (last_second != second_) {
    convert_time(unix_epoch);
    // Send time and date to serial monitor
    Serial.println(Time);
    Serial.println(Date);

    // Display time and date on the 16x2 LCD
    showDigit(Time[5] - '0', 0);      // Hour
    showDigit(Time[6] - '0', 1);
    showDigit(Time[8] - '0', 2);      // Minute
    showDigit(Time[9] - '0', 3);
    lcd.setCursor(7, 0);              // Colons between HH and MM ie. HH:MM
    lcd.print(":");
    lcd.setCursor(7, 1);
    lcd.print(":");
    lcd.setCursor(15, 0);             // 1st digit of seconds
    lcd.print(Time[11]);
    lcd.setCursor(15, 1);             // 2nd digit of seconds
    lcd.print(Time[12]);
    last_second = second_;
  }
  
  delay(1000);
}

//-------------------------------------------------------------------------------
void convert_time( unsigned long unix_epoch )
{
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

//------------------------------------------------------------------------------
// Function to print large character 3x3
//
void showDigit(int digit, int position)
{
  lcd.setCursor(position * (digitWidth + 1), 0);
  for (int i = 0; i < digitWidth; i++)
    lcd.print(bigDigitsTop[digit][i]);
  lcd.setCursor(position * (digitWidth + 1), 1);
  for (int i = 0; i < digitWidth; i++)
    lcd.print(bigDigitsBot[digit][i]);
}
