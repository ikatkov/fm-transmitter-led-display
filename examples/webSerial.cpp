#include "SimpleWebSerial.h"
#include "FmDisplay.h"

const int latchPin = 12;
const int clockPin = 11;
const int dataPin = 10;

SimpleWebSerial WebSerial;
FmDisplay fmDisplay = FmDisplay(clockPin, dataPin, latchPin);

void applyValue(JSONVar json_value)
{
    int value[] = {(int)json_value, (int)json_value, (int)json_value, (int)json_value};
    fmDisplay.sendRawData(MSBFIRST, value, 12, 4);
}

void setup()
{
    Serial.begin(57600); // should be this value for SimpleWebSerial
    delay(100);
    fmDisplay.start();
    fmDisplay.clear();

    int text[] = {_H, _o, _t, _degree};
    fmDisplay.display_symbols(text);

    WebSerial.on("dec", applyValue);
}

void loop()
{
    WebSerial.check();
    delay(5);
}
