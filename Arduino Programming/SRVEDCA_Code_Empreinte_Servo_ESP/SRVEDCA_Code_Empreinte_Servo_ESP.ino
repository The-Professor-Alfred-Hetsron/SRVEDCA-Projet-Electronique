/**** Library import Here ****/
#include  <EEPROM.h> // EEPROM library for storing data
#include <SoftwareSerial.h>
#include <Adafruit_Fingerprint.h>
#include "VoiceRecognitionV3.h"
#include<Servo.h>
#include <Wire.h>
#include <ThreeWire.h>  
#include <RtcDS1302.h>

/**** Pins declarations here ****/
#define ledGreen 22
#define ledRed 23
#define ledYellow 24

#define buzzer 25

#define btnGreen 26 // Enrollement button
#define btnRed 27 // Clear fingerprint button 
#define btnYellow 28 // Authenticate button
#define btnBlue 29 // All record button

#define pir 30
#define echo 31
#define trigger 32

Servo Myservo1; // Pin 33 purple
Servo Myservo2; // Pin 34 blue
Servo Myservo3; // Pin 35 green
Servo Myservo4; // Pin 36 yellow // Servo for the door lock

ThreeWire myWire(38,37,39); // DAT, CLK, RST
RtcDS1302<ThreeWire> Rtc(myWire);

#define mySerial Serial1 /**** Arduino Mega 2560 pin 18(TX1) and 19(RX1) ****/
Adafruit_Fingerprint finger = Adafruit_Fingerprint(&mySerial);

/**        
 * Connection
 * Arduino             VoiceRecognitionModule
 * 10:RX (green)   ------->     TX
 * 11:TX (blue)    ------->     RX
 * 
 * Serial2 == Esp32 primary
 * Serial3 == Esp32 secondary
 */
VR myVR(10,11);

/***************************************************************************/
/** declare print functions */
void printSignature(uint8_t *buf, int len);
void printLoad(uint8_t *buf, uint8_t len);
void printSigTrain(uint8_t *buf, uint8_t len);

/***************************************************************************/
// command analyze part
#define CMD_BUF_LEN      64+1
#define CMD_NUM     4
typedef int (*cmd_function_t)(int, int);
uint8_t cmd[CMD_BUF_LEN];
uint8_t cmd_cnt;
uint8_t *paraAddr;
int receiveCMD();
int checkCMD(int len);
int checkParaNum(int len);
int findPara(int len, int paraNum, uint8_t **addr);
int compareCMD(uint8_t *para1 , uint8_t *para2, int len);

int cmdSigTrain(int len, int paraNum);
int cmdLoad(int len, int paraNum);
int cmdClear(int len, int paraNum);
int cmdGetSig(int len, int paraNum);

/** cmdList, cmdLen, cmdFunction has correspondence */
const char cmdList[CMD_NUM][10] = {  // command list table
  {
    "sigtrain"  }
  ,
  {
    "load"  }
  , 
  {
    "clear"  }
  ,
  {
    "getsig"  }
};
const char cmdLen[CMD_NUM]= {    // command length
  8,  //  {"sigtrain"},
  4,  //  {"load"}, 
  5,  //  {"clear"},
  6,  //  {"getsig"}
};
cmd_function_t cmdFunction[CMD_NUM]={      // command handle fuction(function pointer table)
  cmdSigTrain,
  cmdLoad,
  cmdClear,
  cmdGetSig
};

/***************************************************************************/
/** temprory data */
uint8_t buf[255];
uint8_t buf2[64];
uint8_t records[7]; // save record



/**** Variables ****/
int enrolID;
int authID;
long time = 0;
long distance = 0;
float maxAuthDuration = 5000;
//String stringID;

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
  
  // initialise Serial Monitor Communication
  Serial.begin(9600);
  
  // Initialise First ESP32 Serial port communication
  Serial2.begin(9600);

  // Initialise Second ESP32 Serial port communication
  Serial3.begin(9600);

  // intialise the fingerprint baud rate communication
  finger.begin(57600);

  // initialise the voice recognition module baud rate communication
  myVR.begin(9600);
  
  cmd_cnt = 0; // intialise the count for the voice input command from Serial
  
  // Initialise the RTC
  Rtc.Begin();

  RtcDateTime currentTime = RtcDateTime(__DATE__ , __TIME__);
  Rtc.SetDateTime(currentTime);
  
  // setup pin modes
  pinMode(ledGreen, OUTPUT);
  pinMode(ledRed, OUTPUT);
  pinMode(ledYellow, OUTPUT);

  pinMode(buzzer, OUTPUT);

  pinMode(btnGreen, INPUT_PULLUP);
  pinMode(btnRed, INPUT_PULLUP);
  pinMode(btnYellow, INPUT_PULLUP);
  pinMode(btnBlue, INPUT_PULLUP);

  pinMode(pir, INPUT);
  pinMode(echo, INPUT);
  pinMode(trigger, OUTPUT);

  Myservo1.attach(33);
  Myservo2.attach(34);
  Myservo3.attach(35);
  Myservo4.attach(36);
  
  Myservo1.write(120);
  Myservo2.write(120);
  Myservo3.write(120);
  Myservo4.write(180); // Serrure Fermé

  digitalWrite(buzzer, LOW);
  
  Serial.println(" SRVEDCA Attendance System ");
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
  Serial.println("Waiting for commands from ESP32");
}

