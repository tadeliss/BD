/*
  Tadas Raiselis KIKf-16, 2020m. Bakalauro baigiamasis darbas, El. fakultetas
  v0 - variklio mygtukas
  v1 - maisto talpa
  v2 - laikrodis
  v3 - 1 laikmatis
  v4 - 1 kiekis
  v5 - 2 laikmatis
  v6 - 2 kiekis
  v7 - siurblio valdymo jungiklis
  
  */

#define BLYNK_PRINT Serial
#include <Wire.h>
#include <Adafruit_SSD1306.h>
#include <Adafruit_GFX.h>
#include <WiFi.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>
#include <TimeLib.h>
#include <WidgetRTC.h>
#include "HX711.h"

//oled nustatymai
#define SCREEN_WIDTH 128 // OLED plotis pikseliais
#define SCREEN_HEIGHT 64 // OLED aukstis pikseliais
#define OLED_RESET     4 // reset




Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

//isvadu nustatymas
const int CLK = 19; //svarstykliu SCK
const int DOUT = 18; //svarstykliu DT
const int dirPin = 16; //Variklio krypties
const int stepPin = 17; //Variklio zingsnio
const int trigPin = 4; //Ultragarso trig
const int echoPin = 2; //Ultragarso echo
const int sleepPin = 3; // miego rezimo
const int  vandensPin = 25; //vandens siurblio

float calibration_factor = -424650;  //nustatomas kalibracijos svarstyklėms skaičius
long t; //garso signalo sugaistas laikas ultragarso funkcijoje
int ats; //atstumas ultragarso funkcijoje
int proc; //atstumas procentais ultragarso funkcijoje
int kiek1, kiek2;
int i = 0;
int isFirstConnect = true;
float svoris; //svarstykliu duomenu kintamasis
int valand;
int minut;
HX711 scale(DOUT, CLK);

//Blynk atpazinimo kodas
char auth[] = "hKeENDdhyYgTWMNzOvLqoNfpO2tLg5oR";

//Wifi duomenys
char ssid[] = "Tado apartamentai";
char pass[] = "tadas111";

BlynkTimer timer;
WidgetRTC rtc;

//tikrinama ar prisijungta i blynk serveri, prisijungus sinchronizuojami duomenys
BLYNK_CONNECTED() {
  if (isFirstConnect) {
    Blynk.syncAll();
    isFirstConnect = false;
  }
}

void setup() {

  Serial.begin(9600);
  pinMode(vandensPin, OUTPUT);
  pinMode(stepPin, OUTPUT);
  pinMode(dirPin, OUTPUT);
  pinMode(trigPin, OUTPUT);
  pinMode(sleepPin, OUTPUT);
  pinMode(echoPin, INPUT);

  //uzmigdomas variklis
  digitalWrite(sleepPin, LOW);

  //inicijuojamas Blynk
  Blynk.begin(auth, ssid, pass);
  while (Blynk.connect() == false);

  //real-time-clock pradzia
  rtc.begin();

  //nustatomi oled parametrai
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { // adresas 0x3C kur OLED
    Serial.println(F("SSD1306 allocation failed"));
    for (;;); // jei nepavyko prijungti oled, netesiama
  }
  delay(1000);
  display.setFont();
  display.clearDisplay();
  display.setTextColor(WHITE);

  //svarstyklės iniciavimas ir kalibravimas
  scale.set_scale();
  scale.tare();

  //funkciju laikmaciai
  timer.setInterval(59000L, synckiek);
  timer.setInterval(90000L, synci);
  timer.setInterval(100000L, ultragarsasBlynk);
  timer.setInterval(5000L, clockvalue);
  timer.setInterval(15000L, syncvand);
  timer.setInterval(105000L, oled);

//  
ultragarsas();
delay(100);
oled();
}

//kiek kartu pasukti varikli(1)
BLYNK_WRITE(V4) {
  kiek1 = param.asInt();
}

//kiek kartu pasukti varikli(2)
BLYNK_WRITE(V6) {
  kiek2 = param.asInt();
}

//paspaudus mygtuka pasukamas variklis
BLYNK_WRITE(V0) {

  if (param.asInt() == 1) {
    oledmait();
    delay(500);
    variklis(300, 100);
  }
  valand = hour();
  minut = minute();
 oled(); 
}
// tikrinama ar ijungtas vandens tiekimo mygtukas
BLYNK_WRITE(V7) {
  if (param.asInt() == 1) {
    scale.set_scale(calibration_factor);
    svoris = scale.get_units();
     
  delay (100);
    if (svoris < 0.150) {
      pinMode(echoPin, OUTPUT);
      delay(100);
      oledvand();
      
      digitalWrite(vandensPin, HIGH);
      delay(4000);
      digitalWrite(vandensPin, LOW);
      delay(100);
      pinMode(echoPin, INPUT);
    }
    oled();
  }
}
//nustatytam laikui atejus pasukamas variklis (1 laikmatis)
BLYNK_WRITE(V3) {

  Blynk.syncVirtual(V4);

  long TimeSec = param[0].asLong();
  int val = hour();
  int minut = minute();
  long laikSec = val * 3600 + minut * 60;

  if (TimeSec == laikSec && i == 0) {
    i = i + 1;

    for (int x = 0; x < kiek1; x++) {

      variklis(300, 100);

      delay(500);
    }
  valand = hour();
  minut = minute();
    oled();
  }
}

