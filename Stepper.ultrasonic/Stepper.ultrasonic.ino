#define BLYNK_PRINT Serial
#include <Wire.h>
#include <Adafruit_SSD1306.h>
#include <Adafruit_GFX.h>
#include <WiFi.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>

#define SCREEN_WIDTH 128 // OLED plotis pikseliais
#define SCREEN_HEIGHT 64 // OLED aukstis pikseliais
#define OLED_RESET     4 // reset
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

const int dirPin = 16; //Variklio krypties
const int stepPin = 17; //Variklio zingsnio
const int trigPin = 4; //Ultragarso trig 
const int echoPin = 5; //Ultragarso echo
long t; //garso signalo sugaistas laikas
int ats; //atstumas
int proc;
float x;

//Blynk atpazinimo kodas
char auth[] = "hKeENDdhyYgTWMNzOvLqoNfpO2tLg5oR";

//Wifi duomenys
char ssid[] = "Tado apartamentai";
char pass[] = "tadas111";

BlynkTimer timer; 
//-----------------------------------------------------------------------------

void setup()
{
  Serial.begin(9600);
  pinMode(stepPin, OUTPUT);
  pinMode(dirPin, OUTPUT);
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  Blynk.begin(auth, ssid, pass);
  
  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { // adresas 0x3C kur OLED
    Serial.println(F("SSD1306 allocation failed"));
    for(;;); // Don't proceed, loop forever
  }
  
  delay(1000);
  display.setFont();
  display.clearDisplay();
  display.setTextColor(WHITE);
  timer.setInterval(1000L, ultra);
}
void ultra()
{
  ultragarsas();
  Blynk.virtualWrite(V5, proc);
}


//tikrinama ar mygtuko aplikacijoje paspaudimas = true ir paleidziama variklio funkcija
BLYNK_WRITE(V0)
{
  if(param.asInt() == 1) {
    variklis(500, 100);
  }
}



//---------------------------------------------------
  void loop()
{
  //ultragarsas();
  //oled();
  Blynk.run();
  timer.run(); 
}
//---------------------------------------------------
void oled()
{
  display.setTextSize(2); //nustatomas teksto dydis
  display.setCursor(0,30); // nustatomas pradzios taskas
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
  x =(((ats+2.5)*100)/15)-50;
  
  if(ats <= 5) {
    proc = 100;
  } else if (ats>=20) {
    proc = 0;
  } else{
    proc = 100 - x;
  }
  return proc;
  }

//Variklio pasukimo funkcija
void variklis(int stepsLeft, int stepsRight)
{
//Pasukamas i viena puse
  digitalWrite(dirPin, LOW);
  for(int x = 0; x < stepsLeft; x++)
  {
    digitalWrite(stepPin, HIGH);
    delayMicroseconds(2000);
    digitalWrite(stepPin, LOW);
    delayMicroseconds(2000);
  }
  
//Pasukamas i kita puse 
  digitalWrite(dirPin, HIGH);
  for(int x = 0; x < stepsRight; x++)
  {
    digitalWrite(stepPin, HIGH);
    delayMicroseconds(1000);
    digitalWrite(stepPin, LOW);
    delayMicroseconds(1000);
  }
  }
