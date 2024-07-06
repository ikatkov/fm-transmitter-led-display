#include <Arduino.h>
#include <FmDisplay.h>

// uint16_t lastData[4];
// uint16_t point_data;

FmDisplay::FmDisplay(uint8_t clk, uint8_t dio, uint8_t cs)
{
    clkPin = clk;
    dataPin = dio;
    csPin = cs;
    pinMode(clkPin, OUTPUT);
    pinMode(dataPin, OUTPUT);
    pinMode(csPin, OUTPUT);
    digitalWrite(csPin, HIGH);
    digitalWrite(clkPin, LOW);
    digitalWrite(dataPin, LOW);
}

void FmDisplay::clear()
{
    int data[4] = {0, 0, 0, 0};
    sendRawData(MSBFIRST, data, 12, 4);
}

void FmDisplay::display(uint8_t DispData[])
{
    int SegData[4];
    for (byte i = 0; i < 4; i++)
    {
        lastData[i] = symbols[DispData[i]];
        SegData[i] = symbols[DispData[i]] + point_data;
    }
    sendRawData(MSBFIRST, SegData, 12, 4);
}

void FmDisplay::display_int(int value)
{
    if (value > 9999 || value < -999)
        return;
    boolean negative = false;
    byte digits[4];
    if (value < 0)
        negative = true;
    value = abs(value);
    digits[0] = (int)value / 1000;
    uint16_t b = (int)digits[0] * 1000;
    digits[1] = ((int)value - b) / 100;
    b += digits[1] * 100;
    digits[2] = (int)(value - b) / 10;
    b += digits[2] * 10;
    digits[3] = value - b;

    if (!negative)
    {
        for (byte i = 0; i < 3; i++)
        {
            if (digits[i] == 0)
                digits[i] = 10;
            else
                break;
        }
    }
    else
    {
        for (byte i = 0; i < 3; i++)
        {
            if (digits[i] == 0)
            {
                if (digits[i + 1] == 0)
                {
                    digits[i] = 10;
                }
                else
                {
                    digits[i] = 11;
                    break;
                }
            }
        }
    }
    display(digits);
}

void FmDisplay::display_symbols(int data[])
{
    int seg_data[4];
    for (byte i = 0; i < 4; i++)
    {
        lastData[i] = data[i];
        seg_data[i] = data[i] + point_data;
    }
    sendRawData(MSBFIRST, seg_data, 12, 4);
}

void FmDisplay::display_show_dot(bool value)
{
    if (value)
        point_data = 256;
    else
        point_data = 0;
}

void FmDisplay::sendRawData(uint8_t bitOrder, int *data, uint8_t word_length, uint8_t length)
{
    digitalWrite(csPin, LOW);
    // send AA prefix
    wordShiftOut(bitOrder, 0xAA, 8);
    for (int i = length - 1; i >= 0; i--)
    {
        wordShiftOut(bitOrder, data[i], word_length);
    }
    digitalWrite(dataPin, LOW);
    digitalWrite(clkPin, LOW);
    digitalWrite(csPin, HIGH);
}

void FmDisplay::wordShiftOut(uint8_t bitOrder, int val, uint8_t word_length)
{
    uint8_t i;
    for (i = 0; i < word_length; i++)
    {
        if (bitOrder == LSBFIRST)
        {
            digitalWrite(dataPin, !!(val & (1 << i)));
        }
        else
        {
            bool temp_value = !!(val & (1 << (word_length - 1 - i)));
            digitalWrite(dataPin, temp_value);
        }
        digitalWrite(clkPin, HIGH);
        digitalWrite(clkPin, LOW);
    }
}
