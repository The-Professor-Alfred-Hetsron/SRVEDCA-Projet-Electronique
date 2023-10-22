//#include "FS.h"
#include <string.h>
#include <SPI.h>
#include <TFT_eSPI.h>      // Hardware-specific library
#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>
#include <SoftwareSerial.h>

SoftwareSerial arduinoMega(2, 4);
/*
 * Connections
  ESP32                     Mega (Serial2)
  green  4 : TX     ---->  17: RX2
  blue  RX0: RX     ---->  16: TX2
*/

SoftwareSerial secondEsp32(12, 13);
/*
 * Connections
  ESP32                     Second ESP32
  green  13 : TX     ---->  16: RX2
  blue   12 : RX     ---->  17: TX2
*/

#define TFT_PRIMARY4 0xD75F
#define TFT_PRIMARY1 0x1290
#define TFT_PRIMARY3 0x257E
#define TFT_CSUCCESS 0x3DEE

#include "pageindex.h"
#include "cardheader.h"

TFT_eSPI_Button homebtn;
TFT_eSPI_Button nextbtn;

TFT_eSPI tft = TFT_eSPI(); // Invoke custom library

const char* ssid = "Hverizon";
const char* password = "pointsv$1234";

//Adresse du serveur
String serverName = "http://192.168.43.38:8080/";

bool showIndex = true;

int pageIndex = 0;
char useCase;

// Keypad start position, key sizes and spacing
#define KEY1_X 80 // Centre of key
#define KEY1_Y 215
#define KEY1_W 70 // Width and height
#define KEY1_H 30
#define KEY1_SPACING_X 10 // X and Y gap
#define KEY1_SPACING_Y 10
#define KEY1_TEXTSIZE 1   // Font size multiplier

// Keypad start position, key sizes and spacing For keypad Name
#define KEY3_X 35 // Centre of key
#define KEY3_Y 215
#define KEY3_W 40 // Width and height
#define KEY3_H 30
#define KEY3_SPACING_X 5 // X and Y gap
#define KEY3_SPACING_Y 10
#define KEY3_TEXTSIZE 1   // Font size multiplier

// Using two fonts since numbers are nice when bold
#define LABEL1_FONT &FreeSansOblique12pt7b // Key label font 1
#define LABEL2_FONT &FreeSansBold12pt7b    // Key label font 2

// Numeric display box size and location
#define DISP1_X 110
#define DISP1_Y 130
#define DISP1_W 238
#define DISP1_H 50
#define DISP1_TSIZE 3
#define DISP1_TCOLOR TFT_BLACK

// Numeric display box size and location for keypad Name
#define DISP3_X 10
#define DISP3_Y 120
#define DISP3_W 460
#define DISP3_H 50
#define DISP3_TSIZE 2
#define DISP3_TCOLOR TFT_BLACK

// Number length, buffer for storing it and character index for Keypad Matricule
#define NUM1_LEN 6
// Number length, buffer for storing it and character index for Keypad Classe
#define NUM2_LEN 5
// Number length, buffer for storing it and character index for Keypad Name
#define NUM3_LEN 30

uint8_t numberIndex1 = 0; // For Matricule buffer
uint8_t numberIndex2 = 0; // For Class buffer
uint8_t numberIndex3 = 0; // For Name buffer

char numberBuffer1[NUM1_LEN + 1] = ""; // Matricule buffer
char numberBuffer2[NUM2_LEN + 1] = ""; // Class buffer
char numberBuffer3[NUM3_LEN + 1] = ""; // Name buffer

// We have a status line for messages
#define STATUS1_X 230 // Centred on this
#define STATUS1_Y 175

// Create 15 keys for the keypad Matricule
char keyLabel1[15][6] = {"SAuto", "Supr", "OK", "P", "p", "1", "2", "3", "4", "5", "6", "7", "8", "9", "0" };
uint16_t keyColor1[15] = {TFT_RED, TFT_DARKGREY, TFT_PRIMARY3,
                         TFT_PRIMARY1, TFT_PRIMARY1, TFT_PRIMARY1,
                         TFT_PRIMARY1, TFT_PRIMARY1, TFT_PRIMARY1,
                         TFT_PRIMARY1, TFT_PRIMARY1, TFT_PRIMARY1,
                         TFT_PRIMARY1, TFT_PRIMARY1, TFT_PRIMARY1
                        };
                        
// Create 15 keys for the keypad Class
char keyLabel2[15][6] = {"SAuto", "Supr", "OK", "1", "2", "3", "4", "5", "MSP", "GI", "GC", "GELE", "GTEL", "GM", "GIND" };
uint16_t keyColor2[15] = {TFT_RED, TFT_DARKGREY, TFT_PRIMARY3,
                         TFT_PRIMARY1, TFT_PRIMARY1, TFT_PRIMARY1,
                         TFT_PRIMARY1, TFT_PRIMARY1, TFT_PRIMARY1,
                         TFT_PRIMARY1, TFT_PRIMARY1, TFT_PRIMARY1,
                         TFT_PRIMARY1, TFT_PRIMARY1, TFT_PRIMARY1
                        };

// Create 30 keys for the keypad Name
char keyLabel3[30][6] = {"Q","W","E","R","T","Y","U","I","O","P",
                         "A","S","D","F","G","H","J","K","L","Z",
                         "X","C","V","B","N","M","__","SA","<--","OK"}; // Try to test space button with "  " instead of "__"
uint16_t keyColor3[30] = {TFT_PRIMARY1,TFT_PRIMARY1,TFT_PRIMARY1,TFT_PRIMARY1,TFT_PRIMARY1,TFT_PRIMARY1,TFT_PRIMARY1,TFT_PRIMARY1,TFT_PRIMARY1,TFT_PRIMARY1,
                          TFT_PRIMARY1,TFT_PRIMARY1,TFT_PRIMARY1,TFT_PRIMARY1,TFT_PRIMARY1,TFT_PRIMARY1,TFT_PRIMARY1,TFT_PRIMARY1,TFT_PRIMARY1,TFT_PRIMARY1,
                         TFT_PRIMARY1, TFT_PRIMARY1, TFT_PRIMARY1,TFT_PRIMARY1,TFT_PRIMARY1,TFT_PRIMARY1,TFT_PRIMARY1,TFT_DARKGREY,TFT_RED,TFT_PRIMARY3};
                        
// Invoke the TFT_eSPI button class and create all the button objects for keypad Matricule
TFT_eSPI_Button key1[15];
// Invoke the TFT_eSPI button class and create all the button objects for keypad Class
TFT_eSPI_Button key2[15];
// Invoke the TFT_eSPI button class and create all the button objects for keypad Name 
TFT_eSPI_Button key3[30];

char authFingerID[3];
char authVoiceSIG[6];

void setup() {
  // Initialise the TFT screen
  tft.init();

  // Initialise the TX pin 4 to Arduino Mega canal
  arduinoMega.begin(9600);

  // Initialise the RX0 pin canal which will communicate with the Serial Monitor
  Serial.begin(9600);

  // Initialise the second ESP32 communication
  secondEsp32.begin(9600);
  
  uint16_t calData[5] = { 351, 3563, 307, 3335, 7 };
  tft.setTouch(calData);
  tft.setRotation(1); // Rotates the screen to landscape
  tft.setSwapBytes(true);
  tft.fillScreen(TFT_PRIMARY4); // background-color property to theme color
  
  tft.pushImage(0,0, 480, 71, header); // Display the main header text;
  tft.fillRect(0, 71, 480, 46, TFT_WHITE);
  tft.setCursor(49, 107);
  tft.setTextSize(1);
  tft.setFreeFont(LABEL2_FONT);
  tft.setTextColor(TFT_BLACK,TFT_WHITE);
  tft.println(F("Verification de la connexion au WIFI"));
  tft.fillRect(0, 117, 480, 320, TFT_PRIMARY4); // Clear the unwanted additional header

  // Connect the first esp32 to the Wifi
  WiFi.begin(ssid, password);
  while(WiFi.status() != WL_CONNECTED) {
    delay(50);
    tft.setTextColor(TFT_RED,TFT_PRIMARY4);
    tft.setCursor(50, 211);
    tft.println(F("Le Module ESP32 est deconnecté du WIFI"));
    tft.setCursor(25, 270);
    tft.println(F("Veuillez Verifier la connexion WIFI"));
    WiFi.begin(ssid, password);
  }
  tft.fillRect(0, 117, 480, 320, TFT_PRIMARY4); // Clear the unwanted additional header
  delay(50);
  tft.setTextColor(TFT_CSUCCESS,TFT_PRIMARY4);
  tft.setCursor(50, 211);
  tft.println(F("Le Module ESP32 est connecté au WIFI"));
  tft.setCursor(25, 270);
  tft.setTextColor(TFT_BLACK,TFT_PRIMARY4);
  tft.print(F("ESP32 IP: ")); tft.println(WiFi.localIP());
  delay(5000);
}

int inputASCI; // used to get commands from the Arduino Mega character per character
char megaCmdBuffer[4]; // used to store commands from the Arduino Mega
uint8_t cmdIndex = 0; // used to get the length of the commandas we read from the serial0
bool endOfReadFromMega = false; // indicate when we read fully the command from the Arduino Mega

char enrolVRCmdBuffer[24]; // used to store enrol voice recognition commands from the Arduino Mega
bool endOfVREnrol = false; // indicate when the voice recognition commands are all sent

void loop() {
  if(WiFi.status()== WL_CONNECTED){
    if(showIndex){
      indexPageShow();
    }
    else if(useCase == 'e'){
      enrolPagesShow(pageIndex);
    }
    else if(useCase == 'a'){
      authPagesShow(pageIndex);
    }
  }else{
    tft.fillScreen(TFT_PRIMARY4); // background-color property to theme color
    tft.pushImage(0,0, 480, 71, header); // Display the main header text;
    tft.fillRect(0, 71, 480, 46, TFT_WHITE);
    tft.setCursor(49, 107);
    tft.setTextSize(1);
    tft.setFreeFont(LABEL2_FONT);
    tft.setTextColor(TFT_BLACK,TFT_WHITE);
    tft.println(F("Verification de la connexion au WIFI"));
    tft.fillRect(0, 117, 480, 320, TFT_PRIMARY4);
    delay(50);
    tft.setTextColor(TFT_RED,TFT_PRIMARY4);
    tft.setCursor(50, 211);
    tft.println(F("Le Module ESP32 est deconnecté du WIFI"));
    tft.setCursor(25, 270);
    tft.println(F("Veuillez Verifier la connexion WIFI"));
    WiFi.begin(ssid, password);
  }
}

