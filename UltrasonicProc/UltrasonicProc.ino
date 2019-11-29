#include <Wire.h>
#include <Adafruit_SSD1306.h>
#include <Adafruit_GFX.h>



#define SCREEN_WIDTH 128 // OLED plotis pikseliais
#define SCREEN_HEIGHT 64 // OLED aukstis pikseliais


#define OLED_RESET     4 // reset
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

const int trigPin = 4;
const int echoPin = 5;
long t;
int distance, proc, x;



void setup() {
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);

  Serial.begin(115200);
  
  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { // adresas 0x3C kur OLED
    Serial.println(F("SSD1306 allocation failed"));
    for(;;); // Don't proceed, loop forever
  }
  
  delay(2000);
  display.setFont();
  display.clearDisplay();
  display.setTextColor(WHITE);
  
     
}

void loop() {
  display.setTextSize(2); //nustatomas teksto dydis
  display.setCursor(0,30); // nustatomas pradzios taskas
  
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  
  t = pulseIn(echoPin, HIGH); // laiko paskaiciavimas uztrukto garsui atkeliauti
  distance = t*0.0344/2; //atstumo paskaiciavimas pagal sugaista laika
  
  Serial.print("Distance: ");
  Serial.println(distance); 
  
//*************************************** 
//skaiciavimas procentais 
  x =(((distance+2.5)*100)/15)-50;
  
  if(distance <= 5) {
    proc = 100;
  } else if (distance>=20) {
    proc = 0;
  } else{
    proc = 100 - x;
  }
//***************************************  
  delay(100);

  display.print(proc); //isvedimas i oled
  display.print(" %"); //isvedimas % zenkliuko
  display.display();
  
  delay(100);
  display.clearDisplay(); // isvalomas oled
  

}
