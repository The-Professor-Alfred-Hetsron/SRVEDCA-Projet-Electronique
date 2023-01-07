/**** Library import Here ****/
#include  <EEPROM.h> // EEPROM library for storing data
#include <Adafruit_Fingerprint.h>

/**** Pins declarations here ****/
#define ledGreen 22
#define ledRed 23
#define ledYellow 24

#define buzzer 25

#define btnGreen 26 // Enrollement button
#define btnRed 27 // Clear fingerprint button 
#define btnYellow 28 // Authenticate button
#define btnBlue 29

#define mySerial Serial1 /**** Arduino Mega 2560 pin 18(TX1) and 19(RX1) ****/
Adafruit_Fingerprint finger = Adafruit_Fingerprint(&mySerial);

/**** Variables ****/
uint8_t id;

void ledEnrolSuccess(){
  digitalWrite(buzzer, LOW);
  digitalWrite(ledGreen, LOW);
  digitalWrite(ledRed, LOW);
  digitalWrite(ledYellow, LOW);
  for(int i = 1; i <=2;  i++){
    digitalWrite(ledGreen, HIGH);
    digitalWrite(buzzer, HIGH);
    delay(500);
    digitalWrite(buzzer, LOW);
    delay(500);
    digitalWrite(buzzer, HIGH);
    digitalWrite(ledRed, HIGH);
    delay(500);
    digitalWrite(buzzer, LOW);
    delay(500);
    digitalWrite(buzzer, HIGH);
    digitalWrite(ledYellow, HIGH);
    delay(500);
    digitalWrite(buzzer, LOW);
    digitalWrite(ledGreen, LOW);
    digitalWrite(ledRed, LOW);
    digitalWrite(ledYellow, LOW);
  }
  digitalWrite(ledGreen, HIGH);
  digitalWrite(buzzer, HIGH);
  delay(2000);
  digitalWrite(buzzer, LOW);
  digitalWrite(ledGreen, LOW);
}

void ledEnrolFail(){
  digitalWrite(ledGreen, LOW);
  digitalWrite(ledRed, LOW);
  digitalWrite(ledYellow, LOW);
  digitalWrite(buzzer, LOW);
  for(int i = 1; i <=2;  i++){
    digitalWrite(ledGreen, HIGH);
    digitalWrite(buzzer, HIGH);
    delay(500);
    digitalWrite(buzzer, LOW);
    delay(500);
    digitalWrite(buzzer, HIGH);
    digitalWrite(ledRed, HIGH);
    delay(500);
    digitalWrite(buzzer, LOW);
    delay(500);
    digitalWrite(buzzer, HIGH);
    digitalWrite(ledYellow, HIGH);
    delay(500);
    digitalWrite(buzzer, LOW);
    digitalWrite(ledGreen, LOW);
    digitalWrite(ledRed, LOW);
    digitalWrite(ledYellow, LOW);
  }
  digitalWrite(ledRed, HIGH);
  digitalWrite(buzzer, HIGH);
  delay(1000);
  digitalWrite(ledRed, LOW);
  digitalWrite(buzzer, LOW);
}

void ledAuthSuccess(){
  digitalWrite(ledGreen, LOW);
  digitalWrite(ledRed, LOW);
  digitalWrite(ledYellow, LOW);
  digitalWrite(buzzer, LOW);
  for(int i = 1; i <= 2; i++){
    digitalWrite(ledGreen, HIGH);
    digitalWrite(ledRed, HIGH);
    digitalWrite(ledYellow, HIGH);
    digitalWrite(buzzer, HIGH);
    delay(500);
    digitalWrite(buzzer, LOW);
    digitalWrite(ledGreen, LOW);
    digitalWrite(ledRed, LOW);
    digitalWrite(ledYellow, LOW);
    delay(500);
  }
  digitalWrite(ledGreen, HIGH);
  digitalWrite(buzzer, LOW);
  delay(500);
  digitalWrite(ledGreen, LOW);
  digitalWrite(buzzer, LOW);
  delay(500);
  digitalWrite(buzzer, HIGH);
  digitalWrite(ledRed, HIGH);
  delay(500);
  digitalWrite(ledRed, LOW);
  digitalWrite(buzzer, LOW);
  delay(500);
  digitalWrite(buzzer, HIGH);
  digitalWrite(ledYellow, HIGH);
  delay(500);
  digitalWrite(ledYellow, LOW);
  digitalWrite(buzzer, LOW);
  delay(500);
  digitalWrite(ledGreen, HIGH);
  digitalWrite(ledRed, HIGH);
  digitalWrite(ledYellow, HIGH);
  digitalWrite(buzzer, LOW);
  delay(500);
  digitalWrite(buzzer, LOW);
  digitalWrite(ledGreen, LOW);
  digitalWrite(ledRed, LOW);
  digitalWrite(ledYellow, LOW);
}

