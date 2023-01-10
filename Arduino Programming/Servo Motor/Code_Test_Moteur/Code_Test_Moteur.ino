#include<Servo.h>

Servo Myservo;

void setup()
{
  Serial.begin(9600);
  Myservo.attach(3);
  Myservo.write(0);
}

void loop()
{
  Myservo.write(0);
  Serial.println("0°");
  delay(1000);
  Myservo.write(90);
  Serial.println("90°");
  delay(1000);
  Myservo.write(180);
  Serial.println("180°");
  delay(1000);
  Serial.println("****************");
}