#include <Arduino.h>

class Traffic
{
private:
    unsigned short state = 0, lightsData = 0, lastState;
    unsigned long timer, timeSetter, timeEmpty = 0;

    unsigned short carChecking(unsigned int data)
    {
        unsigned short out = 0;
        short dir[4] = {0, 0, 0, 0};
        for (int i = 31; i >= 0; i--)
        {
            if (i / 8 == 0)
            {
                if (data & (1 << i))
                {
                    dir[0]++;
                }
            }
            else if (i / 8 == 1)
            {
                if (data & (1 << i))
                {
                    dir[1]++;
                }
            }
            else if (i / 8 == 2)
            {
                if (data & (1 << i))
                {
                    dir[2]++;
                }
            }
            else
            {
                if (data & (1 << i))
                {
                    dir[3]++;
                }
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
        if (carChecking(data) == 0)
        {
            if (timeEmpty == 0)
            {
                timeEmpty = millis();
            }
            else if (millis() - timeEmpty == 10000)
            {
                state = 8;
                lightsData &= (B00010001 << 8) | B00010001;
                timeEmpty = 0;
            }
            else if (millis() - timeEmpty > 10000)
            {
                timeEmpty = 0;
            }
        }
        else
        {
            timeEmpty = 0;
        }
    }

    void setLighting(unsigned int data)
    {
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
                timer = 10000;
                timeSetter = millis();
                lightsData &= (B00010001 << 8) | B00010001;
            }
            else if (N == 2)
            {
                state = 1;
                timer = 15000;
                timeSetter = millis();
                lightsData &= (B00010001 << 8) | B00010001;
            }
            else
            {
                state = 1;
                timer = 20000;
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
            else if (millis() - timeSetter == timer)
            {
                lightsData &= (B00110011 << 8) | B00010011;
            }
            else if (millis() - timeSetter < timer + 3000)
            {
                lightsData |= (B00100010 << 8) | B01000010;
            }
            else
            {
                state = 2;
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
                timer = 10000;
                timeSetter = millis();
                lightsData &= (B00010001 << 8) | B00010001;
            }
            else if (W == 2)
            {
                state = 3;
                timer = 15000;
                timeSetter = millis();
                lightsData &= (B00010001 << 8) | B00010001;
            }
            else
            {
                state = 3;
                timer = 20000;
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
            else if (millis() - timeSetter == timer)
            {
                lightsData &= (B00110011 << 8) | B00110001;
            }
            else if (millis() - timeSetter < timer + 3000)
            {
                lightsData |= (B00100010 << 8) | B00100100;
            }
            else
            {
                state = 4;
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
                timer = 10000;
                timeSetter = millis();
                lightsData &= (B00010001 << 8) | B00010001;
            }
            else if (S == 2)
            {
                state = 5;
                timer = 15000;
                timeSetter = millis();
                lightsData &= (B00010001 << 8) | B00010001;
            }
            else
            {
                state = 5;
                timer = 20000;
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
            else if (millis() - timeSetter == timer)
            {
                lightsData &= (B00010011 << 8) | B00110011;
            }
            else if (millis() - timeSetter < timer + 3000)
            {
                lightsData |= (B01000010 << 8) | B00100010;
            }
            else
            {
                state = 6;
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
                timer = 10000;
                timeSetter = millis();
                lightsData &= (B00010001 << 8) | B00010001;
            }
            else if (E == 2)
            {
                state = 7;
                timer = 15000;
                timeSetter = millis();
                lightsData &= (B00010001 << 8) | B00010001;
            }
            else
            {
                state = 7;
                timer = 20000;
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
            else if (millis() - timeSetter == timer)
            {
                lightsData &= (B00110001 << 8) | B00110011;
            }
            else if (millis() - timeSetter < timer + 3000)
            {
                lightsData |= (B00100100 << 8) | B00100010;
            }
            else
            {
                state = 0;
            }
        }
        else if (state == 8)
        {
            if (carChecking(data) != 0)
                state = 0;
            else
            {
                if (millis() - timeSetter < 1000)
                {
                    lightsData |= (B01000100 << 8) | B01000100;
                }
                else if (millis() - timeSetter < 2000)
                {
                    lightsData &= (B00010001 << 8) | B00010001;
                }
                else
                {
                    timeSetter = millis();
                }
            }
        }
        else
        {
            lightsData |= (B00100010 << 8) | B00100010;
        }
    }

public:
    void setSpecial(bool var1)
    {
        if (var1)
        {
            lastState = state;
            state = 9;
        }
        else
        {
            if (lastState % 2 == 0)
            {
                state = lastState;
            }
            else
            {
                state = lastState - 1;
            }
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
    }

    uint16 getLights(uint32 data)
    {
        setLighting(data);
        return lightsData;
    }
};