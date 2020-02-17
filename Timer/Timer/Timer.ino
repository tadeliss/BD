

#define BLYNK_PRINT Serial    // Comment this out to disable prints and save space
#include <WiFi.h>
#include <BlynkSimpleEsp32.h>
#include <SPI.h>
//#include <SimpleTimer.h>
#include <TimeLib.h>
#include <WidgetRTC.h>
int isFirstConnect = true;


// You should get Auth Token in the Blynk App.
// Go to the Project Settings (nut icon).
//Blynk atpazinimo kodas
char auth[] = "hKeENDdhyYgTWMNzOvLqoNfpO2tLg5oR";

//Wifi duomenys
char ssid[] = "Tado apartamentai";
char pass[] = "tadas111";

//SimpleTimer timer;
BlynkTimer timer;
WidgetRTC rtc;

BLYNK_CONNECTED() {
if (isFirstConnect) {
Blynk.syncAll();
isFirstConnect = false;
}
}


void setup()
{
  Serial.begin(9600);
  Blynk.begin(auth, ssid, pass);
 while(Blynk.connect() == false);
  rtc.begin();

  // Display digital clock every 10 seconds
  timer.setInterval(5000L, clockvalue);
}

// Real-time laikas
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
  Blynk.virtualWrite(V8, displaycurrenttime);

}
void loop()
{
  Blynk.run();
  timer.run();
}
