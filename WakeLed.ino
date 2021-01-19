#include "src/display/headers/display.h"
#include "src/rotary/headers/rotary.h"

NHDST7565 Display;

// static const uint8_t D0   = 16;
// static const uint8_t D1   = 5; // PWM per i led
// static const uint8_t D2   = 4; // Rotary
// static const uint8_t D3   = 0; // rotary switch
// static const uint8_t D4   = 2; // Rotary
// static const uint8_t D5   = 14; // SCLK
// static const uint8_t D6   = 12; // DC provo con il gpio9 e metto il reset al 10
// static const uint8_t D7   = 13; // MOSI
// static const uint8_t D8   = 15; // CS
// static const uint8_t D9   = 3;
// static const uint8_t D10  = 1;

void setup()
{
    Serial.begin(115200);
    Serial.println("Programma iniziato");

}


void loop()
{
    Display.testDisplay();
    delay(1000);

}