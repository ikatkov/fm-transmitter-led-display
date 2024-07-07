#include "SimpleWebSerial.h"
#include "FmDisplay.h"

const int csPin = 12;
const int clockPin = 11;
const int dataPin = 10;

SimpleWebSerial WebSerial;
FmDisplay fmDisplay = FmDisplay(clockPin, dataPin, csPin);

void applyValue(JSONVar json_value)
{
    int value[] = {(int)json_value, (int)json_value, (int)json_value, (int)json_value};
    fmDisplay.sendRawData(MSBFIRST, value, 12, 4);
}

void setup()
{
    pinMode(LED_BUILTIN, OUTPUT);
    Serial.begin(57600); // should be this value for SimpleWebSerial
    fmDisplay.start();
    fmDisplay.clear();

    int text[] = {_H, _o, _t, _1};
    fmDisplay.display_symbols(text);

    WebSerial.on("value", applyValue);
}

void loop()
{
    WebSerial.check();
    delay(5);
}
