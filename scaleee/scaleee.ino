#include "HX711.h" 
float x;
#define DOUT  18
#define CLK  19
const int  motorPin = 25;
HX711 scale(DOUT, CLK);
 

float calibration_factor = -425650;  

void setup() {
  pinMode(motorPin, OUTPUT);
  
  Serial.begin(9600);
  
  scale.set_scale();
  scale.tare(); 
 
  
}
 

void loop() {
 
  scale.set_scale(calibration_factor); 
  Serial.println(scale.get_units(),3);
  x= scale.get_units();
  if (x<0.1){
   digitalWrite(motorPin, HIGH);
   delay(4000);
   digitalWrite(motorPin,LOW);
  }
  delay(2000);
}
