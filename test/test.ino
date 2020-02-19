#define BLYNK_PRINT Serial
#include <Wire.h>
#include <Adafruit_SSD1306.h>
#include <Adafruit_GFX.h>
#include <WiFi.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>
#include <SimpleTimer.h>
#include <TimeLib.h>
#include <WidgetRTC.h>
//long TimeSec;
int skait;

char auth[] = "hKeENDdhyYgTWMNzOvLqoNfpO2tLg5oR";
//Wifi duomenys
char ssid[] = "Tado apartamentai";
char pass[] = "tadas111";

SimpleTimer timer;
WidgetRTC rtc;

int isFirstConnect = true;
BLYNK_CONNECTED() {
if (isFirstConnect) {
 Blynk.syncAll();
 isFirstConnect = false;
}
}
void setup() {
Serial.begin(9600);  
 Blynk.begin(auth, ssid, pass);
  while(Blynk.connect() == false);

rtc.begin();
  
 timer.setInterval(5000L, clockvalue);
 timer.setInterval(1000L, sync) ;
  
}

BLYNK_WRITE(V4){
 skait = param.asInt();
}
BLYNK_WRITE(V5){
 long TimeSec = param[0].asLong();
 int val = hour();
 int minut = minute();
 long laikSec = val*3600+minut*60;
if(TimeSec==laikSec){
  Serial.println("achujenai");
}
}

void sync(){
   Blynk.syncVirtual(V5);
}

void loop() {
  Blynk.run();
  timer.run(); 

}

void clockvalue()
{
 int gmthour = hour();
  if (gmthour == 24){
     gmthour = 0;
  }
  String displayhour =   String(gmthour, DEC);
  int hourdigits = displayhour.length();
  if(hourdigits == 1){
    displayhour = "0" + displayhour;
  }
  String displayminute = String(minute(), DEC);
  int minutedigits = displayminute.length();  
  if(minutedigits == 1){
    displayminute = "0" + displayminute;
  }  
  String displaycurrenttime = displayhour + ":" + displayminute;
  Blynk.virtualWrite(V2, displaycurrenttime);

}
