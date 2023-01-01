int ledPin =8;
int mouvPin = 7;

void setup() {
  pinMode(ledPin, OUTPUT);
  pinMode(mouvPin, INPUT);

  digitalWrite(ledPin, LOW);

}

void loop() {
  if(digitalRead(mouvPin) == HIGH)
  {
     digitalWrite(ledPin,HIGH);
  }
  else
  {
    digitalWrite(ledPin,LOW);
  }

}
