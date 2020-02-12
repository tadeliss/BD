
const int dirPin = 16; //Krypties 
const int stepPin = 17; // Žingsnio
const int ledPin = 13;
const int touchPin = 4;
const int threshold = 20;
int touchValue;

void setup()
{
  pinMode(stepPin, OUTPUT);
  pinMode(dirPin, OUTPUT);
  pinMode(ledPin, OUTPUT);
}
void loop()
{
  touchValue = touchRead(touchPin);
  
  if (touchValue < threshold)
  {
    digitalWrite(ledPin, LOW);
    variklis(500,150);
    } 
  else
  {
    digitalWrite(ledPin, HIGH);
    }  
}

//Pasukimas variklio
void variklis(int stepsLeft, int stepsRight)
{
//Pasukamas i kaire puse (beriamas maistas)
  digitalWrite(dirPin, LOW);
  for(int x = 0; x < stepsLeft; x++)
  {
    digitalWrite(stepPin, HIGH);
    delayMicroseconds(2000);
    digitalWrite(stepPin, LOW);
    delayMicroseconds(2000);
  }

//Pasukamas atgal, kad neuzstrigtu maistas  
  digitalWrite(dirPin, HIGH);
  for(int x = 0; x < stepsRight; x++)
  {
    digitalWrite(stepPin, HIGH);
    delayMicroseconds(1000);
    digitalWrite(stepPin, LOW);
    delayMicroseconds(1000);
  }
  }
