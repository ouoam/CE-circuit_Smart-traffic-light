#include <Arduino.h>

//**************************************************************//
//  Name    : shiftOutCode, Hello World
//  Author  : Carlyn Maw,Tom Igoe, David A. Mellis
//  Date    : 25 Oct, 2006
//  Modified: 23 Mar 2010
//  Version : 2.0
//  Notes   : Code for using a 74HC595 Shift Register           //
//          : to count from 0 to 255
//****************************************************************

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

        byte out = (data >> 8) & B11111111;
        // out = (out >> 4) | (out << 4);
        shiftOut(dataPin, clockPin, LSBFIRST, out);

        out = data & B11111111;
        // out = (out >> 4) | (out << 4);
        shiftOut(dataPin, clockPin, LSBFIRST, out);

        digitalWrite(latchPin, HIGH);
    }
};
