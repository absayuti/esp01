/*******************************************************************************
  Sketch P03

  Using ESP-01 WiFi module to display hello world on i2c LCD 16x2
 

  Ref: 
      https://create.arduino.cc/projecthub/akshayjoseph666/interface-i2c-16x2-lcd-with-arduino-uno-just-4-wires-273b24
      Theories, sketches, link to library

  LiquidCrystal_I2C Library:
      https://github.com/agnunez/ESP8266-I2C-LCD1602

      Fix bug in the library with:
          #define printIIC(args) Wire.write(args)
          inline size_t LiquidCrystal_I2C::write(uint8_t value) {
            send(value, Rs);
          return 1;
      }



  Circuit:
      https://simple-circuit.com/esp8266-esp-01-internet-clock-wifi/
  

  Board settings
      Menu: File >> Preferences
      Additional Board Manager: “http://arduino.esp8266.com/stable/package_esp8266com_index.json” 
      [OK]
      Menu: Tools > Board > Boards Manager…
      In search box, type: “ESP8266”
      wait …
      Choose “ESP8266 by ESP8266 Community”
      Click [Install]
      Click [Close]
      Menu: Tools >> Board: “Generic ESP8266 module”
  
  
/******************************************************************************/


#include <Wire.h>
#include <LiquidCrystal_I2C.h>       // Include LiquidCrystal_I2C library 
 
LiquidCrystal_I2C lcd(0x27, 16, 2);  // Configure LiquidCrystal_I2C library with 0x27 address, 16 columns and 2 rows

// The following macro is to fix a bug in the library.
// Printing problem when compiled using current/newer IDE.
// The write function supposed to return 1 when done/OK

#define printIIC(args) Wire.write(args)
inline size_t LiquidCrystal_I2C::write(uint8_t value) {
  send(value, Rs);
  return 1;
}

void setup()
{
  lcd.begin(0,2);                   // Initialize I2C LCD module (SDA = GPIO0, SCL = GPIO2)
  lcd.print("Hello world! ");
}

void loop()
{
  delay(1000);
  lcd.setCursor(0,1);
  lcd.print("It works!");
  delay(1000);
  lcd.setCursor(0,1);
  lcd.print("         ");
}