void indexPageShow(){
  tft.pushImage(0,0, 480, 320, page_index); // Display the index page image on screen
  
  while(1){
    uint16_t x = 0, y = 0; // To store the touch coordinates
    // Pressed will be set true is there is a valid touch on the screen
    bool pressed = tft.getTouch(&x, &y);
    if (pressed) {
      if((x >= 11 && x <= 234) && (y >= 123 && y <= 268)){ // If Enrol button is pressed
        showIndex = false;
        arduinoMega.write("$E0*"); // We send the command to the Mega to trigger enroll fxns
        useCase = 'e';
        pageIndex = 0;
        return;
      }
  
      if((x >= 246 && x <= 469) && (y >= 123 && y <= 268)){ // If Authentification button is pressed
        showIndex = false;
        arduinoMega.write("$A0*"); // We send the command to the MEga to trigger auth fxns
        useCase = 'a';
        pageIndex = 0;
        return;
      }
    }
  }
}

void clearAllInputs(){
  // Matricule buffer and counter
  numberIndex1 = 0; // Reset index to 0
  for(int i = 0; i < NUM1_LEN ; i++){
    numberBuffer1[i] = 0;
  }

  // Name buffer and counter
  numberIndex3 = 0; // Reset index to 0
  for(int i = 0; i < NUM3_LEN ; i++){
    numberBuffer3[i] = 0;
  }

  // Class buffer and counter
  numberIndex2 = 0; // Reset index to 0
  for(int i = 0; i < NUM2_LEN ; i++){
    numberBuffer2[i] = 0;
  }
}

/************************************************************************************************************************************/
//************************************************************************************************************************************
/***************************************************** ENROLEMENT START *************************************************************/
//************************************************************************************************************************************
/************************************************************************************************************************************/


void enrolMatriculePage(){
  tft.fillScreen(TFT_PRIMARY4); // Clear the screen
  delay(50);
  tft.pushImage(0,0, 480, 71, header); // Display the main header text;
  tft.fillRect(0, 71, 480, 46, TFT_WHITE);
  tft.setCursor(49, 107);
  tft.setTextSize(1);
  tft.setFreeFont(LABEL2_FONT);
  tft.setTextColor(TFT_BLACK,TFT_WHITE);
  tft.println(F("Enregistrement de votre Matricule"));
  tft.fillRect(0, 117, 480, 320, TFT_PRIMARY4); // Clear the unwanted additional header
  
  // Draw keypad background
  tft.fillRect(0, 187, 480, 320, TFT_PRIMARY1);

  // Draw number display area and frame
  tft.fillRect(DISP1_X, DISP1_Y, DISP1_W, DISP1_H, TFT_WHITE);
  tft.drawRect(DISP1_X, DISP1_Y, DISP1_W, DISP1_H, TFT_PRIMARY3);

  // Draw keypad
  drawKeypad1();
  numberIndex1 = 0;
  while(1){
    //Serial.println("In Matricule Enrol");
    uint16_t t_x = 0, t_y = 0; // To store the touch coordinates

    // Pressed will be set true is there is a valid touch on the screen
    bool pressed = tft.getTouch(&t_x, &t_y);

    // / Check if any key coordinate boxes contain the touch coordinates
    for (uint8_t b = 0; b < 15; b++) {
      if (pressed && key1[b].contains(t_x, t_y)) {
        key1[b].press(true);  // tell the button it is pressed
      } else {
        key1[b].press(false);  // tell the button it is NOT pressed
      }
    }
  
    // Check if any key has changed state
    for (uint8_t b = 0; b < 15; b++) {
  
      if (b < 3) tft.setFreeFont(LABEL1_FONT);
      else tft.setFreeFont(LABEL2_FONT);
  
      if (key1[b].justReleased()) key1[b].drawButton();     // draw normal
  
      if (key1[b].justPressed()) {
        key1[b].drawButton(true);  // draw invert
  
        // if a numberpad button, append the relevant # to the numberBuffer
        if (b >= 3) {
          if (numberIndex1 < NUM1_LEN) {
            numberBuffer1[numberIndex1] = keyLabel1[b][0];
            numberIndex1++;
            numberBuffer1[numberIndex1] = 0; // zero terminate
          }
          status(""); // Clear the old status
        }
  
        // Del button, so delete last char
        if (b == 1) {
          numberBuffer1[numberIndex1] = 0;
          if (numberIndex1 > 0) {
            numberIndex1--;
            numberBuffer1[numberIndex1] = 0;//' ';
          }
          status(""); // Clear the old status
        }
  
        if (b == 2) { // If Ok button is pressed
          if(numberIndex1 > 0){
            status("Sent value to serial port");
            //Serial.println(numberBuffer1);
            showIndex = false;
            useCase = 'e';
            pageIndex = 1;
            return;
          }else{
            clearAllInputs();
            showIndex = true;
            useCase = ' ';
            pageIndex = 0;
            return;
          }
        }
        // we dont really check that the text field makes sense
        // just try to call
        if (b == 0) {
          status("Value cleared");
          numberIndex1 = 0; // Reset index to 0
          numberBuffer1[numberIndex1] = 0; // Place null in buffer
        }
  
        // Update the number display field
        tft.setTextDatum(TL_DATUM);        // Use top left corner as text coord datum
        tft.setFreeFont(&FreeSans18pt7b);  // Choose a nicefont that fits box
        tft.setTextColor(DISP1_TCOLOR);     // Set the font colour
  
        // Draw the string, the value returned is the width in pixels
        int xwidth = tft.drawString(numberBuffer1, DISP1_X + 4, DISP1_Y + 12);
  
        // Now cover up the rest of the line up by drawing a black rectangle.  No flicker this way
        // but it will not work with italic or oblique fonts due to character overlap.
        tft.fillRect(DISP1_X + 4 + xwidth, DISP1_Y + 1, DISP1_W - xwidth - 5, DISP1_H - 2, TFT_WHITE);
  
        delay(10); // UI debouncing
      }
    }
  }
}

void enrolNamePage(){
  /*tft.fillScreen(TFT_PRIMARY4); // Clear the screen
  delay(50);
  tft.pushImage(0,0, 480, 71, header); // Display the main header text;
  */
  tft.fillRect(0, 71, 480, 46, TFT_WHITE);
  tft.setCursor(77, 107);
  tft.setTextSize(1);
  tft.setFreeFont(LABEL2_FONT);
  tft.setTextColor(TFT_BLACK,TFT_WHITE);
  tft.println(F("Enregistrement de votre Nom"));
  tft.fillRect(0, 117, 480, 320, TFT_PRIMARY4); // Clear the unwanted additional header
  
  // Draw keypad background
  tft.fillRect(0, 187, 480, 320, TFT_PRIMARY1);

  // Draw number display area and frame
  tft.fillRect(DISP3_X, DISP3_Y, DISP3_W, DISP3_H, TFT_WHITE);
  tft.drawRect(DISP3_X, DISP3_Y, DISP3_W, DISP3_H, TFT_PRIMARY3);

  // Draw keypad
  drawKeypad3();
  numberIndex3 = 0;
  
  while(1){
    //Serial.println("In Name Enrol");
    uint16_t t_x = 0, t_y = 0; // To store the touch coordinates

    // Pressed will be set true is there is a valid touch on the screen
    bool pressed = tft.getTouch(&t_x, &t_y);
    
    // / Check if any key coordinate boxes contain the touch coordinates
    for (uint8_t b = 0; b < 30; b++) {
      if (pressed && key3[b].contains(t_x, t_y)) {
        key3[b].press(true);  // tell the button it is pressed
      } else {
        key3[b].press(false);  // tell the button it is NOT pressed
      }
    }
  
    // Check if any key has changed state
    for (uint8_t b = 0; b < 30; b++) {
  
      if (b >= 27) tft.setFreeFont(LABEL1_FONT);
      else tft.setFreeFont(LABEL2_FONT);
  
      if (key3[b].justReleased()) key3[b].drawButton();     // draw normal
  
      if (key3[b].justPressed()) {
        key3[b].drawButton(true);  // draw invert
  
        // if a numberpad button, append the relevant # to the numberBuffer
        if (b < 26) {
          if (numberIndex3 < NUM3_LEN) {
            numberBuffer3[numberIndex3] = keyLabel3[b][0];
            numberIndex3++;
            numberBuffer3[numberIndex3] = 0; // zero terminate
          }
          status(""); // Clear the old status
        }
        if(b == 26){
          numberBuffer3[numberIndex3] = ' ';
          numberIndex3++;
          numberBuffer3[numberIndex3] = 0; // zero terminate
        }
        // we dont really check that the text field makes sense
        // just try to call
        if (b == 27) {
          status("Value cleared");
          numberIndex3 = 0; // Reset index to 0
          numberBuffer3[numberIndex3] = 0; // Place null in buffer
        }
        
        // Del button, so delete last char
        if (b == 28) {
          numberBuffer3[numberIndex3] = 0;
          if (numberIndex3 > 0) {
            numberIndex3--;
            numberBuffer3[numberIndex3] = 0;//' ';
          }
          status(""); // Clear the old status
        }
  
        if (b == 29) {
          if(numberIndex3 > 0){
            status("Sent value to serial port");
            //Serial.println(numberBuffer3);
            showIndex = false;
            useCase = 'e';
            pageIndex = 2;
            return;
          }else{
            clearAllInputs();
            showIndex = true;
            useCase = ' ';
            pageIndex = 0;
            return;
          }
        }
  
        // Update the number display field
        tft.setTextDatum(TL_DATUM);        // Use top left corner as text coord datum
        tft.setFreeFont(&FreeSansBold12pt7b);  // Choose a nicefont that fits box
        tft.setTextColor(DISP3_TCOLOR);     // Set the font colour
  
        // Draw the string, the value returned is the width in pixels
        int xwidth = tft.drawString(numberBuffer3, DISP3_X + 4, DISP3_Y + 18);
  
        // Now cover up the rest of the line up by drawing a black rectangle.  No flicker this way
        // but it will not work with italic or oblique fonts due to character overlap.
        tft.fillRect(DISP3_X + 4 + xwidth, DISP3_Y + 1, DISP3_W - xwidth - 5, DISP3_H - 2, TFT_WHITE);
  
        delay(10); // UI debouncing
      }
    }
  }
}

