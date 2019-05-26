#include <Arduino.h>

#define NUMBER_OF_SHIFT_CHIPS 4
#define DATA_WIDTH NUMBER_OF_SHIFT_CHIPS * 8
#define BYTES_VAL_T unsigned int

const int ploadPin = D3; // Connects to Parallel load pin the 165
const int clockPin = D0; // Connects to the Clock pin the 165

class Road
{
private:
    BYTES_VAL_T pinValues;
    BYTES_VAL_T oldPinValues;

public:
    void begin()
    {
        pinMode(ploadPin, OUTPUT);
        pinMode(clockPin, OUTPUT);
        pinMode(D2, INPUT);
        pinMode(D7, INPUT);
        pinMode(D6, INPUT);
        pinMode(D5, INPUT);

        digitalWrite(clockPin, LOW);
        digitalWrite(ploadPin, HIGH);

        pinValues = read_shift_regs();
        print_pin_values();
        oldPinValues = pinValues;
    };

    BYTES_VAL_T read_shift_regs()
    {
        BYTES_VAL_T bytesVal = 0;

        digitalWrite(ploadPin, LOW);
        delayMicroseconds(2);
        digitalWrite(ploadPin, HIGH);
        delayMicroseconds(4);

        for (int i = 0; i < 8; i++)
        {
            bytesVal |= (digitalRead(D2) << ((32 - 1) - i));
            bytesVal |= (digitalRead(D8) << ((24 - 1) - i));
            bytesVal |= (digitalRead(D7) << ((16 - 1) - i));
            bytesVal |= (digitalRead(D6) << ((8 - 1) - i));

            digitalWrite(clockPin, HIGH);
            delayMicroseconds(2);
            digitalWrite(clockPin, LOW);
            delayMicroseconds(2);
        }
        return (bytesVal);
    }

    void print_pin_values()
    {
        for (int i = 0; i < DATA_WIDTH; i++)
        {
            Serial.print((pinValues >> i) & 1);
            if (i % 8 == 7)
                Serial.print(" ");
        }
        Serial.println();
    }

    bool isChange()
    {
        pinValues = read_shift_regs();
        if (pinValues != oldPinValues)
        {
            oldPinValues = pinValues;
            return true;
        }
        return false;
    }

    BYTES_VAL_T get()
    {
        return pinValues;
    }
};