int inputASCI; // used to get commands from the ESP32 character per character
char esp32CmdBuffer[3]; // used to store commands from the ESP32
uint8_t numberIndex = 0; // used to get the length of the commandas we read from the serial2
bool endOfReadFromEsp = false; // indicate when we read fully the command from the esp32

void loop() {  
  checkKeys();
  
  endOfReadFromEsp = false;
  delay(500);
  if(Serial2.available()){
    inputASCI = Serial2.read();
    if(inputASCI > 0){ // Esp32 = $E0* or $A0*
      if((char)inputASCI == '$'){ // Mega gets = $
        while(!endOfReadFromEsp){
          delay(500);
          if(Serial2.available()){
            inputASCI = Serial2.read();
            if((char)inputASCI != '*'){ // Mega gets = E0 or A0
              esp32CmdBuffer[numberIndex] = (char)inputASCI;
              numberIndex++;
            }else{ // Mega gets = *
              endOfReadFromEsp = true;
              numberIndex = 0; // Initialises the counter for the next read
            }
          }
        }
      }
    }
  }
  delay(1000);
}

void checkInput(){
  // Principle: Check if a command is received from the Esp32
  numberIndex = 0;
  endOfReadFromEsp = false;
  while(1){
    delay(500);
    if(Serial2.available()){
      inputASCI = Serial2.read();
      if(inputASCI > 0){ // Esp32 = $__* where __ = command from ESP32
        if((char)inputASCI == '$'){ // Mega gets = $
          while(!endOfReadFromEsp){
            delay(500);
            if(Serial2.available()){
              inputASCI = Serial2.read();
              if((char)inputASCI != '*'){ // Mega gets = __
                esp32CmdBuffer[numberIndex] = (char)inputASCI;
                numberIndex++;
              }else{ // Mega gets = *
                endOfReadFromEsp = true;
                numberIndex = 0; // Initialises the counter for the next read
              }
            }
          }
          return;
        }
      }
    }
  }
}

void checkKeys(){
  // Principle: Check for any key pressed and perform the action associated

  if(esp32CmdBuffer[0]== 'E' && esp32CmdBuffer[1]== '0')
  {
    esp32CmdBuffer[0] = '*';
    esp32CmdBuffer[1] = '*';
    delay(1000);
    enroll();
  }

  else if(esp32CmdBuffer[0]== 'A' && esp32CmdBuffer[1]== '0')
  {
    esp32CmdBuffer[0] = '*';
    esp32CmdBuffer[1] = '*';
    delay(1000);
    auth();
  }
  
  /*else if(digitalRead(btnBlue) == 0){
    /*Serial.println("**** Student Enrolled informations ****");
    finger.getTemplateCount();
    Serial.print("Number of enrolled Student: ");
    Serial.print(finger.templateCount);
    finger.getParameters();
    Serial.print(" enrolled out of ");
    Serial.println(finger.packet_len);
    return;
  }*/
}

int getNextToEnrolID(){
  // Principle: return the next free Id space in the register to be in enrolled

  for(int i = 1; i < EEPROM.length(); i++){
    enrolID = i;
    if(EEPROM.read(enrolID) == 0xff || EEPROM.read(enrolID) == 0){
      return i;
    }
    else if(i == EEPROM.length()-1){
      return -1;
    }
  }
}

void openDoor(){
    Myservo4.write(0); // Rotation a 0° sense trigonometrique // On desactive la serrure c-a-d deverouiller la porte
    delay(500);
    Myservo1.write(0); // Rotation de la porte1 a 0° sense inverse trigonometrique
    Myservo2.write(0); // Rotation de la porte2 a 0° sense inverse trigonometrique
    Myservo3.write(0); // Rotation de la porte3 a 0° sense inverse trigonometrique
    //Serial.println("0°");
    delay(6000); // Delai de 6 secondes pour l'ouverture de la porte
}

void closeDoor(){
    Myservo1.write(120); // Rotation de la porte1 a 120° sense trigonometrique
    Myservo2.write(120); // Rotation de la porte2 a 120° sense trigonometrique
    Myservo3.write(120); // Rotation de la porte3 a 120° sense trigonometrique
    //Serial.println("120°");
    delay(6000); // Delai de 6 secondes pour a fermeture de la porte
    Myservo4.write(180); // Rotation a 180° sense trigonometrique // On active la serrure c-a-d verouiller la porte
}

void printHex(int num, int precision) {
  char tmp[16];
  char format[128];

  sprintf(format, "%%.%dX", precision);

  sprintf(tmp, format, num);
  Serial3.print(tmp);
}

