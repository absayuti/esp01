## ESP-01 dan LCD
Menyambung Modul ESP-01 ke Modul Paparan LCD 16x2

### Perkakasan
* Modul ESP-01
* Modul LCD 16x2 dengan I2C interface
* Power adapter 5V
* Stepdown converter 5V to 3.3V. Jika mengggunakan Modul LCD 16x2 jenis 3.3V, maka power adapter tidak diperlukan
* Female header 2 rows x 8 pins (atau lebih) atau female header 1 row x 16 pins (atau lebih)

### Litar
* Keperluan voltan berbeza
* Penyesuai voltan 5v kpd 3.3v
* Sambungan pada ESP01 perlu dipermudahkan. Supaya boleh dicucuk (dan dicabut) pada (dari) USB-ESP01 adapter; dan dicucuk (dicabut) pada (dari) litar

~~Sambung litar seperti ditunjukkan:~~

## Sketch
* Install LiquidCrystal_I2C library
  - https://github.com/agnunez/ESP8266-I2C-LCD1602
  - Download it in as zip file. To install, use the menu: Sketch >>  Include library >> Add .ZIP library.
  - This library (and other similars) seems to have two issues:
    - There’s a bug in its print() method. Need code fixes as shown below.
    - Will not compile with the new Arduino IDE 2.0. Works OK with IDE 1.8.19.
  - Add the following code in your sketch to fix the print() bug:
```
      #define printIIC(args) Wire.write(args)
      inline size_t LiquidCrystal_I2C::write(uint8_t value) {
          send(value, Rs);
          return 1;
      }
```
* Test sketch
```
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
```

## Reference:
* https://simple-circuit.com/esp8266-esp-01-i2c-lcd/
* https://forum.arduino.cc/t/a-problem-with-my-liquidcrystal_i2c-display/348684/7

Youtube video [available here](https://youtu.be/_KkJnoVWE6o)