void setup() {
  delay(1000);
  
  // initialise Serial port
  Serial.begin(9600);

  // intialise the fingerprint baud rate
  finger.begin(57600);

  // setup pin modes
  pinMode(ledGreen, OUTPUT);
  pinMode(ledRed, OUTPUT);
  pinMode(ledYellow, OUTPUT);

  pinMode(buzzer, OUTPUT);

  pinMode(btnGreen, INPUT_PULLUP);
  pinMode(btnRed, INPUT_PULLUP);
  pinMode(btnYellow, INPUT_PULLUP);
  pinMode(btnBlue, INPUT_PULLUP);

  digitalWrite(buzzer, LOW);
  
  Serial.println(" Fingerprint Attendance System ");
  digitalWrite(ledGreen, HIGH);
  delay(500);
  digitalWrite(ledRed, HIGH);
  delay(500);
  digitalWrite(ledYellow, HIGH);
  
  digitalWrite(buzzer, HIGH);
  delay(1000);
  digitalWrite(buzzer, LOW);
  digitalWrite(buzzer, HIGH);
  delay(500);
  digitalWrite(buzzer, LOW);

  digitalWrite(ledGreen, LOW);
  delay(500);
  digitalWrite(ledRed, LOW);
  delay(500);
  digitalWrite(ledYellow, LOW);
  
  // Verifying fingerprint sensor connection
  Serial.println("Finding Module..");
  if (finger.verifyPassword()){
    Serial.println("Found fingerprint sensor!");
  }
  else
  {
    Serial.println("Did not find fingerprint sensor :(");
    while (1){ delay(1); }
  }
}

void loop() {
  Serial.println("Waiting for any command or finger");
  checkKeys();
  delay(1000);
}

void checkKeys(){
  // Principle: Check for any key pressed and perform the action associated

  if(digitalRead(btnGreen) == 0)
  {
    Serial.println("**** Student Enrollement ****");
    Serial.println("Press again enrollement button while placing your hands...");
    delay(1000);
    while(1){
      if(digitalRead(btnGreen) == 0){
        enroll();
        return;
      }
    }
  }

  else if(digitalRead(btnRed) == 0)
  {
    Serial.println("**** Delete Enrollement ****");
    Serial.println("Please Wait...");
    delay(1000);
    delet();
    return;
  }

  else if(digitalRead(btnYellow) == 0)
  {
    Serial.println("**** Student Presence Authentification ****");
    Serial.println("Press again Authentification button while placing your hands...");
    delay(1000);
    while(1){
      if(digitalRead(btnYellow) == 0){
        auth();
        return;
      }
    }
  }
}

int getNextToEnrolID(){
  // Principle: return the next free Id space in the register to be in enrolled

  for(int i = 1; i < EEPROM.length(); i++){
    id = i;
    if(EEPROM.read(id) == 0xff || EEPROM.read(id) == 0){
      return i;
    }
    else if(i == EEPROM.length()-1){
      return -1;
    }
  }
}

void enroll(){
  // Principle: Enrolls the student's fingerprint in the sensor
  
  int freeID = getNextToEnrolID(); // We get the next free register id to enrol
  Serial.print("freeID: ");
  Serial.println(freeID);
  if(freeID > 0){ // If it is present
    Serial.print("Your Finger ID:");
    Serial.println(freeID);
    getFingerprintEnroll(freeID);
    ledEnrolSuccess();
  }
  else{ // If there is no free space
    Serial.println("Enrollement Register totally full...");
    Serial.println("You cannot enrol");
    ledEnrolFail();
    return;
  }
}