uint8_t downloadFingerprintTemplate(uint16_t id)
{
  //Serial.println("------------------------------------");
  //Serial.print("Attempting to load #"); Serial.println(id);
  uint8_t p = finger.loadModel(id);
  switch (p) {
    case FINGERPRINT_OK:
      Serial3.write("$AB1*");
      //Serial.print("Template "); Serial.print(id); Serial.println(" loaded");
      break;
    case FINGERPRINT_PACKETRECIEVEERR:
      //Serial.println("Communication error");
      return p;
    default:
      Serial.print("Unknown error "); Serial.println(p);
      return p;
  }

  // OK success!

  //Serial.print("Attempting to get #"); Serial.println(id);
  p = finger.getModel();
  switch (p) {
    case FINGERPRINT_OK:
       Serial3.write("$AB2*");
      //Serial.print("Template "); Serial.print(id); Serial.println(" transferring:");
      break;
    default:
      //Serial.print("Unknown error "); Serial.println(p);
      return p;
  }

  // one data packet is 267 bytes. in one data packet, 11 bytes are 'usesless' :D
  uint8_t bytesReceived[534]; // 2 data packets
  memset(bytesReceived, 0xff, 534);

  uint32_t starttime = millis();
  int i = 0;
  while (i < 534 && (millis() - starttime) < 20000) {
    if (mySerial.available()) {
      bytesReceived[i++] = mySerial.read();
    }
  }
  //Serial.print(i); Serial.println(" bytes read.");
  //Serial.println("Decoding packet...");
  Serial3.write("$AB3*");
  
  uint8_t fingerTemplate[512]; // the real template
  memset(fingerTemplate, 0xff, 512);

  // filtering only the data packets
  int uindx = 9, index = 0;
  memcpy(fingerTemplate + index, bytesReceived + uindx, 256);   // first 256 bytes
  uindx += 256;       // skip data
  uindx += 2;         // skip checksum
  uindx += 9;         // skip next header
  index += 256;       // advance pointer
  memcpy(fingerTemplate + index, bytesReceived + uindx, 256);   // second 256 bytes

  Serial3.write('$');
  for (int i = 0; i < 512; ++i) {
    //Serial.print("0x");
    printHex(fingerTemplate[i], 2);
    //Serial.print(", ");
  }
  Serial3.write('*');
  //Serial.println("\ndone.");

  return p;
}

void getPresenceAndDoorClose(int fingerID){ // Apply fingerprint authentification
  uint32_t startCheckTime = millis();
  int durationLimit = 10000;
  while(1){
    digitalWrite(trigger, LOW); //Low voltage on the trigger pin to produce a 
    //clear signal.
    delay(5); //….for 5 milliseconds.
    digitalWrite(trigger, HIGH); //Creating the soundwave.
    delay(10); //..for 10 milliseconds.
    digitalWrite(trigger, LOW);

    //time = pulseIn(echo, HIGH);
    distance = (time/2) / 29.1;
    //Serial.print("Distance = ");
    //Serial.println(distance);

    int movementStatus = digitalRead(pir);
    if(movementStatus==HIGH && distance < 10){ // If he is detected by the sensor and at less than 1m to the ultrasonic sensor
      //Serial.println("Presence Detected..........");
      //Serial.print("Distance = ");
      //Serial.println(distance);
      //downloadFingerprintTemplate(fingerID);
      Serial2.write("$AB1*");      
      delay(1000);
      Serial2.write("$AB2*");
      delay(1000);
      Serial2.write("$AB3*");
      ledEnrolSuccess();
      // Activate the RTC module and get the time;
      getDateTime();
      closeDoor();
      digitalWrite(ledGreen, HIGH);
      digitalWrite(buzzer, HIGH);
      delay(100);
      digitalWrite(buzzer, LOW);
      //Serial.print("ID: ");
      //Serial.println(fingerID);
 //     Serial.println("Please Wait....");
      delay(1000);
      //Serial.println("Attendance Registered");
      delay(1000);
      ledAuthSuccess();
      return;
    }
    int period =  millis() - startCheckTime;
    if(period > durationLimit){
      //Serial.println("End of tolerated duration for presence detection...");
      //Serial.println("Door Close");
      Serial2.write("$ABA*");
      closeDoor();
      return;
    }
  }
}


void getDateTime(){
  RtcDateTime now = Rtc.GetDateTime();

  Serial2.write('$');
  Serial2.write(now.Year());
  Serial2.write("-");
  Serial2.write(now.Month());
  Serial2.write("-");
  Serial2.write(now.Day());
  Serial2.write(' ');
  Serial2.write(now.Hour());
  Serial2.write(":");
  Serial2.write(now.Minute());
  Serial2.write(":");
  Serial2.write(now.Second());
  Serial2.write('*');

  Serial3.write('$');
  Serial3.write(now.Year());
  Serial3.write("-");
  Serial3.write(now.Month());
  Serial3.write("-");
  Serial3.write(now.Day());
  Serial3.write(' ');
  Serial3.write(now.Hour());
  Serial3.write(":");
  Serial3.write(now.Minute());
  Serial3.write(":");
  Serial3.write(now.Second());
  Serial3.write('*');
}

