#ifndef LED_H
#define LED_H
#include <Arduino.h>
#include <stdint.h>

#define ON   true
#define OFF  false

class LEDS
{
    private:
        uint8_t pinNumber;
        uint8_t pinModality;
        bool digitalOutStatus = OFF;

    public:
        enum
        {
            DIGITAL_IN = 0,
            DIGITAL_OUT,
            PWM,
            ANALOG
        };
        static const uint16_t PWM_RANGE = 4096;
        LEDS(uint8_t Pin, uint8_t Mode);
        void writePwm(uint16_t DutyCycle);
        void writeDigital(bool Status);
        bool readDigital();
        bool getDigitalOutStatus();
        uint16_t readAnalog();
};

#endif