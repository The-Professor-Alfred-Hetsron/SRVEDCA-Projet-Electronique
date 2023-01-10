#include<Servo.h>

#define serrure 2
Servo Myservo1;
Servo Myservo2;

#define Pous 5
#define signalPous digitalRead(Pous)

void setup()
{
  Serial.begin(9600);
  Myservo1.attach(3);
  Myservo2.attach(4);
  Myservo1.write(0);
  Myservo2.write(0);
  pinMode(serrure, OUTPUT);
  pinMode(signalPous, INPUT);
}

void loop()
{
  if(signalPous == HIGH){ // Lorsqu'on recois un signale HIGH
    digitalWrite(motor, LOW); // On active la serrure c-a-d deverouiller la porte
    delay(500);
    Myservo1.write(85); // Rotation de la porte1 a 85° sense trigonometrique
    Myservo2.write(85); // Rotation de la porte2 a 85° sense trigonometrique
    Serial.println("85°");
    delay(5000) // Delai de 5 secondes pour l'ouverture de la porte
  }

  if(signalPous == LOW){ // Lorsqu'on recois un signale LOW
    Myservo1.write(0); // Rotation de la porte1 a 85° sense inverse trigonometrique
    Myservo2.write(0); // Rotation de la porte2 a 85° sense inverse trigonometrique
    Serial.println("0°");
    delay(5000); // Delai de 5 secondes pour a fermeture de la porte
    digitalWrite(motor, LOW); // On desactive la serrure c-a-d verouiller la porte
  }
  
}