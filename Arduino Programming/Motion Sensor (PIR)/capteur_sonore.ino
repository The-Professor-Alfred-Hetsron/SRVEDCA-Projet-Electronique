int ledPin =2;
int soundPin = 12;

void setup() {
  
  pinMode(ledPin, OUTPUT);
  pinMode(soundPin, INPUT);
  Serial.begin(9600);
 
}

void loop() {
  if(digitalRead(soundPin) == HIGH)
  {
     digitalWrite(ledPin,HIGH);
  }
  else
  {
    digitalWrite(ledPin,LOW);
  }
}