void enroll(){
  // Principle: Enrolls the student's fingerprint in the sensor
  
  int freeID = getNextToEnrolID(); // We get the next free register id to enrol
  
  if(freeID > 0){ // If it is present
    enrolID = freeID;
    //stringID = String(freeID); // We convert the ID to a string
    int resultFinger, resultVoice;
    
    checkInput();
    if(esp32CmdBuffer[0]== 'E' && esp32CmdBuffer[1]== 'F'){
      esp32CmdBuffer[0] = '*';
      esp32CmdBuffer[1] = '*';
      delay(1000);
      resultFinger = getFingerprintEnroll(freeID); // start fingerprint enrollement from the module
    }
    if(resultFinger == freeID){
      checkInput();
      if(esp32CmdBuffer[0]== 'E' && esp32CmdBuffer[1]== 'V'){
        esp32CmdBuffer[0] = '*';
        esp32CmdBuffer[1] = '*';
        delay(1000);
        Serial2.write('$'); 
        Serial2.write(enrolID);
        /*char* sendStringId = (char*) malloc(sizeof(char)*stringID.length()+1);
        stringID.toCharArray(sendStringId, stringID.length()+1);
        Serial2.write(sendStringId);
        free(sendStringId);*/
        Serial2.write('*');
        resultVoice = enrolVoiceCommand(enrolID); // start voice recognition module enrollement from module
      }
      if(resultVoice > 0){
        Serial3.write("$EAS*");
        downloadFingerprintTemplate(freeID);
        Serial2.write("$AB1*"); // We send the enrol command to the first Esp32
        Serial2.write("$AB2*"); // To enrol the time and display it on the TFT screen
        Serial2.write("$AB3*");
        getDateTime();
        return;
      }
    }
    return;
  }
  else{ // If there is no free ID in the EEPROM space
    Serial.println("Enrollement Register totally full...");
    Serial.println("You cannot enrol");
    ledEnrolFail();
    return;
  }
}

void auth(){
  // Principle: We get the incomming fingerprint image and check if it is enrolled or not
  delay(1000);

  int resultFinger, resultVoice;
  checkInput();
  
  if(esp32CmdBuffer[0]== 'A' && esp32CmdBuffer[1]== 'F'){
    esp32CmdBuffer[0] = '*';
    esp32CmdBuffer[1] = '*';
    resultFinger = getFingerprintID();

    if(resultFinger>0){ // If finger authetication is a success
      checkInput();
      if(esp32CmdBuffer[0]== 'A' && esp32CmdBuffer[1]== 'V'){
        esp32CmdBuffer[0] = '*';
        esp32CmdBuffer[1] = '*';
        if(resultFinger == authID){ // We verify if the fingerprint match is unique
          resultVoice = authVoiceCommand(resultFinger);
        }
      }
    }else{
      return;
    }

    if(resultVoice > 0){ // If voice authentication is a success
      if(resultVoice == resultFinger){
        openDoor();
        getPresenceAndDoorClose(resultFinger);
        return;
      }
    }else{ // If voice recognition had an error
      return;
    }
  }
}

/*void delet(){
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
}*/

