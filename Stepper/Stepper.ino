#define BLYNK_PRINT Serial

#include <WiFi.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>
#include <Stepper.h>


const int dirPin = 16; //Krypties 
const int stepPin = 17; // Zingsnio
const int ledPin = 13;

//Blynk atpazinimo kodas
char auth[] = "hKeENDdhyYgTWMNzOvLqoNfpO2tLg5oR";

//Wifi duomenys
char ssid[] = "Tado apartamentai";
char pass[] = "tadas111";

void setup()
{
  Serial.begin(9600);
  pinMode(stepPin, OUTPUT);
  pinMode(dirPin, OUTPUT);
  pinMode(ledPin, OUTPUT);
  Blynk.begin(auth, ssid, pass);
}

//Pasukimas variklio
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

BLYNK_WRITE(V0)
{
  if(param.asInt() == 1) {
    variklis(500, 100);
  }
}

  void loop()
{
  Blynk.run(); 
}
