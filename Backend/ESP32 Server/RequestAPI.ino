#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>

const char* ssid = "Hverizon";
const char* password = "pointsv$1234";

//Adresse du serveur
String serverName = "http://192.168.43.38:8080/";

void setup() {
  Serial.begin(115200); delay(10);

  Serial.print("Connecting to "); Serial.println(ssid);
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
    addPresence();
    getPresences();
    }
    else {
      Serial.println("WiFi Disconnected");
    }

    delay(300000); //5 minutes
  }

void addPresence(){ //Ajoute une présence en base de données
    HTTPClient http;
  //Ajoutons une présence
      String serverPath = serverName + "api/presence/save";
      http.begin(serverPath.c_str());
      
      http.addHeader("Content-Type", "application/json");
      int httpResponseCode = http.POST("{\"etudiantId\":\"63ad96ccf59641f5a86faf93\", \"dateheure\":\"2023-01-06T15:40:25.330Z\"}");

      if (httpResponseCode>0) {
        Serial.print("HTTP Response code: ");
        Serial.println(httpResponseCode);
        String payload = http.getString();
        Serial.println(payload);
      }
      else {
        Serial.print("Error code: ");
        Serial.println(httpResponseCode);
      }
      // Free resources
      http.end();
      //Fin de l'ajout de la présence
}

void getPresences(){ //Obtient une ou des présences à partir de la base de données
  HTTPClient http;

      String serverPath = serverName + "api/presence";
      // Your Domain name with URL path or IP address with path
      http.begin(serverPath.c_str());
      
      // If you need Node-RED/server authentication, insert user and password below
      //http.setAuthorization("REPLACE_WITH_SERVER_USERNAME", "REPLACE_WITH_SERVER_PASSWORD");
      
      // Obtenons les présences du mois de Janvier 2023
      http.addHeader("Content-Type", "application/json");
      int httpResponseCode = http.POST("{\"classeId\":\"63ad916c03c7f46bc507090e\", \"dateDebut\":\"2023-01-01T04:31:56.940Z\"}");
      
      if (httpResponseCode>0) {
        Serial.print("HTTP Response code: ");
        Serial.println(httpResponseCode);
        String payload = http.getString();
        Serial.println(payload);
      }
      else {
        Serial.print("Error code: ");
        Serial.println(httpResponseCode);
      }
      // Free resources
      http.end();
}
