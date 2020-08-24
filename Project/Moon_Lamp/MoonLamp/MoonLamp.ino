//Author: nhantam12022002
//Email: tam8493@gmail.com
//Date: 14/08/2019
//Revision: V1.0
//License: ITS Future
#include <Blynk.h>
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <EEPROM.h>
#include <BlynkSimpleEsp8266.h>
#include <Adafruit_NeoPixel.h>
#include <SPI.h>
#define  BLYNK_PRINT Serial // Comment this out to disable prints and save space
#define  PIN 2
#define  numberOfNeopixels 4
#define  BLYNK_PRINT Serial
Adafruit_NeoPixel strip = Adafruit_NeoPixel(numberOfNeopixels, PIN, NEO_GRB + NEO_KHZ800);
ESP8266WebServer server(80);
const char*     ssid           = "ITS FUTURE";
const char*     passphrase     = "SCIENCEFAIR2019";
String          content;
String          st;
int             statusCode,Brightness,R,G,B;
void setup()
{
    Serial.begin(115200);
    EEPROM.begin(512);
    strip.begin(); //start neopixels
    strip.show();
    delay(10);
    Serial.println("Startup");
    Serial.println("Reading EEPROM ssid");
    String esid;
    for (int i = 0; i < 32; ++i) // do dai ma token blynk
    {
        esid += char(EEPROM.read(i));
    }
    Serial.print("SSID: ");
    Serial.println(esid.c_str());
    esid.trim();
 
    Serial.println("Reading EEPROM pass");
    String epass = "";
    for (int i = 32; i < 96; ++i)
    {
        epass += char(EEPROM.read(i));
    }
    Serial.print("PASS: ");
    Serial.println(epass.c_str());
    epass.trim();
 
    Serial.println("Reading EEPROM blynk");
    String eblynk = "";
    for (int i = 96; i < 128; ++i)
    {
        eblynk += char(EEPROM.read(i));
    }
    Serial.print("BLYNK: ");
    Serial.println(eblynk.c_str());
    eblynk.trim();
 
    if ( esid.length() > 1 )
    {
        WiFi.begin(esid.c_str(), epass.c_str());
        if (testWifi())
        {
            launchWeb(0);
            WiFi.disconnect();
 
            char * auth_ = new char[eblynk.length() + 1];
            eblynk.toCharArray(auth_, eblynk.length() + 1);
            Blynk.begin(auth_, esid.c_str(), epass.c_str());
            EEPROM.end();
            return;
        }
    }
    setupAP();
    EEPROM.end();
}
 
bool testWifi(void)
{
    int c = 0;
    Serial.println("DANG KET NOI TOI WIFI ");
    while ( c < 20 )
    {
        if (WiFi.status() == WL_CONNECTED)
        {
            return true;
        }
        delay(1000);
        Serial.print(WiFi.status());
        c++;
    }
    Serial.println("");
    Serial.println("KHONG THE KET NOI. VUI LONG KET NOI TOI MODULE WIFI VA NHAP DIA CHI 192.168.4.1 ");
    return false;
}
 
void launchWeb(int webtype)
{
    Serial.println("");
    Serial.println("WiFi DA KET NOI ");
    Serial.print("DIA CHI MANG IP: ");
    Serial.println(WiFi.localIP());
    Serial.print("DIA CHI WIFI IP: ");
    Serial.println(WiFi.softAPIP());
    createWebServer(webtype);
    // Start the server
    server.begin();
    Serial.println("MAY CHU BAT DAU ");
}
 
void setupAP(void)
{
    WiFi.mode(WIFI_STA);
    WiFi.disconnect();
    delay(100);
    int n = WiFi.scanNetworks();
    Serial.println("HOAN TAT TIM KIEM ");
    if (n == 0)
    {
        Serial.println("KHONG TIM THAY WIFI");
    }
    else
    {
        Serial.print(n);
        Serial.println("WIFI TIM THAY ");
        for (int i = 0; i < n; ++i)
        {
            // Print SSID and RSSI for each network found
            Serial.print(i + 1);
            Serial.print(": ");
            Serial.print(WiFi.SSID(i));
            Serial.print(" (");
            Serial.print(WiFi.RSSI(i));
            Serial.print(")");
            Serial.println((WiFi.encryptionType(i) == ENC_TYPE_NONE) ? " " : "*");
            delay(10);
        }
    }
    Serial.println("");
    st = "<ol>";
    for (int i = 0; i < n; ++i)
    {
        // Print SSID and RSSI for each network found
        st += "<li>";
        st += WiFi.SSID(i);
        st += " (";
        st += WiFi.RSSI(i);
        st += ")";
        st += (WiFi.encryptionType(i) == ENC_TYPE_NONE) ? " " : "*";
        st += "</li>";
    }
    st += "</ol>";
    delay(100);
    Serial.println("DIA CHI WIFI IP: ");
    Serial.println(ssid);
    Serial.println(passphrase);
    WiFi.softAP(ssid, passphrase, 6);
 
    launchWeb(1);
    Serial.println("KET THUC ");
}
 