void auth(){
  // Principle: We get the incomming fingerprint image and check if it is enrolled or not
  delay(1000);
  int result=getFingerprintIDez();
  for(int i = 1; i<50; i++){
    result=getFingerprintIDez();
    delay(500);
    if(result > 0){
      break;
    }
  }
  if(result>0)
  {
    digitalWrite(ledGreen, HIGH);
    digitalWrite(buzzer, HIGH);
    delay(100);
    digitalWrite(buzzer, LOW);
    Serial.print("ID: ");
    Serial.println(result);
    Serial.println("Please Wait....");
    delay(1000);
    Serial.println("Attendance Registered");
    delay(1000);
    ledAuthSuccess();
    return;
  }
}

void delet(){
  // Principle: the student chooses the id to delete and the system deletes the fingerprint and the presence
  int count=1;
  Serial.print("Enter Finger ID: ");

  while(1)
  {
    Serial.println(count);
    if(digitalRead(btnBlue) == 0)
    {
      count++;
      if(count > EEPROM.length()){count = 1;}
      delay(2000);
    }
    else if(digitalRead(btnYellow) == 0)
    {
      count--;
      if(count < 1){count = 1;}
      delay(2000);
    }
    else if(digitalRead(btnRed) == 0)
    {
      id=count;
      if(EEPROM.read(id)!= 0xff || EEPROM.read(id)> 0){
        deleteFingerprint(id);
        return;  
      }
      else{
        Serial.println("The chosen ID does not exist or is not enrolled");
        return;
      }
    }
  }
}

int getFingerprintIDez(){
  // Principle:  returns -1 if failed, otherwise returns ID #
  
  uint8_t p = finger.getImage(); // We takes(snap) the image from thumb by the sensor
  if (p != FINGERPRINT_OK){// if image is not taken
    Serial.println("Image is not taken");
    return -1;
  } 

  p = finger.image2Tz(); // We takes the 2Tz characteristics from sensor
  if (p != FINGERPRINT_OK){// if image 2Tz characteristics is not seen
    Serial.println("Image 2Tz characteristics is not found");
    return -1;
  }

  p = finger.fingerFastSearch(); // We search for the existence of the image in the sensor's memory
  if (p != FINGERPRINT_OK) // if the image does not exist in the sensor memory
  {
    Serial.println("Finger Not Found");
    Serial.println("Try Later...");
    delay(2000);
    return -1;
  }
  // found a match!
  Serial.print("Found ID #");
  Serial.println(finger.fingerID);
  Serial.print(" with confidence of ");
  Serial.println(finger.confidence);
  id = finger.fingerID;
  int authValue = EEPROM.read(id);
  authValue++;
  EEPROM.write(id, authValue); // We increment the number of attendance
  Serial.print("Authenticated :");
  authValue--;
  Serial.print(authValue);
  Serial.println(" times");
  return id;
}

