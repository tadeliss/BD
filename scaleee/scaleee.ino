#include "HX711.h" 
//float x;
#define DOUT  18
#define CLK  19

HX711 scale(DOUT, CLK);
 

float calibration_factor = -404650;  

void setup() {
 
  
  Serial.begin(9600);
  
  scale.set_scale();
  scale.tare(); 
 
  
}
 

void loop() {
delay (1000);
  scale.set_scale(calibration_factor); 
  Serial.println(scale.get_units(),3);

}
