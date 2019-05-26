#include <Arduino.h>

#include "road.cpp"
#include "cloud.cpp"
#include "traffic.cpp"
#include "light.cpp"

Road road;
Cloud cloud;
Traffic traffic;
Light light;

void setup()
{
    Serial.begin(9600);

    road.begin();
    cloud.begin();
    digitalWrite(D1, cloud.getSiren());
    light.begin();

    pinMode(D1, OUTPUT);
}

int old = 0;

void loop()
{
    if (road.isChange())
    {
        cloud.setRoad(road.get());
        road.print_pin_values();
    }

    if (cloud.isChange())
    {
        digitalWrite(D1, cloud.getSiren());
        traffic.setSpecial(cloud.getSiren());
        traffic.setRoadLight(cloud.getLight());
        Serial.println(road.get());
    }

    light.setLight(traffic.getLights(road.get()));

    if (old != traffic.getLights())
    {
        Serial.println(traffic.getLights());
        old = traffic.getLights();
    }
}