void createWebServer(int webtype)
{
    if ( webtype == 1 )
    {
        server.on("/", []()
        {
            IPAddress ip = WiFi.softAPIP();
            String ipStr = String(ip[0]) + '.' + String(ip[1]) + '.' + String(ip[2]) + '.' + String(ip[3]);
            content = "<!DOCTYPE HTML>\r\n<html><h2>DANG NHAP WIFI NHA BAN VA MA AUTHTOKEN BLYNK </h2>";
            //content += ipStr;
            //content += "<form method='get' action='setting'><table width='100%' border='1'><tr><td width=\"30%\"><label>Wifi</label></td><td width=\"70%\><input name='ssid' length=32 width='500'></td></tr><tr><td><label>Password</label></td><td><input name='pass' length=64 width='500'></td></tr><tr><td><label>Blynk</label></td><td><input name='blynk' length=32 width='500'></td></tr><tr><td></td><td><input type='submit'></tr></tr></table></form>";
            content += "<form method=\"get\" action=\"setting\">";
            content += "<div>WIFI</div>";
            content += "<div><input name=\"ssid\" size=\"40\"></div>";
            content += "<div>MAT KHAU</div>";
            content += "<div><input name=\"pass\" size=\"40\"></div>";
            content += "<div>Blynk TOKEN</div>";
            content += "<div><input name=\"blynk\" size=\"40\"></div>";
            content += "<div><input type='submit'></div>";
 
            content += "<p>";
            content += st;
            content += "</p>";
            content += "</html>";
            server.send(200, "text/html", content);
              });
        server.on("/setting", []()
        {
            String qsid = server.arg("ssid");
            String qpass = server.arg("pass");
            String qblynk = server.arg("blynk");
            if (qsid.length() > 0 && qpass.length() > 0)
            {
                EEPROM.begin(512);
                Serial.println("clearing eeprom");
                for (int i = 0; i < 128; ++i)
                {
                    EEPROM.write(i, 0);
                }
                EEPROM.commit();
                Serial.println(qsid);
                Serial.println("");
                Serial.println(qpass);
                Serial.println("");
                Serial.println(qblynk);
                Serial.println("");
 
                Serial.println("writing eeprom ssid:");
                for (int i = 0; i < qsid.length(); ++i)
                {
                    EEPROM.write(i, qsid[i]);
                    Serial.print("Wrote: ");
                    Serial.println(qsid[i]);
                }
 
                Serial.println("writing eeprom pass:");
                for (int i = 0; i < qpass.length(); ++i)
                {
                    EEPROM.write(32 + i, qpass[i]);
                    Serial.print("Wrote: ");
                    Serial.println(qpass[i]);
                }
 
                Serial.println("writing eeprom blynk:");
                for (int i = 0; i < qblynk.length(); ++i)
                {
                    EEPROM.write(96 + i, qblynk[i]);
                    Serial.print("Wrote: ");
                    Serial.println(qblynk[i]);
                }
                EEPROM.commit();
                EEPROM.end();
                //Chop den  sau khi lam xong
                pinMode(13, OUTPUT);
                digitalWrite(13, LOW);
                digitalWrite(13, HIGH);
                delay(500);
                digitalWrite(13, LOW);
                content = "{\"DA LUU VAO HE THONG. VUI LONG KHOI DONG LAI MODULE \"}";
                statusCode = 200;
            }
            else
            {
                content = "{\"Error\":\"404 not found\"}";
                statusCode = 404;
                Serial.println("Sending 404");
            }
            server.send(statusCode, "application/json", content);
        });
    }
    else if (webtype == 0)
    {
        server.on("/", []()
        {
            IPAddress ip = WiFi.localIP();
            String ipStr = String(ip[0]) + '.' + String(ip[1]) + '.' + String(ip[2]) + '.' + String(ip[3]);
            server.send(200, "application/json", "{\"IP\":\"" + ipStr + "\"}");
        });
        server.on("/cleareeprom", []()
        {
            content = "<!DOCTYPE HTML>\r\n<html>";
            content += "<h2>ITS Future</h2><p>Clearing the EEPROM</p></html>";
            server.send(200, "text/html", content);
            Serial.println("clearing eeprom");
            for (int i = 0; i < 128; ++i)
            {
                EEPROM.write(i, 0);
            }
            EEPROM.commit();
        });
    }
}
BLYNK_WRITE(V0)
{
R = param[0].asInt();
G = param[1].asInt();
B = param[2].asInt();
Serial.println(R);
Serial.println(G);
Serial.println(B);
for(int i=0;i<numberOfNeopixels;i++){
strip.setPixelColor(i,R,G,B);
strip.setBrightness(Brightness);
strip.show();
}
}
BLYNK_WRITE(V1) 
{
  int pinValue = param.asInt();
  Serial.print("V1 Slider value is: ");
  Serial.println(pinValue);
  Brightness = pinValue;
  for(int i=0;i<numberOfNeopixels;i++){
  strip.setPixelColor(i,R,G,B);
  strip.setBrightness(Brightness);
  strip.show();
}
}
BLYNK_WRITE(V2){
  int pinValue=param.asInt();
  if(pinValue==1){
    for(int i=0;i<numberOfNeopixels;i++){
    strip.setPixelColor(i,0,0,0);
    strip.setBrightness(0);
    strip.show();
}
}
}
void loop()
{
    server.handleClient();
    Blynk.run();
}