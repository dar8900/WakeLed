#ifndef LED_H
#define LED_H
#include <Arduino.h>

#define ON   true
#define OFF  false

class LEDS
{
    private:
        uint8_t pinNumber;
        uint8_t pinModality;

    public:
        enum
        {
            DIGITAL_IN = 0,
            DIGITAL_OUT,
            PWM,
            ANALOG
        }
        LEDS(uint8_t Pin, uint8_t Mode);
        void writePwm(uint16_t DutyCycle);
        void writeDigital(bool Status);
        bool readDigital();
        uint16_t readAnalog();
};

export LEDS WakeLed;

#endif