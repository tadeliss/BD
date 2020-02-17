#include <WiFi.h>
#include <BlynkSimpleEsp32.h>
#include <SPI.h>
#include <SimpleTimer.h>
#include <TimeLib.h>
#include <WidgetRTC.h>
#include <WiFiClient.h>

int isFirstConnect = true;

// You should get Auth Token in the Blynk App.
// Go to the Project Settings (nut icon).
//Blynk atpazinimo kodas
char auth[] = "hKeENDdhyYgTWMNzOvLqoNfpO2tLg5oR";

//Wifi duomenys
char ssid[] = "Tado apartamentai";
char pass[] = "tadas111";

SimpleTimer timer;

WidgetRTC rtc;

//BLYNK_ATTACH_WIDGET(rtc, V5);

BLYNK_CONNECTED() {
if (isFirstConnect) {
Blynk.syncAll();
isFirstConnect = false;
}
}
void setup() {
  // put your setup code here, to run once:

}

void loop() {
  // put your main code here, to run repeatedly:

}