void enrolClassPage(){
  /*tft.fillScreen(TFT_PRIMARY4); // Clear the screen
  delay(50);
  tft.pushImage(0,0, 480, 71, header); // Display the main header text;
  */
  tft.fillRect(0, 71, 480, 46, TFT_WHITE);
  tft.setCursor(65, 107);
  tft.setTextSize(1);
  tft.setFreeFont(LABEL2_FONT);
  tft.setTextColor(TFT_BLACK,TFT_WHITE);
  tft.println(F("Enregistrement de votre Classe"));
  tft.fillRect(0, 117, 480, 320, TFT_PRIMARY4); // Clear the unwanted additional header
  
  // Draw keypad background
  tft.fillRect(0, 187, 480, 320, TFT_PRIMARY1);

  // Draw number display area and frame
  tft.fillRect(DISP1_X, DISP1_Y, DISP1_W, DISP1_H, TFT_WHITE);
  tft.drawRect(DISP1_X, DISP1_Y, DISP1_W, DISP1_H, TFT_PRIMARY3);

  // Draw keypad
  drawKeypad2();
  numberIndex2 = 0;
  
  while(1){
    //Serial.println("In Matricule Enrol");
    uint16_t t_x = 0, t_y = 0; // To store the touch coordinates

    // Pressed will be set true is there is a valid touch on the screen
    bool pressed = tft.getTouch(&t_x, &t_y);
    
    // / Check if any key coordinate boxes contain the touch coordinates
    for (uint8_t b = 0; b < 15; b++) {
      if (pressed && key2[b].contains(t_x, t_y)) {
        key2[b].press(true);  // tell the button it is pressed
      } else {
        key2[b].press(false);  // tell the button it is NOT pressed
      }
    }
  
    // Check if any key has changed state
    for (uint8_t b = 0; b < 15; b++) {
  
      if (b < 3) tft.setFreeFont(LABEL1_FONT);
      else tft.setFreeFont(LABEL2_FONT);
  
      if (key2[b].justReleased()) key2[b].drawButton();     // draw normal
  
      if (key2[b].justPressed()) {
        key2[b].drawButton(true);  // draw invert
  
        // if a numberpad button, append the relevant # to the numberBuffer
        if (b >= 3) {
          if (numberIndex2 < NUM1_LEN) {
            if(b < 8){
              numberBuffer2[numberIndex2] = keyLabel2[b][0];
              //Serial.println(keyLabel2[b][0]);
              numberIndex2++;
            }
            else{
              switch(b){
                case 8:
                  numberBuffer2[numberIndex2] = 'M';
                  numberIndex2++;
                  numberBuffer2[numberIndex2] = 'S';
                  numberIndex2++;
                  numberBuffer2[numberIndex2] = 'P';
                  numberIndex2++;
                  break;
                case 9:
                  numberBuffer2[numberIndex2] = 'G';
                  numberIndex2++;
                  numberBuffer2[numberIndex2] = 'I';
                  numberIndex2++;
                  break;
                case 10:
                  numberBuffer2[numberIndex2] = 'G';
                  numberIndex2++;
                  numberBuffer2[numberIndex2] = 'C';
                  numberIndex2++;
                  break;
                case 11:
                  numberBuffer2[numberIndex2] = 'G';
                  numberIndex2++;
                  numberBuffer2[numberIndex2] = 'E';
                  numberIndex2++;
                  numberBuffer2[numberIndex2] = 'L';
                  numberIndex2++;
                  numberBuffer2[numberIndex2] = 'E';
                  numberIndex2++;
                  break;
                case 12:
                  numberBuffer2[numberIndex2] = 'G';
                  numberIndex2++;
                  numberBuffer2[numberIndex2] = 'T';
                  numberIndex2++;
                  numberBuffer2[numberIndex2] = 'E';
                  numberIndex2++;
                  numberBuffer2[numberIndex2] = 'L';
                  numberIndex2++;
                  break;
                case 13:
                  numberBuffer2[numberIndex2] = 'G';
                  numberIndex2++;
                  numberBuffer2[numberIndex2] = 'M';
                  numberIndex2++;
                  break;
                case 14:
                  numberBuffer2[numberIndex2] = 'G';
                  numberIndex2++;
                  numberBuffer2[numberIndex2] = 'I';
                  numberIndex2++;
                  numberBuffer2[numberIndex2] = 'N';
                  numberIndex2++;
                  numberBuffer2[numberIndex2] = 'D';
                  numberIndex2++;
                  break;
                default:
                  break;
              }
            }
            
            numberBuffer2[numberIndex2] = 0; // zero terminate
          }
          status(""); // Clear the old status
        }
  
        // Del button, so delete last char
        if (b == 1) {
          numberBuffer2[numberIndex2] = 0;
          if (numberIndex2 > 0) {
            numberIndex2--;
            numberBuffer2[numberIndex2] = 0;//' ';
          }
          status(""); // Clear the old status
        }
  
        if (b == 2) {
          if(numberIndex2 > 0){
            status("Sent value to serial port");
            //Serial.println(numberBuffer2);
            showIndex = false;
            useCase = 'e';
            pageIndex = 3;
            return;
          }else{
            clearAllInputs();
            showIndex = true;
            useCase = ' ';
            pageIndex = 0;
            return;
          }
        }
        // we dont really check that the text field makes sense
        // just try to call
        if (b == 0) {
          status("Value cleared");
          numberIndex2 = 0; // Reset index to 0
          numberBuffer2[numberIndex2] = 0; // Place null in buffer
        }
  
        // Update the number display field
        tft.setTextDatum(TL_DATUM);        // Use top left corner as text coord datum
        tft.setFreeFont(&FreeSans18pt7b);  // Choose a nicefont that fits box
        tft.setTextColor(DISP1_TCOLOR);     // Set the font colour
  
        // Draw the string, the value returned is the width in pixels
        int xwidth = tft.drawString(numberBuffer2, DISP1_X + 4, DISP1_Y + 12);
  
        // Now cover up the rest of the line up by drawing a black rectangle.  No flicker this way
        // but it will not work with italic or oblique fonts due to character overlap.
        tft.fillRect(DISP1_X + 4 + xwidth, DISP1_Y + 1, DISP1_W - xwidth - 5, DISP1_H - 2, TFT_WHITE);
  
        delay(10); // UI debouncing
      }
    }
  }
}

void enrolFprintIndex(){
  /*tft.fillScreen(TFT_PRIMARY4); // Clear the screen
  delay(50);
  tft.pushImage(0, 0, 480, 71, header); // Display the main header text;
  */
  tft.fillRect(0, 71, 480, 46, TFT_WHITE);
  tft.setCursor(25, 107);
  tft.setTextSize(1);
  tft.setFreeFont(LABEL2_FONT);
  tft.setTextColor(TFT_BLACK,TFT_WHITE);
  tft.println(F("Enregistrement de l’Empreinte Digitale"));
  tft.fillRect(0, 117, 480, 320, TFT_PRIMARY4); // Clear the unwanted additional header
  //tft.pushImage(16, 167, 351, 271, enrol_fprint_index_msg); // Display the enrol fingerprint index message
  /*tft.setTextSize(1);
  tft.setFreeFont(LABEL2_FONT);*/
  tft.setTextColor(TFT_BLACK,TFT_PRIMARY4);
  tft.setCursor(54, 196);
  tft.println(F("Appuyez sur Suivant en"));
  tft.setCursor(47, 219);
  tft.println(F("placant votre doigt sur le"));
  tft.setCursor(25, 247);
  tft.println(F("module et en respectant les"));
  tft.setCursor(103, 271);
  tft.println(F("consignes SVP"));
  drawNextbtn(); // Display the next button on the screen
  
  while(1){
    //Serial.println("In Fingerprint index Enrol");
    uint16_t t_x = 0, t_y = 0;
    bool pressed = tft.getTouch(&t_x, &t_y);
    
    if(pressed && nextbtn.contains(t_x, t_y)) {
      nextbtn.press(true);  // tell the button it is pressed
    } else {
      nextbtn.press(false);  // tell the button it is NOT pressed
    }
    tft.setFreeFont(LABEL1_FONT);
    if (nextbtn.justReleased()) nextbtn.drawButton();     // draw normal
    
    if (nextbtn.justPressed()) {
      nextbtn.drawButton(true);  // draw invert
  
      arduinoMega.write("$EF*"); // We send the Enrol Fingerprint command to Mega to start the fingerprint enrollment
          
      showIndex = false;
      useCase = 'e';
      pageIndex = 4;
      return;
    }
  }
}

