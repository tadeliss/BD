#include <Wire.h>
#include <Adafruit_SSD1306.h>
#include <Adafruit_GFX.h>



#define SCREEN_WIDTH 128 // OLED plotis pikseliais
#define SCREEN_HEIGHT 64 // OLED aukstis pikseliais


#define OLED_RESET     4 // reset
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

const int trigPin = 4;
const int echoPin = 5;
long duration;
int distance, proc;



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
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0,30);
     
}

void loop() {
display.setTextSize(2);
display.setCursor(0,30);
digitalWrite(trigPin, LOW);
delayMicroseconds(2);
digitalWrite(trigPin, HIGH);
delayMicroseconds(10);
digitalWrite(trigPin, LOW);
duration = pulseIn(echoPin, HIGH);
distance = duration*0.0344/2;
Serial.print("Distance: ");
Serial.println(distance); 

  if(distance <= 5) {
    proc = 100;
  } else if (distance>=20) {
    proc = 0;
  }

delay(100);
//display.print("Distance: ");
//display.print(distance);
display.print(proc);
display.print(" %");
display.display();
delay(500);
display.clearDisplay();
delay(500);
  

}
