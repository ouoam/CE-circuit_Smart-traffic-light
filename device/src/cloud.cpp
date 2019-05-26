#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <FirebaseArduino.h>

#include "config.cpp"

class Cloud
{
private:
    bool siren = false;
    bool light = false;

    void FirebaseCheckFail()
    {
        if (Firebase.failed())
        {
            Serial.println("Firebase get failed");
            Serial.println(Firebase.error());
        }
    }

public:
    void begin()
    {
        // start ----- wifi -----
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
        // end ----- wifi -----

        // start ----- firebase -----
        Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
        Firebase.stream("control");
        // end ----- firebase -----

        // start ----- Get old data -----
        FirebaseObject control = Firebase.get("/control");
        FirebaseCheckFail();
        siren = control.getBool("siren");
        light = control.getBool("light");
        // end ----- Get old data -----
    }

    bool isChange()
    {
        bool change = false;

        FirebaseCheckFail();
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
                    siren = event.getBool("data");

                if (path == "/light")
                    light = event.getBool("data");

                change = true;
            }
        }

        return change;
    }

    bool getSiren()
    {
        return siren;
    }

    bool getLight()
    {
        return light;
    }

    void setRoad(uint32 value)
    {
        Firebase.setInt("data/road", value);
    }
};
