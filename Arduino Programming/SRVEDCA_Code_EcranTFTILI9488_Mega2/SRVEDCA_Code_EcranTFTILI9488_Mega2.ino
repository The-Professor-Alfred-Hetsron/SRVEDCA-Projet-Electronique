/**** "Système de Reconnaissance Vocale et d'Empreinte Digitale pour le Controle d'Acces" ****/
#include <string.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>
#include <WiFi.h>

/*
 * Connections
  Second ESP32 (Serial1)          Mega (Serial3)
  green   9(SD3) : TX1     ---->  19: RX3
  blue   10(SD2) : RX1     ---->  18: TX3

  Second ESP32 (Serial2)      First ESP32
  blue    17 : TX2     ---->  12: RX
  green   16 : RX2     ---->  13: TX
*/

const char* ssid = "Hverizon";
const char* password = "pointsv$1234";

//Adresse du serveur
String serverName = "http://192.168.43.38:8080/";

int inputASCI; // used to get commands from the Arduino Mega character per character
char megaCmdBuffer[4]; // used to store commands from the Arduino Mega
uint8_t cmdIndex = 0; // used to get the length of the commandas we read from the serial0
bool endOfReadFromMega = false; // indicate when we read fully the command from the Arduino Mega

  
void setup() {
  // put your setup code here, to run once:
  
  // Initialise the Serial Monitor Communication
  Serial.begin(9600);

  // Initialise the Arduino Mega Communication
  Serial1.begin(9600);

  // Initialise the First Esp32 Communication
  Serial2.begin(9600);

  // Connect the second esp32 to the Wifi
  WiFi.begin(ssid, password);

  while(WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to WiFi network with IP Address: ");
  Serial.println(WiFi.localIP());
}

void loop() {
  if(WiFi.status()== WL_CONNECTED){
    Serial.println("WiFi Connected");
    checkInput();

    if(megaCmdBuffer[0] == 'E' && megaCmdBuffer[1] == 'A' && megaCmdBuffer[2] == 'S'){
      /*If the Enrol add Student command is sent from the Arduino Mega*/
      addStudent();
    }
    if(megaCmdBuffer[0] == 'A' && megaCmdBuffer[1] == 'A' && megaCmdBuffer[2] == 'P'){
      /*If the Authenticate add Presence command is sent from the Arduino Mega*/
      addPresence();
    }
  }else{
    Serial.println("WiFi Disconnected");
    WiFi.begin(ssid, password);
  }
}

void checkInput(){
  // Principle: Check if a command is received from the Esp32
  cmdIndex = 0;
  endOfReadFromMega = false;
  endOfVREnrol = false;
  while(1){
    delay(500);
    if(Serial1.available()){
      inputASCI = Serial1.read();
      if(inputASCI > 0){ // Mega = $__* where __ = command from Mega
        if((char)inputASCI == '$'){ // Esp gets = $
          while(!endOfReadFromMega){
            delay(500);
            if(Serial1.available()){
              inputASCI = Serial1.read();
              if((char)inputASCI != '*'){ // Esp gets = ###...
                megaCmdBuffer[cmdIndex] = (char)inputASCI;
                cmdIndex++;
              }else{ // Esp gets = *
                endOfReadFromMega = true;
                cmdIndex = 0; // Initialises the counter for the next read
              }
            }
          }
          return;
        }
      }
    }
  }
}

void checkInput2(){
  // Principle: Check if a command is received from the Esp32
  cmdIndex = 0;
  endOfReadFromMega = false;
  endOfVREnrol = false;
  while(1){
    delay(500);
    if(Serial2.available()){
      inputASCI = Serial2.read();
      if(inputASCI > 0){ // Mega = $__* where __ = command from First ESP32
        if((char)inputASCI == '$'){ // Esp gets = $
          while(!endOfReadFromMega){
            delay(500);
            if(Serial2.available()){
              inputASCI = Serial2.read();
              if((char)inputASCI != '*'){ // Esp gets = ###...
                megaCmdBuffer[cmdIndex] = (char)inputASCI;
                cmdIndex++;
              }else{ // Esp gets = *
                endOfReadFromMega = true;
                cmdIndex = 0; // Initialises the counter for the next read
              }
            }
          }
          return;
        }
      }
    }
  }
}


void addStudent(){ //Ajoute un etudiant en base de données
  String fingerTemplate = ""; // the real fingerprint template
  String enrolDateTime = ""; // e.g 2023-01-06 15:40:25
  String matricule = "";
  String classe = "";
  String studentName = "";
  
  while(1){
    checkInput();
    if(megaCmdBuffer[0] == 'A' && megaCmdBuffer[1] == 'B' && megaCmdBuffer[2] == '1'){
      checkInput();
      if(megaCmdBuffer[0] == 'A' && megaCmdBuffer[1] == 'B' && megaCmdBuffer[2] == '2'){
        checkInput();
        if(megaCmdBuffer[0] == 'A' && megaCmdBuffer[1] == 'B' && megaCmdBuffer[2] == '3'){

          int inputArray, arrayCount = 0;
          bool isAllInfo = false;

          while(arrayCount < 2){
            isAllInfo = false;
            if(Serial1.available()){
              inputArray = Serial1.read();
              if((char)inputArray == '$'){ // If we have the start command
                while(!isAllInfo){
                  inputArray = Serial1.read();
                  if((char)inputArray != '*'){
                    if(arrayCount == 0){ // If it is fingerprint template
                      fingerTemplate = fingerTemplate + (char)inputArray;
                    }else if(arrayCount == 1){ // If it is auth datetime
                      enrolDateTime = enrolDateTime + (char)inputArray;
                    }
                  }else{ // If we encounter the terminating string
                    arrayCount++;
                    isAllInfo = true;
                  }
                }
              }
            }
          }
        }
      }
    }

    checkInput2();
    if(megaCmdBuffer[0] == '1' && megaCmdBuffer[1] == 'T' && megaCmdBuffer[2] == '2'){
      int inputArray, arrayCount = 0;
      bool isAllInfo = false;

      while(arrayCount < 3){
        isAllInfo = false;
        if(Serial2.available()){
          inputArray = Serial2.read();
          if((char)inputArray == '$'){ // If we have the start command
            while(!isAllInfo){
              inputArray = Serial2.read();
              if((char)inputArray != '*'){
                if(arrayCount == 0){ // If it is matricule
                  matricule = matricule + (char)inputArray;
                }
                else if(arrayCount == 1){ // If it is Class
                  classe = classe + (char)inputArray;
                }
                else if(arrayCount == 2){ // If it is Name
                  studentName = studentName + (char)inputArray;
                }
              }else{ // If we encounter the terminating string
                arrayCount++;
                isAllInfo = true;
              }
            }
          }
        }
      }
      // Envoyer les informations du nouveau etudiant au serveur
      HTTPClient http;
      // Ajoutons un etudiant
      String serverPath = serverName + "api/etudiant/create";
      http.begin(serverPath.c_str());
      http.addHeader("Content-Type", "application/json");

      const size_t CAPACITY = JSON_OBJECT_SIZE(4);
      StaticJsonDocument<CAPACITY> requestObject;
      Serial.print("Name: ");Serial.println(studentName);
      requestObject["nom"] = studentName;
      Serial.print("Matricule: ");Serial.println(matricule);
      requestObject["matricule"] = matricule;
      Serial.print("Class: ");Serial.println(classe);
      requestObject["classe"] = classe;
      Serial.print("Fingerprint: ");Serial.println(fingerTemplate);
      requestObject["empreinte"] = fingerTemplate;

      String requestJson;
      serializeJson(requestObject, requestJson);
      int httpResponseCode = http.POST(requestJson);

      if (httpResponseCode>0) {
        Serial2.write("$ESS*");
        Serial.write("HTTP Response code: ");
        Serial.write(httpResponseCode);
        String payload = http.getString();
        Serial.write(payload);
      }
      else {
        Serial2.write("$ESE*");
        Serial.write("Error code: ");
        Serial.write(httpResponseCode);
      }
      // Free resources
      http.end();
      //Fin de l'ajout de l'etudiant
      return;
    }
  }
}


void addPresence(){ //Ajoute une présence en base de données
  String matricule = "";
  int inputArray;
  bool isAllInfo = false;

  while(1){
    if(Serial1.available()){
      inputArray = Serial1.read();
      if((char)inputArray == '$'){ // If we have the start command
        while(!isAllInfo){
          inputArray = Serial1.read();
          if((char)inputArray != '*'){
            matricule = matricule + (char)inputArray;
          }else{ // If we encounter the terminating string
            isAllInfo = true;
          }
        }
      }
    }
    break;
  }

  while(1){
    checkInput2();
    if(megaCmdBuffer[0] == '1' && megaCmdBuffer[1] == 'T' && megaCmdBuffer[2] == '2'){
      
      // Envoyer les informations du nouveau etudiant au serveur
      HTTPClient http;
      // Ajoutons une presence
      String serverPath = serverName + "api/presence/saveByMatricule";
      http.begin(serverPath.c_str());
      http.addHeader("Content-Type", "application/json");
    
      const size_t CAPACITY = JSON_OBJECT_SIZE(1);
      StaticJsonDocument<CAPACITY> requestObject;
      Serial.print("Matricule: ");Serial.println(matricule);
      requestObject["matricule"] = matricule;
    
      String requestJson;
      serializeJson(requestObject, requestJson);
      int httpResponseCode = http.POST(requestJson);
    
      if(httpResponseCode>0) {
        Serial2.write("$APS*");
        Serial.write("HTTP Response code: ");
        Serial.write(httpResponseCode);
        String payload = http.getString();
        Serial.write(payload);
      }
      else{
        Serial2.write("$APE*");
        Serial.write("Error code: ");
        Serial.write(httpResponseCode);
      }
      // Free resources
      http.end();
      //Fin de l'ajout de la presence
      return;
   }
 }
}
