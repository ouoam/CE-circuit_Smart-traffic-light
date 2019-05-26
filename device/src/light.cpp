#include <Arduino.h>

class Light
{
private:
    const int latchPin = D5;
    const int clockPin = D0;
    const int dataPin = D4;

public:
    void begin()
    {
        pinMode(latchPin, OUTPUT);
        pinMode(clockPin, OUTPUT);
        pinMode(dataPin, OUTPUT);
    }

    void setLight(uint16 data)
    {
        digitalWrite(latchPin, LOW);

        byte out = data & B11111111;
        out = (out >> 4) | (out << 4);
        shiftOut(dataPin, clockPin, LSBFIRST, out);
        out = (data >> 8) & B11111111;
        out = (out >> 4) | (out << 4);
        shiftOut(dataPin, clockPin, LSBFIRST, out);

        digitalWrite(latchPin, HIGH);
    }
};
