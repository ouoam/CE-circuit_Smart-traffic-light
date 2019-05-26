#include <Arduino.h>

class Traffic
{
private:
    unsigned short state = 0, lightsData = 0, lastState = 10;
    unsigned long timer, timeSetter, timeEmpty = 0;
    int checky = 0;

    unsigned short carChecking(unsigned int data)
    {
        unsigned short out = 0;
        short dir[4] = {0, 0, 0, 0};
        for (int i = 31; i >= 0; i--)
        {
            if (data & (1 << i))
            {
                dir[i / 8]++;
            }
        }
        for (int i = 0; i < 4; i++)
        {
            if (dir[i] > 6)
            {
                out += (3 * pow(10, i));
            }
            else if (dir[i] > 3)
            {
                out += (2 * pow(10, i));
            }
            else if (dir[i] > 0)
            {
                out += pow(10, i);
            }
        }
        return out;
    }

    void emptyChecking(unsigned int data)
    {
        if (carChecking(data) == 0 && state != 9 && state != 8)
        {
            if (timeEmpty == 0)
            {
                timeEmpty = millis();
            }
            else if (millis() - timeEmpty <= 5250 && millis() - timeEmpty >= 4750)
            {
                state = 8;
                lightsData &= (B00010001 << 8) | B00010001;
                timeEmpty = 0;
            }
            else if (millis() - timeEmpty > 5250)
            {
                timeEmpty = 0;
            }
        }
    }