uint8_t getFingerprintEnroll(int freeID){
  // Principle : The sensor stores the fingerprint images in the memory and the auth in the EEPROM is set to 1

  int p = -1;
  Serial.println("Place Finger....");
  delay(2000);
  
  while (p != FINGERPRINT_OK)
  {
    p = finger.getImage();
    switch (p)
    {
      case FINGERPRINT_OK:
        Serial.println("Image taken");
        break;
      case FINGERPRINT_NOFINGER:
        Serial.println("No Finger");
        break;
      case FINGERPRINT_PACKETRECIEVEERR:
        Serial.println("Communication error");
        break;
      case FINGERPRINT_IMAGEFAIL:
        Serial.println("Imaging error");
        break;
      default:
        Serial.println("Unknown error");
        break;
    }
  }

  // OK success!

  p = finger.image2Tz(1);
  switch (p) {
    case FINGERPRINT_OK:
      Serial.println("Image converted");
      break;
    case FINGERPRINT_IMAGEMESS:
      Serial.println("Image too messy");
      return p;
    case FINGERPRINT_PACKETRECIEVEERR:
      Serial.println("Communication error");
      return p;
    case FINGERPRINT_FEATUREFAIL:
      Serial.println("Could not find fingerprint features");
      return p;
    case FINGERPRINT_INVALIDIMAGE:
      Serial.println("Could not find fingerprint features");
      return p;
    default:
      Serial.println("Unknown error");
      return p;
  }

  Serial.println("Remove finger");
  delay(2000);
  
  p = 0;
  while (p != FINGERPRINT_NOFINGER) {
    p = finger.getImage();
  }
  
  p = -1;
  Serial.println("Place same finger again");
  
  while (p != FINGERPRINT_OK) {
    p = finger.getImage();
    switch (p) {
      case FINGERPRINT_OK:
        Serial.println("Image taken");
        break;
      case FINGERPRINT_NOFINGER:
        Serial.print(".");
        break;
      case FINGERPRINT_PACKETRECIEVEERR:
        Serial.println("Communication error");
        break;
      case FINGERPRINT_IMAGEFAIL:
        Serial.println("Imaging error");
        break;
      default:
        Serial.println("Unknown error");
        return;
    }
  }

  // OK success!

  p = finger.image2Tz(2);
  switch (p) {
    case FINGERPRINT_OK:
      Serial.println("Image converted");
      break;
    case FINGERPRINT_IMAGEMESS:
      Serial.println("Image too messy");
      return p;
    case FINGERPRINT_PACKETRECIEVEERR:
      Serial.println("Communication error");
      return p;
    case FINGERPRINT_FEATUREFAIL:
      Serial.println("Could not find fingerprint features");
      return p;
    case FINGERPRINT_INVALIDIMAGE:
      Serial.println("Could not find fingerprint features");
      return p;
    default:
      Serial.println("Unknown error");
      return p;
  }

  // OK converted!
  Serial.print("Creating model for #"); Serial.println(id);

  p = finger.createModel();
  if (p == FINGERPRINT_OK) {
    Serial.println("Prints matched!");
  }
  else if (p == FINGERPRINT_PACKETRECIEVEERR) {
    Serial.println("Communication error");
    return p;
  }
  else if (p == FINGERPRINT_ENROLLMISMATCH) {
    Serial.println("Fingerprints did not match");
    return p;
  }
  else {
    Serial.println("Unknown error");
    return p;
  }

  Serial.print("ID "); Serial.println(freeID);
  p = finger.storeModel(freeID);
  if (p == FINGERPRINT_OK) {
    Serial.println("Stored!");
    EEPROM.write(freeID, 1); // We indicate the enroll fingerprint by adding 1 as value so as to make it not empty
  }
  else if (p == FINGERPRINT_PACKETRECIEVEERR) {
    Serial.println("Communication error");
    return p;
  }
  else if (p == FINGERPRINT_BADLOCATION) {
    Serial.println("Could not store in that location");
    return p;
  }
  else if (p == FINGERPRINT_FLASHERR) {
    Serial.println("Error writing to flash");
    return p;
  }
  else {
    Serial.println("Unknown error");
    return p;
  } 
}


uint8_t deleteFingerprint(uint8_t delID){
  // Principle: Delete the fingerprint image at the specified id and clear the presence from the EEPROM
  
  uint8_t p = -1;
  Serial.print("Please wait");
  p = finger.deleteModel(delID);
  if (p == FINGERPRINT_OK)
  {
    Serial.println("Deleted!");
    EEPROM.write(delID, 0);
    ledEnrolSuccess();
    delay(1000);
  }else if (p == FINGERPRINT_PACKETRECIEVEERR) {
    Serial.println("Communication error");
  } else if (p == FINGERPRINT_BADLOCATION) {
    Serial.println("Could not delete in that location");
  } else if (p == FINGERPRINT_FLASHERR) {
    Serial.println("Error writing to flash");
  }
  else
  {
    Serial.print("Something Wrong");
    delay(2000);
    return p;
  }
}