//nustatytam laikui atejus pasukamas variklis (2 laikmatis)
BLYNK_WRITE(V5) {

  Blynk.syncVirtual(V6);

  long TimeSec = param[0].asLong();
  int val = hour();
  int minut = minute();
  long laikSec = val * 3600 + minut * 60;

  if (TimeSec == laikSec && i == 0) {
    i = i + 1;

    for (int x = 0; x < kiek2; x++) {

      variklis(300, 100);

      delay(500);
    }
  valand = hour();
  minut = minute();
    oled();
  }
}

//atnaujina vandens mygtuko reiksme
void syncvand() {
  Blynk.syncVirtual(V7);
}

//atnaujina kiek ir laikmacio reiksmes, atlieka palyginima
void synckiek() {
  Blynk.syncVirtual(V3);
  Blynk.syncVirtual(V5);
}

//atnaujina i reiksme
void synci() {
  i = 0;
}

//ultragarso siunciamas kintamasis i blynk
void ultragarsasBlynk() {
  ultragarsas();
  Blynk.virtualWrite(V1, proc);
}

void loop() {
  Blynk.run();
  timer.run();
}

//laikrodzio funkcija
void clockvalue()
{
  int gmthour = hour();
  if (gmthour == 24) {
    gmthour = 0;
  }
  String displayhour =   String(gmthour, DEC);
  int hourdigits = displayhour.length();
  if (hourdigits == 1) {
    displayhour = "0" + displayhour;
  }
  String displayminute = String(minute(), DEC);
  int minutedigits = displayminute.length();
  if (minutedigits == 1) {
    displayminute = "0" + displayminute;
  }
  String displaycurrenttime = displayhour + ":" + displayminute;
  Blynk.virtualWrite(V2, displaycurrenttime);
}

//Isvedimas i oled
void oled()
{
  display.clearDisplay(); // isvalomas oled
  display.setTextSize(2); //nustatomas teksto dydis
  display.setCursor(0,0); // nustatomas pradzios taskas
  delay(100);
  display.print(proc); //isvedimas i oled
  display.print("%");
  display.setTextSize(3);
  display.setCursor(0,20);
  display.print(valand);
  display.print(":");
  display.print(minut);
  display.display();
  delay(100);

}
void oledmait() {
  display.clearDisplay(); // isvalomas oled

  display.setTextSize(2); //nustatomas teksto dydis
  display.setCursor(0, 0); // nustatomas pradzios taskas
  delay(100);
  display.print ("Beriamas");
  display.println ("  Maistas");
  display.display();
  delay(100);

}

void oledvand() {
  display.clearDisplay(); // isvalomas oled

  display.setTextSize(2); //nustatomas teksto dydis
  display.setCursor(0, 0); // nustatomas pradzios taskas
  delay(100);
  display.print ("Pilamas");
  display.println ("   Vanduo");
  display.display();
  delay(100);

}

int ultragarsas()
{
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  t = pulseIn(echoPin, HIGH); // laiko paskaiciavimas uztrukto garsui atkeliauti
  ats = t * 0.0344 / 2; //atstumo paskaiciavimas pagal sugaista laika

  //skaiciavimas procentais
  float x = (((ats + 1) * 100) / 10) - 30;

  if (ats <= 2) {
    proc = 100;
  } else if (ats >= 12) {
    proc = 0;
  } else {
    proc = 100 - x;
  }
  return proc;
  Serial.println(ats);
}


//Variklio pasukimo funkcija
void variklis(int stepsLeft, int stepsRight) {

  //prikeliamas variklis
  digitalWrite(sleepPin, HIGH);
  delay(10);

  //Pasukamas i viena puse
  digitalWrite(dirPin, HIGH);
  for (int x = 0; x < stepsLeft; x++) {
    digitalWrite(stepPin, HIGH);
    delayMicroseconds(2000);
    digitalWrite(stepPin, LOW);
    delayMicroseconds(2000);
  }

  //Pasukamas i kita puse
  digitalWrite(dirPin, LOW);
  for (int x = 0; x < stepsRight; x++) {
    digitalWrite(stepPin, HIGH);
    delayMicroseconds(1000);
    digitalWrite(stepPin, LOW);
    delayMicroseconds(1000);
  }

  //Pasukamas i viena puse
  digitalWrite(dirPin, HIGH);
  for (int x = 0; x < stepsLeft; x++) {
    digitalWrite(stepPin, HIGH);
    delayMicroseconds(2000);
    digitalWrite(stepPin, LOW);
    delayMicroseconds(2000);
  }

  //Pasukamas i kita puse
  digitalWrite(dirPin, LOW);
  for (int x = 0; x < stepsRight; x++) {
    digitalWrite(stepPin, HIGH);
    delayMicroseconds(1000);
    digitalWrite(stepPin, LOW);
    delayMicroseconds(1000);
  }

  //uzmigdomas variklis
  digitalWrite(sleepPin, LOW);
}
