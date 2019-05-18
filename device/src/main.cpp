#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <FirebaseArduino.h>

// Set these to run example.
#define FIREBASE_HOST "smart-traffic-light-ec485.firebaseio.com"
#define FIREBASE_AUTH "S6StyQdeUErhCDxL4ua29jG253DuxEJeSQCWImap"
#define WIFI_SSID "CE-ESL"
#define WIFI_PASSWORD "ceeslonly"

#define NUMBER_OF_SHIFT_CHIPS 4
#define DATA_WIDTH NUMBER_OF_SHIFT_CHIPS * 8
#define BYTES_VAL_T unsigned int

int ploadPin = D3; // Connects to Parallel load pin the 165
int dataPin = D2;  // Connects to the Q7 pin the 165
int clockPin = D0; // Connects to the Clock pin the 165

BYTES_VAL_T pinValues;
BYTES_VAL_T oldPinValues;

BYTES_VAL_T read_shift_regs()
{
  bool bitVal;
  BYTES_VAL_T bytesVal = 0;

  digitalWrite(ploadPin, LOW);
  digitalWrite(ploadPin, HIGH);

  for (int i = 0; i < DATA_WIDTH; i++)
  {
    bitVal = digitalRead(dataPin);

    bytesVal |= (bitVal << ((DATA_WIDTH - 1) - i));

    digitalWrite(clockPin, HIGH);
    digitalWrite(clockPin, LOW);
  }

  return (bytesVal);
}

void display_pin_values()
{
  char a[40];
  for (int i = 0; i < DATA_WIDTH; i++)
  {
    a[i] = ((pinValues >> i) & 1) + '0';
    Serial.print((pinValues >> i) & 1);
  }
  Serial.print("\r\n");

  a[32] = 0;
  Firebase.setString("DEBUG/road", a);
  Firebase.setInt("data/road", pinValues);
}

void setup()
{
  Serial.begin(9600);

  /* Initialize our digital pins...
    */
  pinMode(ploadPin, OUTPUT);
  pinMode(clockPin, OUTPUT);
  pinMode(dataPin, INPUT);

  digitalWrite(clockPin, LOW);
  digitalWrite(ploadPin, HIGH);

  pinMode(D1, OUTPUT);

  // connect to wifi.
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("connecting");
  while (WiFi.status() != WL_CONNECTED)
  {
    Serial.print(".");
    delay(500);
  }
  Serial.println();
  Serial.print("connected: ");
  Serial.println(WiFi.localIP());

  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
  Firebase.stream("control");

  pinValues = read_shift_regs();
  display_pin_values();
  oldPinValues = pinValues;
}

void loop()
{
  if (Firebase.failed())
  {
    Serial.println("Firebase error");
    Serial.println(Firebase.error());
  }

  pinValues = read_shift_regs();
  if (pinValues != oldPinValues)
  {
    display_pin_values();
    oldPinValues = pinValues;
  }

  if (Firebase.available())
  {
    FirebaseObject event = Firebase.readEvent();
    String eventType = event.getString("type");
    eventType.toLowerCase();

    if (eventType == "put")
    {
      String path = event.getString("path");

      Serial.print("Stream Event : ");
      Serial.print(path);
      Serial.print(" ");
      Serial.println(event.getJsonVariant("data").as<char *>());

      if (path == "/siren")
      {
        digitalWrite(D1, event.getBool("data"));
      }
    }
  }
}