    void setLighting(unsigned int data)
    {
        Serial.println(state);
        emptyChecking(data);
        if (state == 0)
        {
            unsigned short N = carChecking(data);
            N /= 1000;
            if (N == 0)
            {
                state = 2;
                lightsData &= (B00010001 << 8) | B00010001;
            }
            else if (N == 1)
            {
                state = 1;
                timer = 5000;
                timeSetter = millis();
                lightsData &= (B00010001 << 8) | B00010001;
            }
            else if (N == 2)
            {
                state = 1;
                timer = 7000;
                timeSetter = millis();
                lightsData &= (B00010001 << 8) | B00010001;
            }
            else
            {
                state = 1;
                timer = 10000;
                timeSetter = millis();
                lightsData &= (B00010001 << 8) | B00010001;
            }
        }
        else if (state == 1)
        {
            if (millis() - timeSetter < timer)
            {
                lightsData |= (B00100010 << 8) | B10000010;
            }
            else if (carChecking(data) / 1000 != 0 && (carChecking(data) / 100) % 10 == 0 && (carChecking(data) / 10) % 10 == 0 && carChecking(data) % 10 == 0 && checky != 1)
            {
                checky = 1;
            }
            else if (millis() - timeSetter < timer + 3000 && checky == 0)
            {
                lightsData &= (B00010001 << 8) | B00010001;
                lightsData |= (B00100010 << 8) | B01000010;
            }
            else if (checky == 0)
            {
                state = 2;
            }
            else
            {
                timeSetter = millis();
                checky = 0;
            }
        }
        else if (state == 2)
        {
            unsigned short W = carChecking(data);
            W /= 100;
            W %= 10;
            if (W == 0)
            {
                state = 4;
                lightsData &= (B00010001 << 8) | B00010001;
            }
            else if (W == 1)
            {
                state = 3;
                timer = 5000;
                timeSetter = millis();
                lightsData &= (B00010001 << 8) | B00010001;
            }
            else if (W == 2)
            {
                state = 3;
                timer = 7000;
                timeSetter = millis();
                lightsData &= (B00010001 << 8) | B00010001;
            }
            else
            {
                state = 3;
                timer = 10000;
                timeSetter = millis();
                lightsData &= (B00010001 << 8) | B00010001;
            }
        }
        else if (state == 3)
        {
            if (millis() - timeSetter < timer)
            {
                lightsData |= (B00100010 << 8) | B00101000;
            }
            else if (carChecking(data) / 1000 == 0 && (carChecking(data) / 100) % 10 != 0 && (carChecking(data) / 10) % 10 == 0 && carChecking(data) % 10 == 0 && checky != 1)
            {
                checky = 1;
            }
            else if (millis() - timeSetter < timer + 3000 && checky == 0)
            {
                lightsData &= (B00010001 << 8) | B00010001;
                lightsData |= (B00100010 << 8) | B00100100;
            }
            else if (checky == 0)
            {
                state = 4;
            }
            else
            {
                timeSetter = millis();
                checky = 0;
            }
        }
        else if (state == 4)
        {
            unsigned short S = carChecking(data);
            S /= 10;
            S %= 10;
            if (S == 0)
            {
                state = 6;
                lightsData &= (B00010001 << 8) | B00010001;
            }
            else if (S == 1)
            {
                state = 5;
                timer = 5000;
                timeSetter = millis();
                lightsData &= (B00010001 << 8) | B00010001;
            }
            else if (S == 2)
            {
                state = 5;
                timer = 7000;
                timeSetter = millis();
                lightsData &= (B00010001 << 8) | B00010001;
            }
            else
            {
                state = 5;
                timer = 10000;
                timeSetter = millis();
                lightsData &= (B00010001 << 8) | B00010001;
            }
        }
        else if (state == 5)
        {
            if (millis() - timeSetter < timer)
            {
                lightsData |= (B10000010 << 8) | B00100010;
            }
            else if (carChecking(data) / 1000 == 0 && (carChecking(data) / 100) % 10 == 0 && (carChecking(data) / 10) % 10 != 0 && carChecking(data) % 10 == 0 && checky != 1)
            {
                checky = 1;
            }
            else if (millis() - timeSetter < timer + 3000 && checky == 0)
            {
                lightsData &= (B00010001 << 8) | B00010001;
                lightsData |= (B01000010 << 8) | B00100010;
            }
            else if (checky == 0)
            {
                state = 6;
            }
            else
            {
                timeSetter = millis();
                checky = 0;
            }
        }
        else if (state == 6)
        {
            unsigned short E = carChecking(data);
            E %= 10;
            if (E == 0)
            {
                state = 1;
                lightsData &= (B00010001 << 8) | B00010001;
            }
            else if (E == 1)
            {
                state = 7;
                timer = 5000;
                timeSetter = millis();
                lightsData &= (B00010001 << 8) | B00010001;
            }
            else if (E == 2)
            {
                state = 7;
                timer = 7000;
                timeSetter = millis();
                lightsData &= (B00010001 << 8) | B00010001;
            }
            else
            {
                state = 7;
                timer = 10000;
                timeSetter = millis();
                lightsData &= (B00010001 << 8) | B00010001;
            }
        }
        else if (state == 7)
        {
            if (millis() - timeSetter < timer)
            {
                lightsData |= (B00101000 << 8) | B00100010;
            }
            else if (carChecking(data) / 1000 == 0 && (carChecking(data) / 100) % 10 == 0 && (carChecking(data) / 10) % 10 == 0 && carChecking(data) % 10 != 0 && checky != 1)
            {
                checky = 1;
            }
            else if (millis() - timeSetter < timer + 3000 && checky == 0)
            {
                lightsData &= (B00010001 << 8) | B00010001;
                lightsData |= (B00100100 << 8) | B00100010;
            }
            else if (checky == 0)
            {
                state = 0;
            }
            else
            {
                timeSetter = millis();
                checky = 0;
            }
        }
        else if (state == 8)
        {
            if (carChecking(data) != 0)
            {
                Serial.println("...");
                state = 0;
                checky = 0;
            }
            else
            {
                if (millis() - timeSetter < 1000)
                {
                    lightsData &= (B00010001 << 8) | B00010001;
                }
                else if (millis() - timeSetter < 2000)
                {
                    lightsData |= (B01000100 << 8) | B01000100;
                }
                else
                {
                    timeSetter = millis();
                }
            }
        }
        else
        {
            checky = 0;
            lightsData |= (B00100010 << 8) | B00100010;
            lightsData &= (B00110011 << 8) | B00110011;
        }
    }

public:
    void setSpecial(bool var1)
    {
        if (var1)
        {
            lastState = state;
            state = 9;
            Serial.println(".....");
        }
        else if (lastState != 10)
        {
            if (lastState % 2 == 0)
            {
                state = lastState;
            }
            else
            {
                state = lastState - 1;
            }
            lastState = 10;
        }
    }

    void setRoadLight(bool var1)
    {
        if (var1)
        {
            lightsData |= (B00010001 << 8) | B00010001;
        }
        else
        {
            lightsData &= (B11101110 << 8) | B11101110;
        }
        Serial.println(lightsData);
    }

    uint16 getLights()
    {
        return lightsData;
    }

    uint16 getLights(uint32 data)
    {
        setLighting(data);
        return lightsData;
    }
};