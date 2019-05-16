#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <FirebaseArduino.h>

// Set these to run example.
#define FIREBASE_HOST "smart-traffic-light-ec485.firebaseio.com"
#define FIREBASE_AUTH "S6StyQdeUErhCDxL4ua29jG253DuxEJeSQCWImap"
#define WIFI_SSID "TP-LINK_B3D1F6"
#define WIFI_PASSWORD "20818043"

void setup() {
  Serial.begin(9600);

  // connect to wifi.
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("connecting");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
  Serial.println();
  Serial.print("connected: ");
  Serial.println(WiFi.localIP());
  
  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
}

void loop() {
  Firebase.setInt("road/1", 50);
  if (Firebase.failed()) {
      Serial.print("setting1 /number failed:");
      Serial.println(Firebase.error());  
      return;
  }
  Serial.println("SAVE");
  delay(10000);
}