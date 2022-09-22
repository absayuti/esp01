#ESP-01 dan Arduino IDE
#Memprogram Modul ESP-01 dengan Perisian Arduino IDE

#Apa itu Modul ESP-01?
Modul Wifi berasaskan mikrokontroler ESP8266
Saiz kecil, bilangan pin terhad
Boleh diprogram dengan Arduino IDE

Perkakasan yang diperlukan
Modul ESP-01
ESP-01 WiFi Serial Transceiver Module (ESP8266)
Papan penyesuai USB-ESP01
USB to ESP01 (ESP8266) Serial Adapter Board
Suis butang tekan
Push-button switch
Wayar
Jumper wire

'Adapter board’ perlu diubah suai
Papan penyesuai USB-ESP01 perlu diubah suai jika anda mahu memprogram ESP01 dengannya.
Mudah sahaja: dua wayar dan satu suis

ATAU…. dapatkan ‘Programmer-adapter board’

Langkah-langkah ubah-suai 'Adapter board'
Solder wayar pada pin GPIO-0 dan GND
https://www.instructables.com/USB-to-ESP-01-Board-Adapter-Modification/
Sambung pada suis butang tekan
Lekatkan suis pada bahagian tepi 'adapter' (opsional)

Settings pada Arduino IDE
Jenis board yang sepadan untuk ESP8266 perlu dipasang dalam Arduino IDE
Menu: File >> Preferences
Additional Board Manager: “http://arduino.esp8266.com/stable/package_esp8266com_index.json” 
[OK]
Menu: Tools > Board > Boards Manager…
Di ruang carian, taip: “ESP8266”
Tunggu…
Pilih “ESP8266 by ESP8266 Community”
[Install]
[Close]
Menu: Tools >> Board: “Generic ESP8266 module”
Rujukan:
https://create.arduino.cc/projecthub/ROBINTHOMAS/programming-esp8266-esp-01-with-arduino-011389 (STEP 2:Installing ESP8266 Platform)


Menggunakan adapter board yang diubah-suai
Lekatkan modul ESP01 pada adapter
Tekan butang semasa memasukkan adapter
Lepaskan butang
Sekarang ESP-01 berada dalam Flash Mode atau programming mode
Setkan board dan port yang sesuai (perlu buat sekali sahaja)
Tools >> Board: “Generic ESP8266 module”
Tools >> Port >> (pilih port yang sesuai dengan ESP-01 anda) 
Tulis program
Compile dan upload program ke ESP01
Jika upload berjaya, uji program yg tertulis dalam ESP-01
Program berikut ini menggunakan Serial Monitor untuk menghantar data ke Serial Monitor. Jadi, buka Serial Monitor.
Untuk menukar ESP-01 kepada flash mode semula, cabut adapter dan masukkan semula sambil menekan butang suis.

Sketch P1: Menguji ESP-01
void setup() 
{
  Serial.begin(115200);
  Serial.println("Hello world");
}

void loop() 
{
  Serial.println("Hello again from ESP-01");
  delay(1000);
}

