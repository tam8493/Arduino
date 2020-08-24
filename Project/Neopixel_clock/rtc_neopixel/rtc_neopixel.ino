#include <Adafruit_NeoPixel.h>
#include "Wire.h"
#define DS3231_I2C_ADDRESS 0x68
// Convert normal decimal numbers to binary coded decimal
#define PIXEL_PIN    2
#define NUMBERPIXEL 12
Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUMBERPIXEL, PIXEL_PIN, NEO_GRB + NEO_KHZ800);
// Argument 1 = Number of pixels in NeoPixel strip
// Argument 2 = Arduino pin number (most are valid)
// Argument 3 = Pixel type flags, add together as needed:
//   NEO_KHZ800  800 KHz bitstream (most NeoPixel products w/WS2812 LEDs)
//   NEO_KHZ400  400 KHz (classic 'v1' (not v2) FLORA pixels, WS2811 drivers)
//   NEO_GRB     Pixels are wired for GRB bitstream (most NeoPixel products)
//   NEO_RGB     Pixels are wired for RGB bitstream (v1 FLORA pixels, not v2)
//   NEO_RGBW    Pixels are wired for RGBW bitstream (NeoPixel RGBW products)
const uint32_t Red = pixels.Color(255, 0, 0);
const uint32_t Green = pixels.Color(0, 128, 0);
const uint32_t Blue = pixels.Color(0, 0, 200);
const uint32_t White = pixels.Color(68, 68, 68);
byte decToBcd(byte val) {
  return ( (val / 10 * 16) + (val % 10) );
}
// Convert binary coded decimal to normal decimal numbers
byte bcdToDec(byte val) {
  return ( (val / 16 * 10) + (val % 16) );
}
void setup() {
  Wire.begin();
  Serial.begin(9600);
  // set the initial time here:
  // DS3231 seconds, minutes, hours, day, date, month, year
  setDS3231time(30, 42, 16, 5, 13, 10, 16);
  pixels.begin(); // Initialize NeoPixel strip object (REQUIRED)
  pixels.show();  // Initialize all pixels to 'off'
}
void setDS3231time(byte second, byte minute, byte hour, byte dayOfWeek, byte
                   dayOfMonth, byte month, byte year) {
  // sets time and date data to DS3231
  Wire.beginTransmission(DS3231_I2C_ADDRESS);
  Wire.write(0); // set next input to start at the seconds register
  Wire.write(decToBcd(second)); // set seconds
  Wire.write(decToBcd(minute)); // set minutes
  Wire.write(decToBcd(hour)); // set hours
  Wire.write(decToBcd(dayOfWeek)); // set day of week (1=Sunday, 7=Saturday)
  Wire.endTransmission();
}
void readDS3231time(byte *second,
                    byte *minute,
                    byte *hour,
                    byte *dayOfWeek
                   )
{                   
Wire.beginTransmission(DS3231_I2C_ADDRESS);
Wire.write(0); // set DS3231 register pointer to 00h
Wire.endTransmission();
Wire.requestFrom(DS3231_I2C_ADDRESS, 7);
// request seven bytes of data from DS3231 starting from register 00h
*second = bcdToDec(Wire.read() & 0x7f);
*minute = bcdToDec(Wire.read());
*hour = bcdToDec(Wire.read() & 0x3f);
*dayOfWeek = bcdToDec(Wire.read());
}
void displayTime() {
  byte second, minute, hour, dayOfWeek;
  // retrieve data from DS3231
  readDS3231time(&second, &minute, &hour, &dayOfWeek);
  // send it to the serial monitor
  Serial.print(hour, DEC);
  // convert the byte variable to a decimal number when displayed
  Serial.print(":");
  if (minute < 10) {
    Serial.print("0");
  }
  Serial.print(minute, DEC);
  Serial.print(":");
  if (second < 10) {
    Serial.print("0");
  }
  Serial.print(second, DEC);
  Serial.println("");
  pixels.setPixelColor(hour % 12, Red);
  pixels.setPixelColor((minute / 5) % 12, Green);
  pixels.setPixelColor((second / 5) % 12, Blue);
  pixels.show();

  delay(80);
  Reset();
}
void loop() {
  displayTime(); // display the real-time clock data on the Serial Monitor,
  delay(1000); // every second
}
void Reset()
{
  for (int i = 0 ; i < 12; i++)
    pixels.setPixelColor(i, 0);
}
