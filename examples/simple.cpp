#include "FmDisplay.h"

const int latchPin = 12;
const int clockPin = 11;
const int dataPin = 10;

FmDisplay fmDisplay = FmDisplay(clockPin, dataPin, latchPin);

void setup()
{
    fmDisplay.clear();
}

void loop()
{
    int text[] = {_H, _o, _t, _degree};
    fmDisplay.display_symbols(text);
    delay(1000);
    fmDisplay.displayInt(1234);
}
