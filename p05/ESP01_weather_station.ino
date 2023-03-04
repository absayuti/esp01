/*******************************************************************************
  Sketch P05.1: ESP8266 ESP-01 Weather Station Using OpenWeatherMap.org
  
  Using ESP-01 module to display WEATHER data obtained from OpenWeatherMap.org 
  icluding TIME and DATE from NNTP server. 
  
  References: 
  
  1)  Circuit: 
      https://simple-circuit.com/esp8266-esp-01-internet-clock-wifi/

  2)  Base code for Large fonts:
      https://techzeero.com/arduino-tutorials/custom-characters-on-16x2-lcd/
  
  3)  The basics: 
      https://create.arduino.cc/projecthub/akshayjoseph666/interface-i2c-16x2-lcd-with-arduino-uno-just-4-wires-273b24
      Theories, sketches, link to library

  4)  LiquidCrystal_I2C Library:
      https://github.com/agnunez/ESP8266-I2C-LCD1602

      Fix bug in the library in this function:
        inline size_t LiquidCrystal_I2C::write(uint8_t value) {
          send(value, Rs);
          return 1;       // <---- ADD THIS!
        }


.*********.*********.*********.*********.*********.*********.*********.*********/

#include <ESP8266WiFi.h>            // ESP8266 WiFi module library
#include <WiFiUdp.h>                // Libary to use UDP packets
#include <NTPClient.h>              // NTPClient library
#include <Arduino_JSON.h>           // Arduino_JSON library by Arduino
#include <WiFiClient.h>
#include <ESP8266HTTPClient.h>

#include <TimeLib.h>                // Arduino Time library
#include <LiquidCrystal_I2C.h>      // LiquidCrystal I2C library 

// The following macro is to fix a bug in the library i.e. problem when printing 
// to the display if the codes is compiled using current/newer IDE.
// The write function supposed to return 1 when done/OK
//#define printIIC(args) Wire.write(args)
//inline size_t LiquidCrystal_I2C::write(uint8_t value) {
//  send(value, Rs);
//  return 1;
//}

// Configure LiquidCrystal_I2C library with 0x27 address, 16 columns and 2 rows
LiquidCrystal_I2C lcd(0x27, 16, 2);

// Wifi credentials
const char *ssid = "??????????";
const char *password = "??????????";

String openWeatherMapApiKey = "d5f092a1e6acb4ecd3c6da55271a573f";
String city = "Simpang Pulai";
String countryCode = "MY";
String jsonBuffer;
String weather;
int  temperature;
int  humidity;

WiFiUDP ntpUDP;                     // Create an instance of the WiFi UDP class

// Use 'time.nist.gov' serfver to fetch internet time with +8 hour offset 
// (8 hrs = 28800 seconds) with 60 seconds (60000 ms) update interval
NTPClient timeClient(ntpUDP, "pool.ntp.org", 28800, 60000);

// Variables to hold time data
char Time[] = "00:00:00"; 
char Date[] = "00/00/00";
byte last_second, second_, minute_, hour_, day_, month_;
int year_;
char Month[12][4] = {"Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"};

// Variables and constants for large fonts
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
//char buffer[12];  //used to convert a number into a string


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
  for (int i = 0; i < 8; i++)
    lcd.createChar(i, glyphs[i]);

  lcd.clear();
}

//===============================================================================
// The main loop
//
int loopcount = 0;