void checkInput(){
  // Principle: Check if a command is received from the Arduino Mega
  cmdIndex = 0;
  endOfReadFromMega = false;
  endOfVREnrol = false;
  while(1){
    delay(500);
    if(Serial.available()){
      inputASCI = Serial.read();
      if(inputASCI > 0){ // Mega = $__* where __ = command from Mega
        if((char)inputASCI == '$'){ // Esp gets = $
          while(!endOfReadFromMega){
            delay(500);
            if(Serial.available()){
              inputASCI = Serial.read();
              if((char)inputASCI != '*'){ // Esp gets = __
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
        else if((char)inputASCI == '@'){ // Esp gets = @
          while(!endOfVREnrol){
            delay(500);
            if(Serial.available()){
              inputASCI = Serial.read();
              if((char)inputASCI != '*'){ // Esp gets = __
                if((char)inputASCI == '\t'){
                  enrolVRCmdBuffer[cmdIndex] = ' ';
                }else if((char)inputASCI != '\n'){
                  enrolVRCmdBuffer[cmdIndex] = (char)inputASCI;
                }
                cmdIndex++;
              }else{ // Esp gets = *
                endOfVREnrol = true;
                while(cmdIndex < 24){
                  enrolVRCmdBuffer[cmdIndex] = ' ';
                  cmdIndex++;
                }
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
  // Principle: Check if a command is received from the Second ESP32
  cmdIndex = 0;
  endOfReadFromMega = false;
  endOfVREnrol = false;
  while(1){
    delay(500);
    if(secondEsp32.available()){
      inputASCI = secondEsp32.read();
      if(inputASCI > 0){ // Second ESP32 = $__* where __ = command from Second ESP32
        if((char)inputASCI == '$'){ // Esp gets = $
          while(!endOfReadFromMega){
            delay(500);
            if(secondEsp32.available()){
              inputASCI = secondEsp32.read();
              if((char)inputASCI != '*'){ // Esp gets = __
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

void refreshPage(int cursorX, int cursorY){
  tft.fillRect(0, 117, 480, 320, TFT_PRIMARY4); // Clear the unwanted additional header
  tft.setCursor(cursorX, cursorY); // default 73 232
  tft.setFreeFont(LABEL2_FONT);
  tft.setTextSize(1);
  tft.setTextColor(TFT_BLACK, TFT_PRIMARY4);
}

void enrolFprintLogs(){
  /*tft.fillScreen(TFT_PRIMARY4); // Clear the screen
  delay(50);
  tft.pushImage(0,0, 480, 71, header); // Display the main header text;
  tft.fillRect(0, 71, 480, 46, TFT_WHITE);
  tft.setCursor(25, 107);
  tft.setTextSize(1);
  tft.setFreeFont(LABEL2_FONT);
  tft.setTextColor(TFT_BLACK,TFT_WHITE);
  tft.println("Enregistrement de l’Empreinte Digitale");*/
  
  while(1){
    checkInput();
    
    if(megaCmdBuffer[0] == 'E' && megaCmdBuffer[1] == 'F' && megaCmdBuffer[2] == '1'){
      refreshPage(50, 232);
      tft.println(F("Placez votre Doigt...."));
    }
    if(megaCmdBuffer[0] == 'E' && megaCmdBuffer[1] == 'F' && megaCmdBuffer[2] == '2'){
      refreshPage(50, 232);
      tft.println(F("Image prit..."));
    }
    if(megaCmdBuffer[0] == 'E' && megaCmdBuffer[1] == 'F' && megaCmdBuffer[2] == '3'){
      refreshPage(50, 232);
      tft.println(F("Pas de doigt trouver"));
    }
    if(megaCmdBuffer[0] == 'E' && megaCmdBuffer[1] == 'F' && megaCmdBuffer[2] == '4'){
      refreshPage(50, 232);
      tft.println(F("Image Converti"));
    }
    if(megaCmdBuffer[0] == 'E' && megaCmdBuffer[1] == 'F' && megaCmdBuffer[2] == '5'){
      refreshPage(50, 232);
      tft.println(F("Retirez votre doigt SVP"));
    }
    if(megaCmdBuffer[0] == 'E' && megaCmdBuffer[1] == 'F' && megaCmdBuffer[2] == '6'){
      refreshPage(50, 232);
      tft.println("Replacez le meme doigt SVP");
    }
    if(megaCmdBuffer[0] == 'E' && megaCmdBuffer[1] == 'F' && megaCmdBuffer[2] == '7'){
      refreshPage(50, 232);
      tft.println(F("Pas de doigt trouver"));
      delay(1000);
      refreshPage(50, 232);
      tft.println(F("Changez la position du doigt"));
    }
    if(megaCmdBuffer[0] == 'E' && megaCmdBuffer[1] == 'F' && megaCmdBuffer[2] == '8'){
      refreshPage(50, 232);
      tft.println(F("Creation du modele"));
    }
    if(megaCmdBuffer[0] == 'E' && megaCmdBuffer[1] == 'F' && megaCmdBuffer[2] == '>'){
      refreshPage(50, 232);
      tft.println(F("Erreur imagerie"));
    }
    if(megaCmdBuffer[0] == 'E' && megaCmdBuffer[1] == 'F' && megaCmdBuffer[2] == '9'){
      showIndex = false;
      useCase = 'e';
      pageIndex = 5;
      return;
    }
    if((megaCmdBuffer[0] == 'E' && megaCmdBuffer[1] == 'F') && (megaCmdBuffer[2] == 'A' || megaCmdBuffer[2] == 'B' || megaCmdBuffer[2] == 'C' || megaCmdBuffer[2] == 'D' || megaCmdBuffer[2] == '/' || megaCmdBuffer[2] == '<' || megaCmdBuffer[2] == '|')){
      showIndex = false;
      useCase = 'e';
      pageIndex = 6;
      return;
    }
  }
}

void enrolFprintSuccess(){
  /*tft.fillScreen(TFT_PRIMARY4); // Clear the screen
  delay(50);
  tft.pushImage(0,0, 480, 71, header); // Display the main header text;
  tft.fillRect(0, 71, 480, 46, TFT_WHITE);
  tft.setCursor(25, 107);
  tft.setTextSize(1);
  tft.setFreeFont(LABEL2_FONT);
  tft.setTextColor(TFT_BLACK,TFT_WHITE);
  tft.println("Enregistrement de l’Empreinte Digitale");*/
  tft.fillRect(0, 117, 480, 320, TFT_PRIMARY4); // Clear the unwanted additional header
  tft.setCursor(50, 211);
  /*tft.setTextSize(1);
  tft.setFreeFont(LABEL2_FONT);*/
  tft.setTextColor(TFT_CSUCCESS,TFT_PRIMARY4);
  tft.println(F("Enregistrement succes"));  // Display the enrol fingerprint success message
  drawNextbtn(); // Display the next button on the screen
  
  while(1){
    //Serial.println("In Fingerprint index Enrol");
    uint16_t t_x = 0, t_y = 0;
    bool pressed = tft.getTouch(&t_x, &t_y);
    
    if(pressed && nextbtn.contains(t_x, t_y)) {
      nextbtn.press(true);  // tell the button it is pressed
    } else {
      nextbtn.press(false);  // tell the button it is NOT pressed
    }
    tft.setFreeFont(LABEL1_FONT);
    if (nextbtn.justReleased()) nextbtn.drawButton();     // draw normal
    
    if (nextbtn.justPressed()) {
      nextbtn.drawButton(true);  // draw invert
      showIndex = false;
      useCase = 'e';
      pageIndex = 7;
      return;
    }
  }
}

void enrolFprintError(){
  /*tft.fillScreen(TFT_PRIMARY4); // Clear the screen
  delay(50);
  tft.pushImage(0,0, 480, 71, header); // Display the main header text;
  tft.fillRect(0, 71, 480, 46, TFT_WHITE);
  tft.setCursor(25, 107);
  tft.setFreeFont(LABEL2_FONT);
  tft.setTextSize(1);
  tft.setTextColor(TFT_BLACK,TFT_WHITE);
  tft.println("Enregistrement de l’Empreinte Digitale");*/
  tft.fillRect(0, 117, 480, 320, TFT_PRIMARY4); // Clear the unwanted additional header
  tft.setCursor(50, 211);
  /*tft.setTextSize(1);
  tft.setFreeFont(LABEL2_FONT);*/
  tft.setTextColor(TFT_RED,TFT_PRIMARY4);
  tft.println(F("Erreur enregistrement")); // Display the enrol fingerprint error message
  tft.setCursor(25, 252);
  /*tft.setFreeFont(LABEL2_FONT);
  tft.setTextSize(1);*/
  tft.setTextColor(TFT_BLACK, TFT_PRIMARY4);
  switch(megaCmdBuffer[2]){ // We display the error message with respect to the type
    case 'A':
      tft.println(F("Pas de correspondance d'empreinte"));
      break;
    case 'B':
      tft.println(F("Impossible d'enregistrer dans cette ID"));
      break;
    case 'C':
      tft.println(F("Erreur decriture dans le flash"));
      break;
    case 'D':
      tft.println(F("Erreur inconnue"));
      break;
    case '/':
      tft.println(F("Image desordonner"));
      break;
    case '<':
      tft.println(F("Erreur de Communication"));
      break;
    case '|':
      tft.setCursor(25, 232);
      tft.println(F("Impossible de trouver les"));
      tft.setCursor(25, 252);
      tft.println(F("caractéristiques de lempreinte"));
      break;
    default:
      tft.println(F("Impossible d'enregistrer lEmpreinte"));
      break;
  }
  tft.setCursor(25, 270);
  tft.println(F("Veuillez ressayer"));
  drawHomebtn(); // Display the next button on the screen
  
  while(1){
    //Serial.println("In Fingerprint index Enrol");
    uint16_t t_x = 0, t_y = 0;
    bool pressed = tft.getTouch(&t_x, &t_y);
    
    if(pressed && homebtn.contains(t_x, t_y)) {
      homebtn.press(true);  // tell the button it is pressed
    } else {
      homebtn.press(false);  // tell the button it is NOT pressed
    }
    tft.setFreeFont(LABEL1_FONT);
    if (homebtn.justReleased()) homebtn.drawButton();     // draw normal
    
    if (homebtn.justPressed()) {
      homebtn.drawButton(true);  // draw invert
      clearAllInputs();
      showIndex = true;
      useCase = ' ';
      pageIndex = 0;
      return;
    }
  }
}

void enrolVoiceIndex(){
  /*tft.fillScreen(TFT_PRIMARY4); // Clear the screen
  delay(50);
  tft.pushImage(0, 0, 480, 71, header); // Display the main header text;
  */
  tft.fillRect(0, 71, 480, 46, TFT_WHITE);
  tft.setCursor(99, 107);
  tft.setTextSize(1);
  tft.setFreeFont(LABEL2_FONT);
  tft.setTextColor(TFT_BLACK,TFT_WHITE);
  tft.println(F("Enregistrement de la Voix"));
  tft.fillRect(0, 117, 480, 320, TFT_PRIMARY4); // Clear the unwanted additional header
  //tft.pushImage(16, 167, 351, 271, enrol_fprint_index_msg); // Display the enrol fingerprint index message
  /*tft.setTextSize(1);
  tft.setFreeFont(LABEL2_FONT);*/
  tft.setTextColor(TFT_BLACK,TFT_PRIMARY4);
  tft.setCursor(56, 160);
  tft.println(F("Appuyez sur Suivant en"));
  tft.setCursor(35, 185);
  tft.println(F("placant votre bouche sur le"));
  tft.setCursor(38, 209);
  tft.println(F("micro et en respectant les "));
  tft.setCursor(99, 237);
  tft.println(F("consignes SVP"));
  tft.setCursor(38, 278);
  tft.println(F("Choisissez deux mots pour"));
  tft.setCursor(88, 304);
  tft.println(F("votre code vocale (deux nom simple)"));
  drawNextbtn(); // Display the next button on the screen
  while(1){
    //Serial.println("In Fingerprint index Enrol");
    uint16_t t_x = 0, t_y = 0;
    bool pressed = tft.getTouch(&t_x, &t_y);
    
    if(pressed && nextbtn.contains(t_x, t_y)) {
      nextbtn.press(true);  // tell the button it is pressed
    } else {
      nextbtn.press(false);  // tell the button it is NOT pressed
    }
    tft.setFreeFont(LABEL1_FONT);
    if (nextbtn.justReleased()) nextbtn.drawButton();     // draw normal
    
    if (nextbtn.justPressed()){
      nextbtn.drawButton(true);  // draw invert
      
      arduinoMega.write("$EV*"); // Command to Mega to start the voice enrollment
      
      showIndex = false;
      useCase = 'e';
      pageIndex = 8;
      return;
    }
  }
}

void enrolVoiceLogs(){
  /*tft.fillScreen(TFT_PRIMARY4); // Clear the screen
  delay(50);
  tft.pushImage(0,0, 480, 71, header); // Display the main header text;
  tft.fillRect(0, 71, 480, 46, TFT_WHITE);
  tft.setCursor(99, 107);
  tft.setTextSize(1);
  tft.setFreeFont(LABEL2_FONT);
  tft.setTextColor(TFT_BLACK,TFT_WHITE);
  tft.println(F("Enregistrement de la Voix"));*/
  // refreshPage(50, 232);
  //tft.println("Enrol Logs");
  
  char voiceTrainCmd[25] = "$sigtrain"; // $sigtrain 0 19p045*
  char matriculeCmd[7]; // -->19p000*<--
  
  char currentId[5]; // --> 000 <--
  int cId;
  int idCount = 0;
  bool endGetId = false;
  
  int j = 0;
  for(int i=0; i<NUM1_LEN; i++){
    if(numberBuffer1[i] != 0 && j < 6){
      matriculeCmd[j] = numberBuffer1[i]; // Store the matricule in new buffer
      j++;
    }
  }
  matriculeCmd[6] = '*'; // Add a '*' at the end of the matricule e.g 19P045*
  delay(1000);
  
  while(!endGetId){ // We get the freeID from the Arduino Mega
     if(Serial.available()){
       cId = Serial.read();
       if((char)cId == '$'){
        currentId[idCount]= ' ';
        idCount++;
        while(!endGetId){
          if(Serial.available()){
            cId = Serial.read();
            if((char)cId != '*'){
              currentId[idCount]=(char)cId;
              idCount++;
            }else{
              currentId[4]= ' ';
              idCount = 0;
              endGetId = true;
            }
          }
        }
       }
     }
   }
   strcat(voiceTrainCmd,currentId);
   strcat(voiceTrainCmd,matriculeCmd);
  delay(500); // We wait 0.5s before sending the command to receiveCMD() fxn
  arduinoMega.write(voiceTrainCmd);
  
  while(1){
    //Serial.println("In Voice logs Enrol");
    checkInput();
    
    if(endOfVREnrol){
      refreshPage(50, 232);
      tft.println(enrolVRCmdBuffer);
    }
    if(megaCmdBuffer[0] == 'E' && megaCmdBuffer[1] == 'V' && megaCmdBuffer[2] == '2'){
      refreshPage(50, 232);
      tft.println(F("Temps ecoulé"));
    }
    if(megaCmdBuffer[0] == 'E' && megaCmdBuffer[1] == 'V' && megaCmdBuffer[2] == '3'){
      refreshPage(50, 232);
      tft.println(F("ID hors intervalle"));
    }
    if(megaCmdBuffer[0] == 'E' && megaCmdBuffer[1] == 'V' && megaCmdBuffer[2] == '4'){
      refreshPage(50, 232);
      tft.println(F("Erreur inconnue"));
    }
    if(megaCmdBuffer[0] == 'E' && megaCmdBuffer[1] == 'V' && megaCmdBuffer[2] == '8'){
      refreshPage(50, 232);
      tft.println(F("Entrainement Terminé"));
      delay(1000);
      showIndex = false;
      useCase = 'e';
      pageIndex = 9;
      return;
    }
    if((megaCmdBuffer[0] == 'E' && megaCmdBuffer[1] == 'V') && (megaCmdBuffer[2] == 'A' || megaCmdBuffer[2] == 'B' || megaCmdBuffer[2] == 'C')){
      showIndex = false;
      useCase = 'e';
      pageIndex = 10;
      return;
    }
  }
}

void enrolVoiceSuccess(){
  /*tft.fillScreen(TFT_PRIMARY4); // Clear the screen
  delay(50);
  tft.pushImage(0,0, 480, 71, header); // Display the main header text;
  tft.fillRect(0, 71, 480, 46, TFT_WHITE);
  tft.setCursor(99, 107);
  tft.setTextSize(1);
  tft.setFreeFont(LABEL2_FONT);
  tft.setTextColor(TFT_BLACK,TFT_WHITE);
  tft.println("Enregistrement de la Voix");*/
  tft.fillRect(0, 117, 480, 320, TFT_PRIMARY4); // Clear the unwanted additional header
  tft.setCursor(54, 211);
  /*tft.setTextSize(1);
  tft.setFreeFont(LABEL2_FONT);*/
  tft.setTextColor(TFT_CSUCCESS,TFT_PRIMARY4);
  tft.println(F("Enregistrement succes"));  // Display the enrol fingerprint success message
  tft.setCursor(54, 252);
  tft.setTextSize(1);
  tft.setFreeFont(LABEL2_FONT);
  tft.setTextColor(TFT_BLACK,TFT_PRIMARY4);
  tft.println(F("Memorisez votre code vocale"));
  drawNextbtn(); // Display the next button on the screen
  while(1){
    //Serial.println("In Fingerprint index Enrol");
    checkInput();

    if(megaCmdBuffer[0] == 'E' && megaCmdBuffer[1] == 'V' && megaCmdBuffer[2] == '9'){
      bool getAllEnrolInfo;
      char enrolVoiceID[4];
      char enrolVoiceSIG[7];
      int enrolRet, vCount, i = 0;
      while(i < 2){
        getAllEnrolInfo = false;
        vCount = 0;
        if(Serial.available()){
          enrolRet = Serial.read();
          if((char)enrolRet == '$'){
            while(!getAllEnrolInfo){
              enrolRet = Serial.read();
              if((char)enrolRet != '*'){ // If the character is valid
                if(i == 0){
                  enrolVoiceID[vCount] = (char)enrolRet;
                  vCount++;
                }else{ // if i == 1
                  enrolVoiceSIG[vCount] = (char)enrolRet;
                  vCount++;
                }
              }else{ // If the character is the ending character
                 getAllEnrolInfo = true;
                 i++;
                 vCount = 0;
              }
            }
          }
        }
      }
      tft.setCursor(54, 280);
      tft.print("ID: ");tft.print(enrolVoiceID);
      tft.print(" SIG: ");tft.println(enrolVoiceSIG);
    }
    
    uint16_t t_x = 0, t_y = 0;
    bool pressed = tft.getTouch(&t_x, &t_y);
    
    if(pressed && nextbtn.contains(t_x, t_y)) {
      nextbtn.press(true);  // tell the button it is pressed
    } else {
      nextbtn.press(false);  // tell the button it is NOT pressed
    }
    tft.setFreeFont(LABEL1_FONT);
    if (nextbtn.justReleased()) nextbtn.drawButton();     // draw normal
    
    if (nextbtn.justPressed()) {
      nextbtn.drawButton(true);  // draw invert
      showIndex = false;
      useCase = 'e';
      pageIndex = 11;
      return;
    }
  }
}

void enrolVoiceError(){
  /*tft.fillScreen(TFT_PRIMARY4); // Clear the screen
  delay(50);
  tft.pushImage(0,0, 480, 71, header); // Display the main header text;
  tft.fillRect(0, 71, 480, 46, TFT_WHITE);
  tft.setCursor(99, 107);
  tft.setTextSize(1);
  tft.setFreeFont(LABEL2_FONT);
  tft.setTextColor(TFT_BLACK,TFT_WHITE);
  tft.println("Enregistrement de la Voix");*/
  tft.fillRect(0, 117, 480, 320, TFT_PRIMARY4); // Clear the unwanted additional header
  tft.setCursor(50, 211);
  /*tft.setTextSize(1);
  tft.setFreeFont(LABEL2_FONT);*/
  tft.setTextColor(TFT_RED,TFT_PRIMARY4);
  tft.println(F("Erreur enregistrement")); // Display the enrol fingerprint error message
  tft.setCursor(25, 252);
  /*tft.setFreeFont(LABEL2_FONT);
  tft.setTextSize(1);*/
  tft.setTextColor(TFT_BLACK, TFT_PRIMARY4);
  
  switch(megaCmdBuffer[2]){ // We display the error message with respect to the type
    case 'A':
      tft.println(F("Erreur de command"));
      break;
    case 'B':
      tft.println(F("Mauvais Format de command"));
      break;
    case 'C':
      tft.println(F("Temps ecoulé"));
      break;
    default:
      tft.println(F("Impossible d'enregistrer la Voix"));
      break;
  }
  tft.setCursor(25, 270);
  tft.println(F("Veuillez ressayer"));
  drawHomebtn(); // Display the next button on the screen
  
  while(1){
    //Serial.println("In Voice error Enrol");
    uint16_t t_x = 0, t_y = 0;
    bool pressed = tft.getTouch(&t_x, &t_y);
    
    if(pressed && nextbtn.contains(t_x, t_y)) {
      nextbtn.press(true);  // tell the button it is pressed
    } else {
      nextbtn.press(false);  // tell the button it is NOT pressed
    }
    tft.setFreeFont(LABEL1_FONT);
    if (nextbtn.justReleased()) nextbtn.drawButton();     // draw normal
    
    if (nextbtn.justPressed()) {
      nextbtn.drawButton(true);  // draw invert
      clearAllInputs();
      showIndex = true;
      useCase = ' ';
      pageIndex = 0;
      return;
    }
  }
}

void enrolBilan(){
  /*tft.fillScreen(TFT_PRIMARY4); // Clear the screen
  delay(50);
  tft.pushImage(0,0, 480, 71, header); // Display the main header text;
  */
  tft.fillRect(0, 71, 480, 46, TFT_WHITE);
  tft.setCursor(121, 107);
  tft.setTextSize(1);
  tft.setFreeFont(LABEL2_FONT);
  tft.setTextColor(TFT_BLACK,TFT_WHITE);
  tft.println(F("Bilan de l’Enrolement"));
  tft.fillRect(0, 117, 480, 320, TFT_PRIMARY4); // Clear the unwanted additional header
  /*tft.setTextSize(1);
  tft.setFreeFont(LABEL2_FONT);*/
  tft.setTextColor(TFT_BLACK,TFT_PRIMARY4);

  //String fingerTemplate = ""; // the real template
  String enrolDateTime = ""; // e.g 2023-01-06T15:40:25.330Z
          
  while(1){
    checkInput();
    if(megaCmdBuffer[0] == 'A' && megaCmdBuffer[1] == 'B' && megaCmdBuffer[2] == '1'){
      checkInput();
      if(megaCmdBuffer[0] == 'A' && megaCmdBuffer[1] == 'B' && megaCmdBuffer[2] == '2'){
        checkInput();
        if(megaCmdBuffer[0] == 'A' && megaCmdBuffer[1] == 'B' && megaCmdBuffer[2] == '3'){

          int inputArray, arrayCount = 1; // Only for the date
          bool isAllInfo = false;

          while(arrayCount < 2){
            isAllInfo = false;
            if(Serial.available()){
              inputArray = Serial.read();
              if((char)inputArray == '$'){ // If we have the start command
                while(!isAllInfo){
                  inputArray = Serial.read();
                  if((char)inputArray != '*'){
                    if(arrayCount == 0){ // If it is fingerprint template
                      //fingerTemplate = fingerTemplate + (char)inputArray;
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
          tft.setCursor(16, 130);
          tft.println(F("Vous etes desormais enroler"));
          tft.setCursor(16, 160);
          tft.print(F("avec matricule: "));
          tft.println(numberBuffer1);
          tft.setCursor(16, 185);
          tft.print(F("en classe: "));
          tft.println(numberBuffer2);
          tft.setCursor(16, 200);
          tft.print(F("à : ")); tft.println(authTime);
          tft.setCursor(16, 237);
          tft.print(F("sur le nom de: "));
          tft.setCursor(16, 253);
          tft.println(numberBuffer3);
          break;
        }
      }
    }
  }

  // Envoyer les informations du nouveau etudiant au deuxieme ESP32
  secondEsp32.write("$1T2*");
  delay(50);
  secondEsp32.write('$');
  secondEsp32.write(numberBuffer1); // Matricule
  secondEsp32.write('*');
  secondEsp32.write('$');
  secondEsp32.write(numberBuffer2); // Class
  secondEsp32.write('*');
  secondEsp32.write('$');
  secondEsp32.write(numberBuffer3); // Name
  secondEsp32.write('*');

  while(1){
    checkInput2();
    if(megaCmdBuffer[0] == 'E' && megaCmdBuffer[1] == 'S' && megaCmdBuffer[2] == 'S'){
      tft.setCursor(16, 273);
      tft.print(F("Success Student Save in Server"));
      break;
    }
    if(megaCmdBuffer[0] == 'E' && megaCmdBuffer[1] == 'S' && megaCmdBuffer[2] == 'E'){
      tft.setCursor(16, 273);
      tft.print(F("Error Student Save in Server"));
      break;
    }
  }
  drawHomebtn();
  while(1){
    //Serial.println("In Fingerprint index Enrol");
    uint16_t t_x = 0, t_y = 0;
    bool pressed = tft.getTouch(&t_x, &t_y);
    
    if(pressed && homebtn.contains(t_x, t_y)) {
      homebtn.press(true);  // tell the button it is pressed
    } else {
      homebtn.press(false);  // tell the button it is NOT pressed
    }
    tft.setFreeFont(LABEL1_FONT);
    if (homebtn.justReleased()) homebtn.drawButton();     // draw normal
    
    if (homebtn.justPressed()) {
      homebtn.drawButton(true);  // draw invert
      clearAllInputs();
      showIndex = true;
      useCase = ' ';
      pageIndex = 0;
      return;
    }
  }
}
/************************************************************************************************************************************/
//************************************************************************************************************************************
/*************************************************** AUTHENTIFICATION START *********************************************************/
//************************************************************************************************************************************
/************************************************************************************************************************************/

void authFprintIndex(){
  tft.fillScreen(TFT_PRIMARY4); // Clear the screen
  delay(50);
  tft.pushImage(0, 0, 480, 71, header); // Display the main header text;
  tft.fillRect(0, 71, 480, 46, TFT_WHITE);
  tft.setCursor(21, 107);
  tft.setTextSize(1);
  tft.setFreeFont(LABEL2_FONT);
  tft.setTextColor(TFT_BLACK,TFT_WHITE);
  tft.println(F("Authentification de l’Empreinte Digitale"));
  tft.fillRect(0, 117, 480, 320, TFT_PRIMARY4); // Clear the unwanted additional header
  //tft.pushImage(16, 167, 351, 271, enrol_fprint_index_msg); // Display the enrol fingerprint index message
  /*tft.setTextSize(1);
  tft.setFreeFont(LABEL2_FONT);*/
  tft.setTextColor(TFT_BLACK,TFT_PRIMARY4);
  tft.setCursor(54, 196);
  tft.println(F("Appuyez sur Suivant en"));
  tft.setCursor(47, 219);
  tft.println(F("placant votre doigt sur le"));
  tft.setCursor(25, 247);
  tft.println(F("module et en respectant les"));
  tft.setCursor(103, 271);
  tft.println(F("consignes SVP"));
  drawNextbtn(); // Display the next button on the screen
  
  while(1){
    //Serial.println("In Fingerprint index Enrol");
    uint16_t t_x = 0, t_y = 0;
    bool pressed = tft.getTouch(&t_x, &t_y);
    
    if(pressed && nextbtn.contains(t_x, t_y)) {
      nextbtn.press(true);  // tell the button it is pressed
    } else {
      nextbtn.press(false);  // tell the button it is NOT pressed
    }
    tft.setFreeFont(LABEL1_FONT);
    if (nextbtn.justReleased()) nextbtn.drawButton();     // draw normal
    
    if (nextbtn.justPressed()) {
      nextbtn.drawButton(true);  // draw invert

      arduinoMega.write("$AF*");
      
      showIndex = false;
      useCase = 'a';
      pageIndex = 1;
      return;
    }
  }
}

void authFprintLogs(){
  /*tft.fillScreen(TFT_PRIMARY4); // Clear the screen
  delay(50);
  tft.pushImage(0,0, 480, 71, header); // Display the main header text;
  tft.fillRect(0, 71, 480, 46, TFT_WHITE);
   tft.setCursor(21, 107);
  tft.setTextSize(1);
  tft.setFreeFont(LABEL2_FONT);
  tft.setTextColor(TFT_BLACK,TFT_WHITE);
  tft.println("Authentification de l’Empreinte Digitale");*/
  tft.fillRect(0, 117, 480, 320, TFT_PRIMARY4); // Clear the unwanted additional header
  tft.setCursor(73, 232);
  /*tft.setFreeFont(LABEL2_FONT);
  tft.setTextSize(1);*/
  tft.setTextColor(TFT_BLACK, TFT_PRIMARY4);
  //tft.println("Enrol Logs");
  while(1){
    //Serial.println("In Fingerprint logs Auth");
    checkInput();

    if(megaCmdBuffer[0] == 'A' && megaCmdBuffer[1] == 'F' && megaCmdBuffer[2] == '1'){
      refreshPage(50, 232);
      tft.println(F("Placez votre Doigt...."));
    }
    if(megaCmdBuffer[0] == 'A' && megaCmdBuffer[1] == 'F' && megaCmdBuffer[2] == '2'){
      refreshPage(50, 232);
      tft.println(F("Image prit..."));
    }
    if(megaCmdBuffer[0] == 'A' && megaCmdBuffer[1] == 'F' && megaCmdBuffer[2] == '3'){
      refreshPage(50, 232);
      tft.println(F("Pas de doigt trouver"));
    }
    if(megaCmdBuffer[0] == 'A' && megaCmdBuffer[1] == 'F' && megaCmdBuffer[2] == '4'){
      refreshPage(50, 232);
      tft.println(F("Image Converti"));
    }
    if((megaCmdBuffer[0] == 'A' && megaCmdBuffer[1] == 'F') && (megaCmdBuffer[2] == '/' || megaCmdBuffer[2] == '<' || megaCmdBuffer[2] == '|' || megaCmdBuffer[2] == 'A' || megaCmdBuffer[2] == 'D')){
      showIndex = false;
      useCase = 'a';
      pageIndex = 3; // To Auth error page
      return;
    }
    if(megaCmdBuffer[0] == 'A' && megaCmdBuffer[1] == 'F' && megaCmdBuffer[2] == '9'){
      showIndex = false;
      useCase = 'a';
      pageIndex = 2; // To Auth success page
      return;
    }
  }
}

void authFprintSuccess(){
  /*tft.fillScreen(TFT_PRIMARY4); // Clear the screen
  delay(50);
  tft.pushImage(0,0, 480, 71, header); // Display the main header text;
  tft.fillRect(0, 71, 480, 46, TFT_WHITE);
  tft.setCursor(21, 107);
  tft.setTextSize(1);
  tft.setFreeFont(LABEL2_FONT);
  tft.setTextColor(TFT_BLACK,TFT_WHITE);
  tft.println("Authentification de l'Empreinte Digitale");*/
  tft.fillRect(0, 117, 480, 320, TFT_PRIMARY4); // Clear the unwanted additional header
  tft.setCursor(51, 211);
  /*tft.setTextSize(1);
  tft.setFreeFont(LABEL2_FONT);*/
  tft.setTextColor(TFT_CSUCCESS,TFT_PRIMARY4);
  tft.println(F("Authentification succes"));  // Display the enrol fingerprint success message

  char fingerID[4];
  char confidence[4];
  int inputFAuth;
  int cmdCount = 0;
  int inputCount = 0;
  bool getAllAuthInfo = false;
  
  while(inputCount < 2){
    getAllAuthInfo = false;
    cmdCount = 0;
    if(Serial.available()){
      inputFAuth = Serial.read();
      if((char)inputFAuth == '$'){
        while(!getAllAuthInfo){
          inputFAuth = Serial.read();
          if((char)inputFAuth != '*'){ // If the character is valid
            if(inputCount == 0){
              fingerID[cmdCount] = (char)inputFAuth;
              cmdCount++;
            }else{ // if inputCount == 1
              confidence[cmdCount] = (char)inputFAuth;
              cmdCount++;
            }
          }else{ // If the character is the ending character
            getAllAuthInfo = true;
            inputCount++;
            cmdCount = 0;
          }
        }
      }
    }
  }
  tft.setCursor(54, 280);
  tft.print(F("ID: "));tft.print(fingerID);
  tft.print(F(" Confidence: "));tft.println(confidence);
  drawNextbtn(); // Display the next button on the screen
  
  while(1){
    //Serial.println("In Fingerprint index Enrol");
    uint16_t t_x = 0, t_y = 0;
    bool pressed = tft.getTouch(&t_x, &t_y);
    
    if(pressed && nextbtn.contains(t_x, t_y)) {
      nextbtn.press(true);  // tell the button it is pressed
    } else {
      nextbtn.press(false);  // tell the button it is NOT pressed
    }
    tft.setFreeFont(LABEL1_FONT);
    if (nextbtn.justReleased()) nextbtn.drawButton();     // draw normal
    
    if (nextbtn.justPressed()) {
      nextbtn.drawButton(true);  // draw invert
      showIndex = false;
      useCase = 'a';
      pageIndex = 4;
      return;
    }
  }
}

void authFprintError(){
  /*tft.fillScreen(TFT_PRIMARY4); // Clear the screen
  delay(50);
  tft.pushImage(0,0, 480, 71, header); // Display the main header text;
  tft.fillRect(0, 71, 480, 46, TFT_WHITE);
  tft.setCursor(21, 107);
  tft.setTextSize(1);
  tft.setFreeFont(LABEL2_FONT);
  tft.setTextColor(TFT_BLACK,TFT_WHITE);
  tft.println("Authentification de l’Empreinte Digitale");*/
  tft.fillRect(0, 117, 480, 320, TFT_PRIMARY4); // Clear the unwanted additional header
  tft.setCursor(50, 211);
  tft.setTextSize(1);
  tft.setFreeFont(LABEL2_FONT);
  tft.setTextColor(TFT_RED,TFT_PRIMARY4);
  tft.println(F("Erreur d authentification")); // Display the enrol fingerprint error message
  tft.setCursor(25, 252);
  /*tft.setFreeFont(LABEL2_FONT);
  tft.setTextSize(1)*/
  tft.setTextColor(TFT_BLACK, TFT_PRIMARY4);
  switch(megaCmdBuffer[2]){ // We display the error message with respect to the type
    case '/':
      tft.println(F("Image desordonner"));
      break;
    case '<':
      tft.println(F("Erreur de Communication"));
      break;
    case '|':
      tft.setCursor(25, 232);
      tft.println(F("Impossible de trouver les"));
      tft.setCursor(25, 252);
      tft.println(F("caracteristiques de lempreinte"));
      break;
    case 'A':
      tft.println(F("Aucune correspondance trouvé"));
      break;
    case 'B':
      tft.println(F("Temps ecoulé"));
      break;
    case 'D':
      tft.println(F("Erreur inconnue"));
      break;
    default:
      tft.println(F("Impossible d authentifier"));
      break;
  }
  tft.setCursor(25, 270);
  tft.println(F("Veuillez Ressayer"));
  drawHomebtn(); // Display the next button on the screen
  
  while(1){
    uint16_t t_x = 0, t_y = 0;
    bool pressed = tft.getTouch(&t_x, &t_y);
    
    if(pressed && homebtn.contains(t_x, t_y)) {
      homebtn.press(true);  // tell the button it is pressed
    } else {
      homebtn.press(false);  // tell the button it is NOT pressed
    }
    tft.setFreeFont(LABEL1_FONT);
    if (homebtn.justReleased()) homebtn.drawButton();     // draw normal
    
    if (homebtn.justPressed()) {
      homebtn.drawButton(true);  // draw invert
      showIndex = true;
      useCase = ' ';
      pageIndex = 0;
      return;
    }
  }
}

void authVoiceIndex(){
  /*tft.fillScreen(TFT_PRIMARY4); // Clear the screen
  delay(50);
  tft.pushImage(0, 0, 480, 71, header); // Display the main header text;
  */
  tft.fillRect(0, 71, 480, 46, TFT_WHITE);
  tft.setCursor(94, 107);
  tft.setTextSize(1);
  tft.setFreeFont(LABEL2_FONT);
  tft.setTextColor(TFT_BLACK,TFT_WHITE);
  tft.println(F("Authentification de la Voix"));
  tft.fillRect(0, 117, 480, 320, TFT_PRIMARY4); // Clear the unwanted additional header
  //tft.pushImage(16, 167, 351, 271, enrol_fprint_index_msg); // Display the enrol fingerprint index message
  tft.setTextSize(1);
  tft.setFreeFont(LABEL2_FONT);
  tft.setTextColor(TFT_BLACK,TFT_PRIMARY4);
  tft.setCursor(56, 160);
  tft.println(F("Appuyez sur Suivant en"));
  tft.setCursor(35, 185);
  tft.println(F("placant votre bouche sur le"));
  tft.setCursor(38, 209);
  tft.println(F("micro et en respectant les "));
  tft.setCursor(99, 237);
  tft.println(F("consignes SVP"));
  tft.setCursor(38, 278);
  tft.println(F("Utilisez votre code vocale"));
  tft.setCursor(88, 304);
  tft.println(F("(deux mots)"));
  drawNextbtn(); // Display the next button on the screen
  while(1){
    uint16_t t_x = 0, t_y = 0;
    bool pressed = tft.getTouch(&t_x, &t_y);
    
    if(pressed && nextbtn.contains(t_x, t_y)) {
      nextbtn.press(true);  // tell the button it is pressed
    } else {
      nextbtn.press(false);  // tell the button it is NOT pressed
    }
    tft.setFreeFont(LABEL1_FONT);
    if (nextbtn.justReleased()) nextbtn.drawButton();     // draw normal
    
    if (nextbtn.justPressed()) {
      nextbtn.drawButton(true);  // draw invert
      
      arduinoMega.write("$AV*");
      
      showIndex = false;
      useCase = 'a';
      pageIndex = 5;
      return;
    }
  }
}

void authVoiceLogs(){
  /*tft.fillScreen(TFT_PRIMARY4); // Clear the screen
  delay(50);
  tft.pushImage(0,0, 480, 71, header); // Display the main header text;
  tft.fillRect(0, 71, 480, 46, TFT_WHITE);
  tft.setCursor(94, 107);
  tft.setTextSize(1);
  tft.setFreeFont(LABEL2_FONT);
  tft.setTextColor(TFT_BLACK,TFT_WHITE);
  tft.println("Authentification de la Voix");*/
  tft.fillRect(0, 117, 480, 320, TFT_PRIMARY4); // Clear the unwanted additional header
  /*tft.setFreeFont(LABEL2_FONT);
  tft.setTextSize(1);*/
  tft.setTextColor(TFT_BLACK, TFT_PRIMARY4);
  while(1){
    //Serial.println("In Voice logs Enrol");
    checkInput();
    delay(2000);
    
    if(megaCmdBuffer[0] == 'A' && megaCmdBuffer[1] == 'V' && megaCmdBuffer[2] == '1'){
      refreshPage(50, 232);
      tft.println(F("Module detecté...."));
    }
    if(megaCmdBuffer[0] == 'A' && megaCmdBuffer[1] == 'V' && megaCmdBuffer[2] == '2'){
      refreshPage(50, 232);
      tft.println(F("Commande Chargé...."));
    }
    if(megaCmdBuffer[0] == 'A' && megaCmdBuffer[1] == 'V' && megaCmdBuffer[2] == '3'){
      refreshPage(50, 232);
      tft.println(F("En attente du code vocale"));
    }
    
    if((megaCmdBuffer[0] == 'A' && megaCmdBuffer[1] == 'V') && (megaCmdBuffer[2] == 'A' || megaCmdBuffer[2] == 'B' || megaCmdBuffer[2] == 'C' || megaCmdBuffer[2] == 'D' || megaCmdBuffer[2] == 'E')){
      showIndex = false;
      useCase = 'a';
      pageIndex = 7; // To Auth error page
      return;
    }
    if(megaCmdBuffer[0] == 'A' && megaCmdBuffer[1] == 'V' && megaCmdBuffer[2] == '9'){
      showIndex = false;
      useCase = 'a';
      pageIndex = 6; // To Auth success page
      return;
    }
  }
}

void authVoiceSuccess(){
  /*tft.fillScreen(TFT_PRIMARY4); // Clear the screen
  delay(50);
  tft.pushImage(0,0, 480, 71, header); // Display the main header text;
  tft.fillRect(0, 71, 480, 46, TFT_WHITE);
  tft.setCursor(94, 107);
  tft.setTextSize(1);
  tft.setFreeFont(LABEL2_FONT);
  tft.setTextColor(TFT_BLACK,TFT_WHITE);
  tft.println("Authentification de la Voix");*/
  tft.fillRect(0, 117, 480, 320, TFT_PRIMARY4); // Clear the unwanted additional header
  tft.setCursor(51, 211);
  /*tft.setTextSize(1);
  tft.setFreeFont(LABEL2_FONT);*/
  tft.setTextColor(TFT_CSUCCESS,TFT_PRIMARY4);
  tft.println(F("Authentification succes"));  // Display the enrol fingerprint success message

  char voiceSIG[4];
  int inputVoiceSIG;
  int cmdCount = 0;
  bool getAllAuthSIG = false;
  
  while(1){
    if(Serial.available()){
      inputVoiceSIG = Serial.read();
      if((char)inputVoiceSIG == '$'){
        while(!getAllAuthSIG){
          inputVoiceSIG = Serial.read();
          if((char)inputVoiceSIG != '*'){ // If the character is valid
            voiceSIG[cmdCount] = (char)inputVoiceSIG;
            cmdCount++;
          }else{ // If the character is the ending character
            getAllAuthSIG = true;
          }
        }
        break;
      }
    }
  }
  tft.setCursor(54, 280);
  tft.print(F("ID: "));tft.print(authFingerID);
  tft.print(F(" SIG: "));tft.println(voiceSIG);

  strcat(authVoiceSIG, voiceSIG);
  drawNextbtn(); // Display the next button on the screen
  while(1){
    //Serial.println("In Fingerprint index Enrol");
    uint16_t t_x = 0, t_y = 0;
    bool pressed = tft.getTouch(&t_x, &t_y);
    
    if(pressed && nextbtn.contains(t_x, t_y)) {
      nextbtn.press(true);  // tell the button it is pressed
    } else {
      nextbtn.press(false);  // tell the button it is NOT pressed
    }
    tft.setFreeFont(LABEL1_FONT);
    if (nextbtn.justReleased()) nextbtn.drawButton();     // draw normal
    
    if (nextbtn.justPressed()) {
      nextbtn.drawButton(true);  // draw invert
      showIndex = false;
      useCase = 'a';
      pageIndex = 7;
      return;
    }
  }
}

void authVoiceError(){
  /*tft.fillScreen(TFT_PRIMARY4); // Clear the screen
  delay(50);
  tft.pushImage(0,0, 480, 71, header); // Display the main header text;
  tft.fillRect(0, 71, 480, 46, TFT_WHITE);
  tft.setCursor(94, 107);
  tft.setTextSize(1);
  tft.setFreeFont(LABEL2_FONT);
  tft.setTextColor(TFT_BLACK,TFT_WHITE);
  tft.println("Authentification de la Voix");*/
  tft.fillRect(0, 117, 480, 320, TFT_PRIMARY4); // Clear the unwanted additional header
  tft.setCursor(50, 211);
  tft.setTextSize(1);
  tft.setFreeFont(LABEL2_FONT);
  tft.setTextColor(TFT_RED,TFT_PRIMARY4);
  tft.println(F("Erreur authentification")); // Display the enrol fingerprint error message
  tft.setCursor(25, 252);
  /*tft.setFreeFont(LABEL2_FONT);
  tft.setTextSize(1);*/
  tft.setTextColor(TFT_BLACK, TFT_PRIMARY4);

  switch(megaCmdBuffer[2]){ // We display the error message with respect to the type
    case 'A':
      tft.println(F("Module non detecté"));
      break;
    case 'B':
      tft.println(F("Erreur de chargement"));
      break;
    case 'C':
      tft.setCursor(25, 232);
      tft.println(F("Ce code ne correspond pas"));
      tft.setCursor(25, 252);
      tft.println(F("a votre empreinte digitale"));
      break;
    case 'D':
      tft.println(F("ID du code est hors intervalle"));
      break;
    case 'E':
      tft.println(F("Temps Ecoulé"));
      break;
    default:
      tft.println(F("Impossible d authentifier"));
      break;
  }
  tft.setCursor(25, 270);
  tft.println(F("Veuillez Ressayer"));
  drawHomebtn(); // Display the next button on the screen
  
  while(1){
    //Serial.println("In Voice error Enrol");
    uint16_t t_x = 0, t_y = 0;
    bool pressed = tft.getTouch(&t_x, &t_y);
    
    if(pressed && homebtn.contains(t_x, t_y)) {
      homebtn.press(true);  // tell the button it is pressed
    } else {
      homebtn.press(false);  // tell the button it is NOT pressed
    }
    tft.setFreeFont(LABEL1_FONT);
    if (homebtn.justReleased()) homebtn.drawButton();     // draw normal
    
    if (homebtn.justPressed()) {
      homebtn.drawButton(true);  // draw invert
      showIndex = true;
      useCase = ' ';
      pageIndex = 0;
      return;
    }
  }
}

void authBilan(){
  /*tft.fillScreen(TFT_PRIMARY4); // Clear the screen
  delay(50);
  tft.pushImage(0,0, 480, 71, header); // Display the main header text;
  */
  tft.fillRect(0, 71, 480, 46, TFT_WHITE);
  tft.setCursor(121, 107);
  tft.setTextSize(1);
  tft.setFreeFont(LABEL2_FONT);
  tft.setTextColor(TFT_BLACK,TFT_WHITE);
  tft.println(F("Bilan de l’Authentification"));
  tft.fillRect(0, 117, 480, 320, TFT_PRIMARY4); // Clear the unwanted additional header
  /*tft.setTextSize(1);
  tft.setFreeFont(LABEL2_FONT);
  tft.setTextColor(TFT_BLACK,TFT_PRIMARY4);*/

  while(1){
    checkInput();
    if(megaCmdBuffer[0] == 'A' && megaCmdBuffer[1] == 'B' && megaCmdBuffer[2] == 'A'){
      secondEsp32.write("$ABA*");
      tft.setTextColor(TFT_RED,TFT_PRIMARY4);
      tft.setCursor(16, 160);
      tft.println(F("Vous n'avez pas pu acceder"));
      tft.setCursor(16, 232);
      tft.println(F("a la salle a temps"));
      tft.setTextColor(TFT_BLACK,TFT_PRIMARY4);
      tft.setCursor(25, 270);
      tft.println(F("Veuillez Ressayer"));
      break;
    }
    if(megaCmdBuffer[0] == 'A' && megaCmdBuffer[1] == 'B' && megaCmdBuffer[2] == '1'){
      checkInput();
      if(megaCmdBuffer[0] == 'A' && megaCmdBuffer[1] == 'B' && megaCmdBuffer[2] == '2'){
        checkInput();
        if(megaCmdBuffer[0] == 'A' && megaCmdBuffer[1] == 'B' && megaCmdBuffer[2] == '3'){
          //String fingerTemplate = ""; // the real template
          char authTime[20]; // e.g 2023-01-06 15:40:25

          int inputArray, countInput = 0, arrayCount = 1;
          bool isAllInfo = false;

          while(arrayCount < 2){
            isAllInfo = false;
            countInput = 0;
            if(Serial.available()){
              inputArray = Serial.read();
              if((char)inputArray == '$'){ // If we have the start command
                while(!isAllInfo){
                  inputArray = Serial.read();
                  if((char)inputArray != '*'){
                    if(arrayCount == 0){ // If it is fingerprint template
                      //fingerTemplate = fingerTemplate + (char)inputArray;
                    }else if(arrayCount == 1){ // If it is auth datetime
                      authTime[countInput] = (char)inputArray;
                      countInput++;
                    }
                  }else{ // If we encounter the terminating string
                    countInput = 0;
                    arrayCount++;
                    isAllInfo = true;
                  }
                }
              }
            }
          }
          secondEsp32.write("$1T2*");
          tft.setTextColor(TFT_CSUCCESS,TFT_PRIMARY4);
          tft.setCursor(16, 160);
          tft.println(F("Vous etes desormais authentifier"));
          tft.setCursor(48, 238);
          tft.print(F("avec ID: "));
          tft.print(authVoiceSIG);
          tft.print(F(" pour: "));
          tft.println(authTime);
        }
      }
    }
    break;
  }
  
  while(1){
    checkInput2();
    if(megaCmdBuffer[0] == 'A' && megaCmdBuffer[1] == 'P' && megaCmdBuffer[2] == 'S'){
      tft.setCursor(16, 273);
      tft.print(F("Success Presence Save in Server"));
      break;
    }
    if(megaCmdBuffer[0] == 'A' && megaCmdBuffer[1] == 'P' && megaCmdBuffer[2] == 'E'){
      tft.setCursor(16, 273);
      tft.print(F("Error Presence Save in Server"));
      break;
    }
  }
  
  drawHomebtn();
  while(1){
    //Serial.println("In Fingerprint index Enrol");
    uint16_t t_x = 0, t_y = 0;
    bool pressed = tft.getTouch(&t_x, &t_y);
    
    if(pressed && homebtn.contains(t_x, t_y)) {
      homebtn.press(true);  // tell the button it is pressed
    } else {
      homebtn.press(false);  // tell the button it is NOT pressed
    }
    tft.setFreeFont(LABEL1_FONT);
    if (homebtn.justReleased()) homebtn.drawButton();     // draw normal
    
    if (homebtn.justPressed()) {
      homebtn.drawButton(true);  // draw invert
      showIndex = true;
      useCase = ' ';
      pageIndex = 0;
      return;
    }
  }
}


void enrolPagesShow(int enrolIndex){
  switch(enrolIndex){
    case 0: // enrol Matricule
      enrolMatriculePage();
      break;
    case 1: // enrol Name
      enrolNamePage();
      break;
    case 2: // enrol Class
      enrolClassPage();
      break;
    case 3: // enrol fingerprint index
      enrolFprintIndex();
      break;
    case 4: // enrol fprint logs page
      enrolFprintLogs();
      break;
    case 5: // enrol fprint success
      enrolFprintSuccess();
      break;
    case 6: // enrol fprint error
      enrolFprintError();
      break;
    case 7: // enrol voice index
      enrolVoiceIndex();
      break;
    case 8: // enrol voice logs page
      enrolVoiceLogs();
      break;
    case 9: // enrol voice success
      enrolVoiceSuccess();
      break;
    case 10: // enrol voice error
      enrolVoiceError();
      break;
    case 11: // enrol bilan
      enrolBilan();
      break;
    default: // no page selected
      showIndex = true;
      useCase = ' ';
      pageIndex = 0;
      break;
  }
  return;
}


void authPagesShow(int authIndex){
  switch(authIndex){
    case 0: // auth fingerprint index
      authFprintIndex();
      break;
    case 1: // auth fprint logs page
      authFprintLogs();
      break;
    case 2: // auth fprint success
      authFprintSuccess();
      break;
    case 3: // auth fprint error
      authFprintError();
      break;
    case 4: // auth voice index
      authVoiceIndex();
      break;
    case 5: // auth voice logs page
      authVoiceLogs();
      break;
    case 6: // auth voice success
      authVoiceSuccess();
      break;
    case 7: // auth voice error
      authVoiceError();
      break;
    case 8: // auth bilan
      authBilan();
      break;
    default: // no page selected
      break;
  }
  return;
}


void drawKeypad1()
{
  // Draw the keys for the keypad Matricule
  for (uint8_t row = 0; row < 3; row++) {
    for (uint8_t col = 0; col < 5; col++) {
      uint8_t b = col + row * 5;

      if (b < 3) tft.setFreeFont(LABEL1_FONT);
      else tft.setFreeFont(LABEL2_FONT);
      key1[b].initButton(&tft,
                        KEY1_X + col * (KEY1_W + KEY1_SPACING_X), //x
                        KEY1_Y + row * (KEY1_H + KEY1_SPACING_Y), // y, y, w, h, outline, fill, text
                        KEY1_W, //w
                        KEY1_H, // h
                        TFT_WHITE, // outline
                        keyColor1[b], // fill
                        TFT_WHITE, // Text 
                        keyLabel1[b],// Label
                        KEY1_TEXTSIZE);
      key1[b].drawButton();
    }
  }
}

void drawKeypad2()
{
  // Draw the keys for the Keypad Class
  for (uint8_t row = 0; row < 3; row++) {
    for (uint8_t col = 0; col < 5; col++) {
      uint8_t b = col + row * 5;

      if (b < 3) tft.setFreeFont(LABEL1_FONT);
      else tft.setFreeFont(LABEL2_FONT);
      key2[b].initButton(&tft,
                        KEY1_X + col * (KEY1_W + KEY1_SPACING_X), //x
                        KEY1_Y + row * (KEY1_H + KEY1_SPACING_Y), // y, y, w, h, outline, fill, text
                        KEY1_W, //w
                        KEY1_H, // h
                        TFT_WHITE, // outline
                        keyColor2[b], // fill
                        TFT_WHITE, // Text 
                        keyLabel2[b],// Label
                        KEY1_TEXTSIZE);
      key2[b].drawButton();
    }
  }
}

void drawKeypad3()
{
  // Draw the keys
  for (uint8_t row = 0; row < 3; row++) {
    for (uint8_t col = 0; col < 10; col++) {
      uint8_t b = col + row * 10;

      if (b >= 27) tft.setFreeFont(LABEL1_FONT);
      else tft.setFreeFont(LABEL2_FONT);
      key3[b].initButton(&tft,
                        KEY3_X + col * (KEY3_W + KEY3_SPACING_X), //x
                        KEY3_Y + row * (KEY3_H + KEY3_SPACING_Y), // y, y, w, h, outline, fill, text
                        KEY3_W, //w
                        KEY3_H, // h
                        TFT_WHITE, // outline
                        keyColor3[b], // fill
                        TFT_WHITE, // Text 
                        keyLabel3[b],// Label
                        KEY3_TEXTSIZE);
      key3[b].drawButton();
    }
  }
}

void drawNextbtn(){
  tft.setFreeFont(LABEL2_FONT); 
  nextbtn.initButton(&tft,400,198,100,39,TFT_BLACK,TFT_PRIMARY3,TFT_BLACK,"Suivant",1);
  nextbtn.drawButton();
}
void drawHomebtn(){
  tft.setFreeFont(LABEL2_FONT); 
  homebtn.initButton(&tft,400,198,100,39,TFT_BLACK,TFT_PRIMARY3,TFT_BLACK,"Accueil",1);
  homebtn.drawButton();
}

// Print something in the mini status bar
void status(const char *msg) {
  //tft.setCursor(STATUS1_X, STATUS1_Y);
  tft.setTextColor(TFT_BLACK, TFT_WHITE);
  tft.setTextFont(0);
  tft.setTextDatum(TC_DATUM);
  tft.setTextSize(1);
  tft.drawString(msg, STATUS1_X, STATUS1_Y);
}
