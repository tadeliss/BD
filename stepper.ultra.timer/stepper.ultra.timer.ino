#define BLYNK_PRINT Serial
#include <Wire.h>
#include <Adafruit_SSD1306.h>
#include <Adafruit_GFX.h>
#include <WiFi.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>
#include <TimeLib.h>
#include <WidgetRTC.h>

#define SCREEN_WIDTH 128 // OLED plotis pikseliais
#define SCREEN_HEIGHT 64 // OLED aukstis pikseliais
#define OLED_RESET     4 // reset

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

const int dirPin = 16; //Variklio krypties
const int stepPin = 17; //Variklio zingsnio
const int trigPin = 4; //Ultragarso trig 
const int echoPin = 5; //Ultragarso echo
const int sleepPin = 3; // miego rezimo 
int isFirstConnect = true;
long t; //garso signalo sugaistas laikas
int ats; //atstumas
int proc; //atstumas procentais
int kiek;
//Blynk atpazinimo kodas
char auth[] = "hKeENDdhyYgTWMNzOvLqoNfpO2tLg5oR";
//Wifi duomenys
char ssid[] = "Tado apartamentai";
char pass[] = "tadas111";

BlynkTimer timer;
WidgetRTC rtc;

BLYNK_CONNECTED(){
  if(isFirstConnect){
    Blynk.syncAll();
    isFirstConnect = false;
 }
}

void setup(){
  Serial.begin(9600);
  
  pinMode(stepPin, OUTPUT);
  pinMode(dirPin, OUTPUT);
  pinMode(trigPin, OUTPUT);
  pinMode(sleepPin, OUTPUT);
  pinMode(echoPin, INPUT);
  
//inicijuojamas Blynk 
  Blynk.begin(auth, ssid, pass);
  while(Blynk.connect() == false);
  
//real-time-clock pradzia 
  rtc.begin();
  
//nustatomi oled parametrai
  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)){ // adresas 0x3C kur OLED
    Serial.println(F("SSD1306 allocation failed"));
    for(;;); // Don't proceed, loop forever
  }
  delay(1000);
  display.setFont();
  display.clearDisplay();
  display.setTextColor(WHITE);
  
//funkciju laikmaciai
  timer.setInterval(10000L, ultragarsasBlynk); 
  timer.setInterval(5000L, clockvalue); 
//  timer.setInterval(1000L, Blynk.syncVirtual(V4));
  
  digitalWrite(sleepPin, LOW); //uzmigdomas variklis
}

//ultragarso siunciamas kintamasis i blynk
void ultragarsasBlynk(){
  ultragarsas();
  Blynk.virtualWrite(V1, proc);
}

//kiek kartu pasukti varikli
BLYNK_WRITE(V4){
  kiek = param.asInt();
}

//nustatytam laikui atejus pasukamas variklis
BLYNK_WRITE(V5){
  long TimeSec = param[0].asLong();
  int val = hour();
  int minut = minute();
  long laikSec = val*3600+minut*60;
  if(TimeSec==laikSec){
    for(int x; x<=kiek; x++){
      variklis(300, 100);
  }
 }
}

//paspaudus mygtuka pasukamas variklis
BLYNK_WRITE(V0){
  if(param.asInt() == 1){
    variklis(300, 100);
  }
}


void loop(){
  oled();
  Blynk.run();
  timer.run(); 
}

//laikrodis
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
void oled()
{
  display.setTextSize(2); //nustatomas teksto dydis
  display.setCursor(0,0); // nustatomas pradzios taskas
  delay(100);
  display.print(proc); //isvedimas i oled
  display.print(" %"); 
  display.display();
  delay(100);
  
  display.clearDisplay(); // isvalomas oled
  }
  
int ultragarsas()
{
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
 
  t = pulseIn(echoPin, HIGH); // laiko paskaiciavimas uztrukto garsui atkeliauti
  ats = t*0.0344/2; //atstumo paskaiciavimas pagal sugaista laika

//skaiciavimas procentais 
float x =(((ats+2.5)*100)/15)-50;
  
  if(ats <= 5){
    proc = 100;
  } else if(ats>=20){
    proc = 0;
  } else{
    proc = 100 - x;
  }
  return proc;
}


//Variklio pasukimo funkcija
void variklis(int stepsLeft, int stepsRight){
  digitalWrite(sleepPin, HIGH);
  delay(10);
//Pasukamas i viena puse
  digitalWrite(dirPin, LOW);
  for(int x = 0; x < stepsLeft; x++){
    digitalWrite(stepPin, HIGH);
    delayMicroseconds(2000);
    digitalWrite(stepPin, LOW);
    delayMicroseconds(2000);
  }
  
//Pasukamas i kita puse 
  digitalWrite(dirPin, HIGH);
  for(int x = 0; x < stepsRight; x++){
    digitalWrite(stepPin, HIGH);
    delayMicroseconds(1000);
    digitalWrite(stepPin, LOW);
    delayMicroseconds(1000);
  }

   digitalWrite(dirPin, LOW);
  for(int x = 0; x < stepsLeft; x++){
    digitalWrite(stepPin, HIGH);
    delayMicroseconds(2000);
    digitalWrite(stepPin, LOW);
    delayMicroseconds(2000);
  }
  
   digitalWrite(dirPin, HIGH);
  for(int x = 0; x < stepsRight; x++){
    digitalWrite(stepPin, HIGH);
    delayMicroseconds(1000);
    digitalWrite(stepPin, LOW);
    delayMicroseconds(1000);
  }
  digitalWrite(sleepPin, LOW);
}
