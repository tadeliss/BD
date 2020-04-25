#include "HX711.h" 
const int motorPin = 25;
float x;
#define DOUT  16
#define CLK  17
 
HX711 scale(DOUT, CLK);
 

float calibration_factor = -425650;  

void setup() {
  pinMode(motorPin, OUTPUT);
  
  Serial.begin(9600);
  
  scale.set_scale();
  scale.tare(); 
 
  long zero_factor = scale.read_average(); 
    
Serial.println(zero_factor);
}
 

void loop() {
 
  scale.set_scale(calibration_factor); 
  Serial.println(scale.get_units(),3);
  x= scale.get_units();
  if (x<0.1){
   digitalWrite(motorPin, HIGH);
  }else {
   digitalWrite(motorPin,LOW);
   }
   
  delay(2000);
}
