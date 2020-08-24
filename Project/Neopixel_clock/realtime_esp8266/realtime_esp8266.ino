/*
   This sketch shows an example of sending a reading to data.sparkfun.com once per day.
   It uses the Sparkfun testing stream so the only customizing required is the WiFi SSID and password.
   The Harringay Maker Space
   License: Apache License v2
*/
#include <NTPtimeESP.h>
#include <Adafruit_NeoPixel.h>
#include <EEPROM.h>
const byte DS1307 = 0x68;
const byte NumberOfFields = 7;
const int PIN     =D2;
const int NUMPIXELS  =12;
const int BRIGHTNESS =50;
Adafruit_NeoPixel pixels(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);
NTPtime NTPch("ch.pool.ntp.org");   // Server NTP
char *ssid      = "TP-LINK";               // Ten WiFi SSID
char *password  = "*244466666#";               // Mat khau wifi
int nowpix, nowrain;
int style;
/*
 * Cac truong co trong struct DateTime:
 * struct strDateTime
{
  byte hour;
  byte minute;
  byte second;
  int year;
  byte month;
  byte day;
  byte dayofWeek;
  boolean valid;
};
 */
strDateTime dateTime;
int s,m,h;
void setup() {
  Serial.begin(115200);
  Serial.println();
  Serial.println("Booted");
  Serial.println("Connecting to Wi-Fi");
  pixels.begin();
  nowpix = 0;
  WiFi.mode(WIFI_STA);
  WiFi.begin (ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
  Serial.println("WiFi connected");
}
uint32_t RainColor(int num)
{
  if (num <= 255) {
    return pixels.Color(255 - num, num, 0);
  } else if (num <= 511) {
    return pixels.Color(0, 255 - (num - 256), (num - 256));
  } else
  {
    return pixels.Color((num - 512), 0, 255 - (num - 512));
  }
}
void SetPixel0(int h, int m, int s)
{
  nowpix = (nowpix + 1) % 12;
  nowrain = (nowrain + 5) % 768;

  Serial.print(h);
  Serial.print(" ");
  Serial.print(m);
  Serial.print(" ");
  Serial.println(s);


  pixels.setPixelColor(h % 12, pixels.Color(0, 200, 0));
  pixels.setPixelColor((m / 5) % 12, pixels.Color(200, 0, 0));
  pixels.setPixelColor((s / 5) % 12, pixels.Color(0, 0, 200));
  pixels.show();

  delay(80);
  pixels.clear();
}


void loop() {

  // Tham so dau tien la Time zone duoi dang floating point (7.0 la VN); 
  // Tham so thu hai la DayLightSaving: 1 voi thoi gian; 2 la thoi gian US (o VN khong su dung)
  dateTime = NTPch.getNTPtime(7.0, 0);
  
  // Kiem tra dateTime.valid truoc khi dua ra gia tri thoi gian
  if(dateTime.valid){
    byte actualHour = dateTime.hour;      // Gio
    byte actualMinute = dateTime.minute;  // Phut
    byte actualsecond = dateTime.second;  // Giay
    int actualyear = dateTime.year;       // Nam
    byte actualMonth = dateTime.month;    // Thang
    byte actualday =dateTime.day;         // Ngay
    byte actualdayofWeek = dateTime.dayofWeek;
    s=actualsecond;
    m=actualMinute;
    h=actualHour;
    SetPixel0(h,m,s);
  }
  
 
  
}