void loop() 
{
  timeClient.update();
  // Get Unix epoch time from the NTP server
  unsigned long unix_epoch = timeClient.getEpochTime();
  setTime(unix_epoch);
  
  second_ = second(unix_epoch);
  if (last_second != second_) {
    convert_time(unix_epoch);
    // Send time and date to serial monitor
    Serial.println(Time);
    Serial.println(Date);

    last_second = second_;
  }
  
  if( loopcount==0 ) {								// Fetch weather data at start and every 30 minutes 
		getWeather();
	}
	loopcount++;
	if( loopcount==1800 ) loopcount = 0;		// Reset counter every 30 minutes, if delay is 1 second
  
  // Switch output between TIME and WEATHER
	int remain = loopcount%10;
	switch( remain ) {
  	case 0: 
      			lcd.clear();
    				printTime();
  					break;
  	case 5: 
            lcd.clear();
  					printWeather();
  					break;
    case 6:
    case 7:
    case 8:
    case 9:
  					printWeather();
  					break;    
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

  Time[7] = second_ % 10 + 48;
  Time[6] = second_ / 10 + 48;
  Time[4]  = minute_ % 10 + 48;
  Time[3]  = minute_ / 10 + 48;
  Time[1]  = hour_   % 10 + 48;
  Time[0]  = hour_   / 10 + 48;

  Date[0]  = day_   / 10 + 48;
  Date[1]  = day_   % 10 + 48;
  Date[3]  = month_  / 10 + 48;
  Date[4]  = month_  % 10 + 48;
  Date[6] = (year_  / 10) % 10 + 48;
  Date[7] = year_   % 10 % 10 + 48; 
}

//------------------------------------------------------------------------------
// Display time and date on the 16x2 LCD
//
void printTime(void)
{
  showDigit(Time[0] - '0', 0, 0);      // Hour
  showDigit(Time[1] - '0', 1, 0);
  showDigit(Time[3] - '0', 2, 1);      // Minute
  showDigit(Time[4] - '0', 3, 1);
  lcd.setCursor(8, 0);                 // Colons between HH and MM ie. HH:MM
  lcd.print(".");
  lcd.setCursor(8, 1);
  lcd.print(".");
}  

//------------------------------------------------------------------------------
// Function to print large character 3x3
//
void showDigit(int digit, int position, int offset)
{
  lcd.setCursor(position * (digitWidth + 1) + offset, 0);
  for (int i = 0; i < digitWidth; i++)
    lcd.print(bigDigitsTop[digit][i]);
  lcd.setCursor(position * (digitWidth + 1) + offset, 1);
  for (int i = 0; i < digitWidth; i++)
    lcd.print(bigDigitsBot[digit][i]);
}

//-------------------------------------------------------------------------------------
// Function to get weather data
//
void getWeather( void )
{
	if(WiFi.status()== WL_CONNECTED){
		String serverPath = "http://api.openweathermap.org/data/2.5/weather?q=" + city + "," + countryCode + "&APPID=" + openWeatherMapApiKey;

  	jsonBuffer = httpGETRequest(serverPath.c_str());
    //Serial.println(jsonBuffer);
    JSONVar myObject = JSON.parse(jsonBuffer);
  
    // JSON.typeof(jsonVar) can be used to get the type of the var
    if (JSON.typeof(myObject) == "undefined") {
      Serial.println("Parsing input failed!");
    }
    else {
			weather = JSON.stringify(myObject["weather"][0]["main"]);
			int n = weather.length();
			weather.remove(n-1,1);
			weather.remove(0,1);
			
      String temp = JSON.stringify(myObject["main"]["temp"]);
      temperature = temp.toInt() - 273.15;
      
      temp = JSON.stringify(myObject["main"]["humidity"]);
      humidity = temp.toInt();
    }
	}
}

//------------------------------------------------------------------------------
// Function to get data using HTTPGET
//
String httpGETRequest(const char* serverName) 
{
  WiFiClient client;
  HTTPClient http;
    
  // Your IP address with path or Domain name with URL path 
  http.begin(client, serverName);
  
  // Send HTTP POST request
  int httpResponseCode = http.GET();
  
  String payload = "{}"; 
  
  if (httpResponseCode>0) {
    Serial.print("HTTP Response code: ");
    Serial.println(httpResponseCode);
    payload = http.getString();
  }
  else {
    Serial.print("Error code: ");
    Serial.println(httpResponseCode);
  }
  // Free resources
  http.end();

  return payload;
}

//------------------------------------------------------------------------------
// Dsiplay weather info etc...
//
void printWeather(void)
{
  lcd.setCursor(0, 0);              // Print date
  lcd.print(Date[0]);
  lcd.print(Date[1]);
  lcd.setCursor(3, 0);
  lcd.print(Month[month_-1]);
  lcd.setCursor(7, 0);              // Print time
  lcd.print(Time);
  lcd.setCursor(0, 1);              // Print weather
  lcd.print(temperature);
  lcd.print("\337");
  lcd.setCursor(4, 1);
  lcd.print(humidity);
  lcd.print("%");
  lcd.setCursor(8,1);
  lcd.print(weather);
}
