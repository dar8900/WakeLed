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
        uint16_t pwmValueSetted = 0;

    public:
        enum
        {
            DIGITAL_IN = 0,
            DIGITAL_OUT,
            PWM
        };
        static const uint16_t PWM_RANGE = 4096;
        LEDS(uint8_t Pin, uint8_t Mode);
        void writePwm(int DutyCycle);
        void writeDigital(bool Status);
        bool readDigital();
        bool getDigitalOutStatus();
        uint16_t getPwmValue();
};

#endif