int getFingerprintEnroll(int freeID){
  // Principle : The sensor stores the fingerprint images in the memory and the auth in the EEPROM is set to 1

  int p = -1;
  Serial.println("Place Finger....");
  Serial2.write("$EF1*");
  delay(2000);
  
  while (p != FINGERPRINT_OK)
  {
    p = finger.getImage();
    switch (p)
    {
      case FINGERPRINT_OK:
        Serial.println("Image taken");
        Serial2.write("$EF2*");
        break;
      case FINGERPRINT_NOFINGER:
        Serial.println("No Finger detected");
        Serial2.write("$EF3*");
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
      Serial2.write("$EF4*");
      break;
    case FINGERPRINT_IMAGEMESS:
      Serial2.write("$EF/*");
      Serial.println("Image too messy");
      return -1;
    case FINGERPRINT_PACKETRECIEVEERR:
      Serial2.write("$EF<*");
      Serial.println("Communication error");
      return -1;
    case FINGERPRINT_FEATUREFAIL:
      Serial2.write("$EF|*");
      Serial.println("Could not find fingerprint features");
      return -1;
    case FINGERPRINT_INVALIDIMAGE:
      Serial2.write("$EF|*");
      Serial.println("Could not find fingerprint features");
      return -1;
    default:
      Serial2.write("$EFD*");
      Serial.println("Unknown error");
      return -1;
  }

  Serial.println("Remove finger");
  Serial2.write("$EF5*");
  delay(2000);
  
  p = 0;
  while (p != FINGERPRINT_NOFINGER) {
    p = finger.getImage();
  }
  
  p = -1;
  Serial.println("Place same finger again");
  Serial2.write("$EF6*");
  delay(2000);
  
  while (p != FINGERPRINT_OK) {
    p = finger.getImage();
    switch (p) {
      case FINGERPRINT_OK:
        Serial.println("Image taken");
        Serial2.write("$EF2*");
        break;
      case FINGERPRINT_NOFINGER:
        Serial.print(".");
        Serial2.write("$EF7*");
        delay(1000);
        break;
      case FINGERPRINT_PACKETRECIEVEERR:
        Serial.println("Communication error");
        break;
      case FINGERPRINT_IMAGEFAIL:
        Serial2.write("$EF>*");
        Serial.println("Imaging error");
        break;
      default:
        Serial.println("Unknown error");
        break;
    }
  }

  // OK success!

  p = finger.image2Tz(2);
  switch (p) {
    case FINGERPRINT_OK:
      Serial.println("Image converted");
      Serial2.write("$EF4*");
      break;
    case FINGERPRINT_IMAGEMESS:
      Serial2.write("$EF/*");
      Serial.println("Image too messy");
      return -1;
    case FINGERPRINT_PACKETRECIEVEERR:
      Serial2.write("$EF<*");
      Serial.println("Communication error");
      return -1;
    case FINGERPRINT_FEATUREFAIL:
      Serial2.write("$EF|*");
      Serial.println("Could not find fingerprint features");
      return -1;
    case FINGERPRINT_INVALIDIMAGE:
      Serial2.write("$EF|*");
      Serial.println("Could not find fingerprint features");
      return -1;
    default:
      Serial2.write("$EFD*");
      Serial.println("Unknown error");
      return -1;
  }

  // OK converted!
  Serial.print("Creating model for #"); Serial.println(enrolID);
  Serial2.write("$EF8*");
  p = finger.createModel();
  if (p == FINGERPRINT_OK) {
    Serial.println("Prints matched!");
    //Serial2.write("$EF9*");
  }
  else if (p == FINGERPRINT_PACKETRECIEVEERR) {
     Serial2.write("$EF<*");
    Serial.println("Communication error");
    return -1;
  }
  else if (p == FINGERPRINT_ENROLLMISMATCH) {
    Serial.println("Fingerprints did not match");
    Serial2.write("$EFA*");
    return -1;
  }
  else {
     Serial2.write("$EFD*");
    Serial.println("Unknown error");
    return -1;
  }

  Serial.print("ID "); Serial.println(freeID);
  p = finger.storeModel(freeID);
  if (p == FINGERPRINT_OK) {
    Serial.println("Stored!");
    Serial2.write("$EF9*");
    
    EEPROM.write(freeID, 1); // We indicate the enroll fingerprint by adding 1 as value so as to make it not empty
    return freeID;
  }
  else if (p == FINGERPRINT_PACKETRECIEVEERR) {
    Serial2.write("$EF<*");
    Serial.println("Communication error");
    return -1;
  }
  else if (p == FINGERPRINT_BADLOCATION) {
    Serial.println("Could not store in that location");
    Serial2.write("$EFB*");
    return -1;
  }
  else if (p == FINGERPRINT_FLASHERR) {
    Serial2.write("$EFC*");
    Serial.println("Error writing to flash");
    return -1;
  }
  else {
    Serial.println("Unknown error");
    Serial2.write("$EFD*");
    return -1;
  } 
}


int getFingerprintID() {
  float start_millis = millis();
  uint8_t p = 1;
  while(p != FINGERPRINT_OK && (millis() - start_millis) <= maxAuthDuration){
    Serial2.write("$AF1*");
    Serial.println("Place Finger....");
    delay(5000);
    p = finger.getImage();
    switch (p) {
      case FINGERPRINT_OK:
        Serial2.write("$AF2*");
        Serial.println("Image taken");
        break;
      case FINGERPRINT_NOFINGER:
        Serial2.write("$AF3*");
        Serial.println("No finger detected");
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
  
    // OK success!  
  }
  if(millis() - start_millis > 5000){
    Serial2.write("$AFB*");
    // Serial.println("Authentication Timeout");
    return -1;
  }
  
  p = finger.image2Tz();
  switch (p) {
    case FINGERPRINT_OK:
      Serial2.write("$AF4*");
      Serial.println("Image converted");
      break;
    case FINGERPRINT_IMAGEMESS:
      Serial2.write("$AF/*");
      Serial.println("Image too messy");
      return -1;
    case FINGERPRINT_PACKETRECIEVEERR:
      Serial2.write("$AF<*");
      Serial.println("Communication error");
      return -1;
    case FINGERPRINT_FEATUREFAIL:
      Serial2.write("$AF|*");
      Serial.println("Could not find fingerprint features");
      return -1;
    case FINGERPRINT_INVALIDIMAGE:
      Serial2.write("$AF|*");
      Serial.println("Could not find fingerprint features");
      return -1;
    default:
      Serial2.write("$AFD*");
      Serial.println("Unknown error");
      return -1;
  }

  // OK converted!
  p = finger.fingerSearch();
  if (p == FINGERPRINT_OK) {
    Serial2.write("$AF9*");
    Serial.println("Found a print match!");
  } else if (p == FINGERPRINT_PACKETRECIEVEERR) {
     Serial2.write("$AF<*");
    Serial.println("Communication error");
    return -1;
  } else if (p == FINGERPRINT_NOTFOUND) {
    Serial2.write("$AFA*");
    Serial.println("Finger Not Found");
    Serial.println("Try Later...");
    return -1;
  } else {
    Serial2.write("$AFD*");
    Serial.println("Unknown error");
    return -1;
  }

  // found a match!
  Serial2.write("$AF9*");
  
  Serial.print("Found ID #");
  Serial2.write("$");
  Serial2.write(finger.fingerID);
  Serial2.write("*");
  
  Serial.print(" with confidence of ");
  Serial2.write("$");
  Serial2.write(finger.confidence);
  Serial2.write("*");
  
  authID = finger.fingerID;
  int authValue = EEPROM.read(authID);
  authValue++;
  EEPROM.write(authID, authValue); // We increment the number of attendance
  Serial.print("Authenticated :");
  authValue--;
  Serial.print(authValue);
  //Serial.println(" times");
  return finger.fingerID;
}

/**
  @brief   Print authentication signature, if the character is invisible, 
           print hexible value instead.
  @param   buf     --> command length
           len     --> number of parameters
*/
void printAuthSignature(uint8_t *buf, int len)
{
  int i;
  Serial2.write('$');
  Serial3.write('$');
  for(i=0; i<len; i++){
    if(buf[i]>0x19 && buf[i]<0x7F){
      Serial2.write(buf[i]);
      Serial3.write(buf[i]);
    }
    else{
      Serial2.print("[");
      Serial2.print(buf[i], HEX);
      Serial2.print("]");

      Serial3.print("[");
      Serial3.print(buf[i], HEX);
      Serial3.print("]");
    }
  }
  Serial2.write('*');
  Serial3.write('*');
}


int authVoiceCommand(int fingerprintID){
  if(myVR.clear() == 0){ // We clear all possible records from recognizer
    Serial2.write("$AV1*");
  }else{
    Serial2.write("$AVA*");
    return -1;
  }
  /*uint8_t recordID = (fingerprintID);
  myVR.load(recordID);*/
  if(myVR.load(uint8_t (fingerprintID)) >= 0){ // We import(load) the command at the fingerprintID index to the recognizer
    Serial2.write("$AV2*");
  }else{
    Serial2.write("$AVB*");
    return -1;
  }
  float start_millis = millis();
  int ret;
  while(1){
    ret = myVR.recognize(buf2, 50);
    if(ret>0){ // If there is a command recognition
      if(buf2[1] >= 0 && buf2[1] < 255){ // If the command index is valid
         if(buf2[1] == fingerprintID){ // If the recognize command as index fingerprintID 
            if(buf2[3]>0){
              Serial2.write("$AV9*"); // recognition success
              printAuthSignature(buf2 + 4, buf2[3]);
            }
            else{
              Serial2.write("$AV8*");
            }
            return fingerprintID;
         }else{ // If the command doesn't match
           Serial2.write("$AVC*"); // No voice command at fingerprintID
           return -1;
         }
      }else{ // voice commandID out of bounds
        Serial2.write("$AVD*"); 
        return -1;
      }
    }else{ // If no command is spoken
      if((millis() - start_millis) <= maxAuthDuration){
        Serial2.write("$AV3*");
        delay(2000);
      }else{
        Serial2.write("$AVE*");
        return -1;
      }
    }
  }
}

int enrolVoiceCommand(int freeId){
  while(1){
    int len, paraNum, paraLen, i;
    
    /** receive Serial command */
    len = receiveCMD(); // total number of characters in the enterred command + 1 e.g 19  for sigtraine 0 ALFRED
    if(len>0){
      /** check if the received command is valid */
      if(!checkCMD(len)){  // Check if the command consist only of charcaters
  
        /** check parameter number of the received command  */
        paraNum = checkParaNum(len); // Number of words enterred in the monitor e.g 3 for sigtraine 0 ALFRED
  
        /** display the receved command back */
        //Serial.write(cmd, len);
  
        /** find the first parameter */
        paraLen = findPara(len, 1, &paraAddr); // find the length of the first parameter
  
        /** compare the received command with command in the list */
        for(i=0; i<CMD_NUM; i++){
          /** compare command length */
          if(paraLen == cmdLen[i]){
            /** compare command content */
            if( compareCMD(paraAddr, (uint8_t *)cmdList[i], paraLen) == 0 ){
              /** call command function */
              if(cmdFunction[i](len, paraNum) != 0){ // If the funtion had an error
                Serial2.write("$EVA*");
                //Serial.println(F("Command Format Error!"));
              }
              else{
                return 1; // We return a success number = "1"
              }
              break;
            }
          }
        }
  
        /** command is not supported*/
        if(i == CMD_NUM){
          Serial2.write("$EVA*");
          //Serial.println(F("Unkonwn command"));
          return -1;
        }
      }
      else{
        /** received command is invalid */
        Serial2.write("$EVB*");
        //Serial.println(F("Command format error"));
        return -1;
      }
    }
  }
}

/**
 * @brief   receive command from //Serial.
 * @param   NONE.
 * @retval  command length, if no command receive return -1.
 */
int receiveCMD()
{
  int ret;
  int len;
  unsigned long start_millis;
  start_millis = millis();
  while(1){
    if(Serial2.available()){
      ret = Serial2.read();
      if((char)ret == '$'){ //If we receive the start command character '$' from the esp32
        while(1){
          ret = Serial2.read(); // recieve the characters from esp32
          if(ret>0){ // if the character is valid
            start_millis = millis();
            if((char)ret != '*'){
              cmd[cmd_cnt] = ret; // We store the command
            }else{ // If we receive the stop command character '*' from the esp32
              len = cmd_cnt+1; // We calculate the command length
              cmd_cnt = 0; // We initialise the count
              return len; // We return the command length 
            }
            cmd_cnt++; // We increment the count if '*' is not encountered
            if(cmd_cnt == CMD_BUF_LEN){ // If we reach 65 as max count
              cmd_cnt = 0; //  we intialise the count
              return -1; // we return -1 as sign of no command
            }
          }
      
          if(millis() - start_millis > 100){ // If the duration exceeds 100ms
            cmd_cnt = 0; // We initialise the count
            return -1; // we return -1 as sign of no command
          }
        }
      }
    }
  }
}

/**
 * @brief   Check command format.
 * @param   len  -->  command length
 * @retval  0  -->  command is valid
 * -1  -->  command is invalid
 */
int checkCMD(int len)
{
  int i;
  for(i=0; i<len; i++){
    if(cmd[i] > 0x1F && cmd[i] < 0x7F){

    }
    else if(cmd[i] == '\t' || cmd[i] == ' ' || cmd[i] == '\r' || cmd[i] == '\n'){

    }
    else{
      return -1;
    }
  }
  return 0;
}

/**
 * @brief   Check the number of parameters in the command
 * @param   len  -->  command length
 * @retval  number of parameters
 */
int checkParaNum(int len)
{
  int cnt=0, i;
  for(i=0; i<len; ){
    if(cmd[i]!='\t' && cmd[i]!=' ' && cmd[i] != '\r' && cmd[i] != '\n'){
      cnt++;
      while(cmd[i] != '\t' && cmd[i] != ' ' && cmd[i] != '\r' && cmd[i] != '\n'){
        i++;
      }
    }
    i++;
  }
  return cnt;
}

/**
 * @brief   Find the specified parameter.
 * @param   len       -->  command length
 * paraIndex -->  parameter index
 * addr      -->  return value. position of the parameter
 * @retval  length of specified parameter
 */
int findPara(int len, int paraIndex, uint8_t **addr)
{
  int cnt=0, i, paraLen;
  uint8_t dt;
  for(i=0; i<len; ){
    dt = cmd[i];
    if(dt!='\t' && dt!=' '){
      cnt++;
      if(paraIndex == cnt){
        *addr = cmd+i;
        paraLen = 0;
        while(cmd[i] != '\t' && cmd[i] != ' ' && cmd[i] != '\r' && cmd[i] != '\n'){
          i++;
          paraLen++;
        }
        return paraLen;
      }
      else{
        while(cmd[i] != '\t' && cmd[i] != ' ' && cmd[i] != '\r' && cmd[i] != '\n'){
          i++;
        }
      }
    }
    else{
      i++;
    }
  }
  return -1;
}

/**
 * @brief   compare two commands, case insensitive.
 * @param   para1  -->  command buffer 1
 * para2  -->  command buffer 2
 * len    -->  buffer length
 * @retval  0  --> equal
 * -1  --> unequal
 */
int compareCMD(uint8_t *para1 , uint8_t *para2, int len)
{
  int i;
  uint8_t res;
  for(i=0; i<len; i++){
    res = para2[i] - para1[i];
    if(res != 0 && res != 0x20){
      res = para1[i] - para2[i];
      if(res != 0 && res != 0x20){
        return -1;
      }
    }
  }
  return 0;
}

/**
 * @brief   Handle "sigtrain" command
 * @param   len     --> command length
 * paraNum --> number of parameters
 * @retval  0 --> success
 * -1 --> Command format error
 */
int cmdSigTrain(int len, int paraNum)
{
  int ret, sig_len;
  uint8_t *lastAddr;
  if(paraNum < 2){
    return -1;
  }

  findPara(len, 2, &paraAddr);
  records[0] = atoi((char *)paraAddr);
  if(records[0] == 0 && *paraAddr != '0'){
    return -1;
  }

  findPara(len, 3, &paraAddr);
  sig_len = findPara(len, paraNum, &lastAddr);
  sig_len +=( (unsigned int)lastAddr - (unsigned int)paraAddr );

  ret = myVR.trainWithSignature(records[0], paraAddr, sig_len, buf);
  //  ret = myVR.trainWithSignature(records, paraNum-1);
  if(ret >= 0){
    printSigTrain(buf, ret);
  }
  else{
    Serial2.write("$EVC*");
    //Serial.println(F("Train with signature failed or timeout."));
  }

  return 0;
}

/**
 * @brief   Print "sigtrain" command return value.
 * @param   buf  -->  "sigtrain" command return value
 * buf[0]  -->  number of records which are trained successfully.
 * buf[1]  -->  record number
 * buf[2]  -->  record train status.
 * 00 --> Trained 
 * F0 --> Trained, signature truncate
 * FE --> Train Time Out
 * FF --> Value out of range"
 * buf[3] ~ buf[len-1] --> Signature.
 * len  -->  length of buf
 */
void printSigTrain(uint8_t *buf, uint8_t len)
{
  if(len == 0){
    //Serial2.write("$EV9*");
    Serial.println(F("Train With Signature Finish."));
    return;
  }
  else{
    Serial2.write("$EV8*"); // Will direct us to the success page
    //Serial.print(F("Success: "));
    //Serial.println(buf[0], DEC); // Will always show "Success: 1"
  }
  switch(buf[2]){
  case 0:
    Serial2.write("$EV9*");
    //Serial.println(F("Trained"));
    break;
  case 0xF0:
    Serial2.write("$EV9*");
    //Serial.println(F("Trained, signature truncate"));
    break;
  case 0xFE:
    Serial2.write("$EV2*");
    //Serial.println(F("Train Time Out"));
    break;
  case 0xFF:
    Serial2.write("$EV3*");
    //Serial.println(F("Value out of range"));
    break;
  default:
    Serial2.write("$EV4*");
    //Serial.print(F("Unknown status "));
    //Serial.println(buf[2], HEX);
    break;
  }
  delay(1000);
  //Serial.print(F("Record "));
  Serial2.write('$');
  Serial2.print(buf[1], DEC); // Gives the voiceID of the record
  Serial2.write('*');
  //Serial.print(F("\t"));

  delay(1000);
  //Serial.print(F("SIG: "));
  Serial2.write('$');
  Serial2.write(buf+3, len-3); // We send the voiceId and signature JUST for confirmation
  Serial2.write('*');
  //Serial.println();
}

/**
 * @brief   Handle "getsig" command
 * @param   len     --> command length
 * paraNum --> number of parameters
 * @retval  0 --> success
 * -1 --> Command format error
 */
int cmdGetSig(int len, int paraNum)
{
  int ret;
  if(paraNum != 2){
    return -1;
  }

  findPara(len, 2, &paraAddr);
  records[0] = atoi((char *)paraAddr);
  if(records[0] == 0 && *paraAddr != '0'){
    return -1;
  }

  ret = myVR.checkSignature(records[0], buf);
  
  if(ret == 0){
    //Serial.println(F("Signature isn't set."));
  }
  else if(ret > 0){
    //Serial.print(F("Signature:"));
    //printSignature(buf, ret);
    //Serial.println();
  }
  else{
    //Serial.println(F("Get sig error or timeout."));
  }

  return 0;
}

/**
 * @brief   Print signature, if the character is invisible, 
 * print hexible value instead.
 * @param   buf     --> command length
 * len     --> number of parameters
 */
void printSignature(uint8_t *buf, int len)
{
  int i;
  for(i=0; i<len; i++){
    if(buf[i]>0x19 && buf[i]<0x7F){
      //Serial.write(buf[i]);
    }
    else{
      //Serial.print(F("["));
      //Serial.print(buf[i], HEX);
      //Serial.print(F("]"));
    }
  }
}

/**
 * @brief   Handle "load" command
 * @param   len     --> command length
 * paraNum --> number of parameters
 * @retval  0 --> success
 * -1 --> Command format error
 */
int cmdLoad(int len, int paraNum)
{
  int i, ret;
  if(paraNum < 2 || paraNum > 8 ){
    return -1;
  }

  for(i=2; i<=paraNum; i++){
    findPara(len, i, &paraAddr);
    records[i-2] = atoi((char *)paraAddr);
    if(records[i-2] == 0 && *paraAddr != '0'){
      return -1;
    }
  }
  //  myVR.writehex(records, paraNum-1);
  ret = myVR.load(records, paraNum-1, buf);
  if(ret >= 0){
    //printLoad(buf, ret);
  }
  else{
    //Serial.println(F("Load failed or timeout."));
  }
  return 0;
}

/**
 * @brief   Print "load" command return value.
 * @param   buf  -->  "load" command return value
 * buf[0]    -->  number of records which are load successfully.
 * buf[2i+1]  -->  record number
 * buf[2i+2]  -->  record load status.
 * 00 --> Loaded 
 * FC --> Record already in recognizer
 * FD --> Recognizer full
 * FE --> Record untrained
 * FF --> Value out of range"
 * (i = 0 ~ (len-1)/2 )
 * len  -->  length of buf
 */
void printLoad(uint8_t *buf, uint8_t len)
{
  if(len == 0){
    //Serial.println(F("Load Successfully."));
    return;
  }
  else{
    //Serial.print(F("Load success: "));
    //Serial.println(buf[0], DEC);
  }
  for(int i=0; i<len-1; i += 2){
    //Serial.print(F("Record "));
    //Serial.print(buf[i+1], DEC);
    //Serial.print(F("\t"));
    switch(buf[i+2]){
    case 0:
      //Serial.println(F("Loaded"));
      break;
    case 0xFC:
      //Serial.println(F("Record already in recognizer"));
      break;
    case 0xFD:
      //Serial.println(F("Recognizer full"));
      break;
    case 0xFE:
      //Serial.println(F("Record untrained"));
      break;
    case 0xFF:
      //Serial.println(F("Value out of range"));
      break;
    default:
      //Serial.println(F("Unknown status"));
      break;
    }
  }
}

/**
 * @brief   Handle "clear" command
 * @param   len     --> command length
 * paraNum --> number of parameters
 * @retval  0 --> success
 * -1 --> Command format error
 */
int cmdClear(int len, int paraNum)
{
  if(paraNum != 1){
    return -1;
  }
  if(myVR.clear() == 0){
    //Serial.println(F("Recognizer cleared."));
  }
  else{
    //Serial.println(F("Clear recognizer failed or timeout."));
  }
  return 0;
}
